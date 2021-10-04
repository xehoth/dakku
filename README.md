# The Dakku Renderer

[![CI](https://github.com/xehoth/dakku/actions/workflows/build.yml/badge.svg)](https://github.com/xehoth/dakku/actions/workflows/build.yml)

**Warning: This project is currently under developing and does not guarantee any consistency.**

## About

**Dakku** is a **physically based** renderer for the purpose of learning global illumination and ray tracing.   
Dakku is written in **modern C++** (>= 20) and integrates [embree](https://github.com/embree/embree) for high performance computing.   
The project structure is similar to [pbrt-v3](https://github.com/mmp/pbrt-v3) but the implementation is very different.

## Build

You are recommended to use [vcpkg](https://github.com/microsoft/vcpkg) with [manifest mode](https://github.com/microsoft/vcpkg/blob/master/docs/specifications/manifests.md) to install prerequisite libraries.  
You can take a look at [vcpkg.json](vcpkg.json) if you want to install dependency on your own without vcpkg.

Then use [CMake](https://cmake.org/) to build the project.

## Scenes

[Cornell Box](scenes/cornell_box/cornell_box.json)
![Cornell Box](scenes/cornell_box/cornell_box.png)

## Finished features

### Integrators

- Path tracing

### Materials

- BSDFS
  - Lambertian diffuse

### Accleration Structures

- [Embree](https://github.com/embree/embree)

### Lights

- Diffuse area light

### Samplers

- Random sampler

### Cameras

- Perspective camera

## TODO Lists

- Texture
- File loaders
- More materials
- More integrators
- More samplers
- More camera models
- BSSRDF
- MIS
- Volume rendering
- Denoiser
- GUI
- Self-made math library
- GPU (OptiX)
