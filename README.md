# rt

A small demo for ray-tracing based on Peter Shirley's book https://github.com/petershirley/raytracinginoneweekend.

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
- _stb_ for reading and writing images

## Screenshots

![1](output/1.png?raw=true "1")

![2](output/2.png?raw=true "2")
