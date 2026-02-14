# Ray-Tracing-Renderer

A simple C++ ray tracing renderer implementation following [*Ray Tracing in One Weekend*](https://raytracing.github.io/books/RayTracingInOneWeekend.html).

## Features

- Material support for diffuse, metal, and dielectric (glass) spheres.
- Depth of field and anti-aliasing via camera sampling.
- Recursive ray tracing for realistic light transport.

## Build

Run from the project root:

```bash
cmake -S . -B build
cmake --build build --config Release
```

## Run

### Windows PowerShell

```powershell
.\build\main.exe | Set-Content image.ppm -Encoding String
```

### Linux/macOS

```bash
./build/main > image.ppm
```

## Output

- The rendered image is saved as `image.ppm`.
- Open it with any image viewer that supports the PPM format.
