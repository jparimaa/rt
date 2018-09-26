# rt

A small demo for ray-tracing.

- Different materials such as lambertian, reflective, refractive and emitting (light)
- Depth of field
- Multi-threaded
- Bounding volumen hierarchy with AABB
- Image textures

## Clone and build

`git clone --recurse-submodules https://github.com/jparimaa/rt.git`

Configure with CMake and build, e.g. `cmake . && make`

## Submodules

- _glm 0.9.9.1_ for vector math
- _stb_ for writing the output image)

## Screenshots

![output](output/output.png?raw=true "output")
