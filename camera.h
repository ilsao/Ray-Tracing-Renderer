#ifndef CAMERA_H
#define CAMERA_H

#include "color.h"
#include "material.h"
#include "hittable.h"
#include "hittable_list.h"

class camera
{
private:
    int image_height;
    double pixel_samples_scale;
    point3 center = point3(0, 0, 0);
    point3 pixel00_loc;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;
    vec3 defocus_disk_u;
    vec3 defocus_disk_v;

    // (relative) u: right    v: up    w: back
    vec3 u, v, w;

    void initialize() {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        pixel_samples_scale = 1.0 / samples_per_pixel;

        center = lookfrom;

        // Determine viewport dimensions
        auto theta = degree_to_radians(vfov);
        auto h = std::tan(theta / 2);
        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width = viewport_height * (double(image_width)/image_height);

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = unit_vector(cross(w, u));

        // Calculate the vectors across the horizontal and down the vertical viewport edges
        auto viewport_u = viewport_width * u;
        auto viewport_v = viewport_height * (-v);

        // Calculate the horizontal and vertical delta vectors from pixel to pixel
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel
        auto viewport_upper_left = center - (focus_dist * w) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        // Calculate the camera defocus disk basis vectors
        auto defocus_radius = focus_dist * std::tan(degree_to_radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    color ray_color(const ray& r, int depth, const hittable_list& world) const {
        if (depth <= 0)
            return color(0, 0, 0);

        hit_record rec;
        // Take interval [0.001, infinity] to deal with the "shadow acne" problem,
        // which is caused by surface intersects with itself
        if (world.hit(r, interval(0.001, infinity), rec)) {
            ray scattered;
            color attenuation;

            if (rec.mat->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, depth - 1, world);

            // The meterial absorbed all the light
            return color(0, 0, 0);
        }

        // Environmental light
        vec3 unit_direction = unit_vector(r.direction());
        auto a = 0.5 * (unit_direction.y() + 1.0);
        return (1 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }

    ray get_ray(int i, int j) const {
        auto offset = sample_square();
        auto pixel_sample = pixel00_loc 
                          + ((i + offset.x()) * pixel_delta_u)
                          + ((j + offset.y()) * pixel_delta_v);
        auto ray_origin = defocus_angle <= 0 ? center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    vec3 sample_square() const {
        // Return random sample point in the [-.5, +.5] [-.5, +.5] unit square
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    point3 defocus_disk_sample() const {
        auto p = random_in_unit_disk();
        return center + (p.x() * defocus_disk_u) + (p.y() * defocus_disk_v);
    }

public:
    double  aspect_ratio        = 1.0;      // Ideal ratio
    int     image_width         = 100;
    int     samples_per_pixel    = 10;
    int     max_depth           = 10;       // Maximum number of ray bounces into scence

    double vfov = 90;                       // Vertical FOV
    point3 lookfrom = point3(0, 0, 0);
    point3 lookat   = point3(0, 0, -1);
    vec3   vup      = vec3(0, 1, 0);        // Camera absolute up direction 

    double defocus_angle = 0;               // Control the len size
    double focus_dist = 10;                 // Distance from Lookfrom to plane of perfect focus
    
    void render(const hittable_list& world) {
        initialize();
        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; i++) {
                color pixel_color(0, 0, 0);

                // Antialiasing by randomly sample and average the color
                for (int sample = 0; sample < samples_per_pixel; sample++) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                // average the color and write to the image
                write_color(std::cout, pixel_samples_scale * pixel_color);
            }
        }

        std::clog << "\rDone.                               \n";
    }
};

#endif