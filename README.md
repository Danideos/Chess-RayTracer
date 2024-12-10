# Project setup

* Download all the files from `zapoctak/main_project_dir`

* I tried to make the program runnable from all devices like this:
  
  ```shell
  mkdir build
  cmake ninja -G Ninja -S . -B build
  cd build
  ninja
  .\RealChess-RayTracer.exe
  ```
  
  Where `ninja` is path to `ninja.exe`(I have it in my system variables as `ninja`), version `1.12.1` from [Releases · ninja-build/ninja · GitHub](https://github.com/ninja-build/ninja/releases).` cmake` is path to `cmake.exe` and has version `3.29.3` from [Download CMake](https://cmake.org/download/). 

* After succesfull running of the program an empty window should open and after the program finishes running output is shown on the window and saved in the build dir as `output.bmp`

* Disclaimer: RayTracing can be computationaly quite expensive, so even on the lowest settings expect a few seconds before getting results. Because of that I have implemented simple multithreading, which needs to be turned off manually("Utils.h", read further...)
  
  ![alt text](https://github.com/Danideos/Chess-RayTracer/blob/main/OutputImages/100Ray_50Bounces_5Figures.png)
  
  # User manual
  
  Program is an implementation of a Ray Tracer on a 3D scene, which has chessboard and figures.
  User will be working with files `scripts/Utilities/Config.h` and `scripts/Utilities/Utils.h`. 
  You can examine some resulting .bmp images in `OutputImages` directory for reference.
  
  ### Config.h
  
  There are 3 types of materials based on properties that light has when interacts with them: lambertian(woodlike), dielectric(glasslike), metal. In this part is all that user needs to play with individual materials.

* lambertian: `std::make_shared<RT::Lambertian>(COLOR)`, where `COLOR` is a custom`Vec3D`type representing RGB color in range [0-1] instead of [0-255], example of black: `Vec3D{0., 0., 0.}`, green: `Vec3D{0., 1., 0.}`... 
  
  * all knowledge to create custom color is: `auto color = Vec3D{double R, double G, double B}`

* dielectric: `std::make)shared<RT::Dielectric>(REFRACTION, COLOR, TRANSPARENCY)`, where `REFRACTION` is `double` type, refraction index of how much light refracts when in contact with material(like it does in glass for example), `TRANSPARENCY` is `Vec3D` type and shows how much light gets through on scale of [0-1] for each RGB channel.

* metal: `std::make_shared<RT::Metal>(RT::Metal(COLOR, FUZZ))`, where `FUZZ` is `double` type and signifies how much fuzzy objects look in reflection(0. is not fuzzy at all, 0.01 is a little fuzzy).
  There is also chessboard configuration `FIGURE_CONFIGURATION`, where individual chess pieces can be placed on corresponding chessboard squares. Write the figure name of the desired piece as shown in the basic configuration.

### Utils.h

  All other available settings for the project are in this file. Most notably:

* `SQRT_SAMPLES`: how many rays squared get shot for each pixel(rays bounce randomly so bigger sample size is needed, more rays, less noise), biggest performance bottleneck. Set to 1 for testing, 3 for a decently unnoisy picture, sky is the limit for quality picture(10 is brutal for performance already). Testing on pictures with 5 figures with 1 ray takes around 20sec on my machine, time complexity scales linearly with amount of rays, so 9 rays(3 sqrt samples) is already around 3 minutes.

* `BOUNCES`: How many times rays bounce on hit, allows gathering more light. Set higher for good dielectric materials interaction(light can get stuck inside dielectric objects), baseline 20. Higher numbers are ok as light almost always doesn't hit anything sooner.

* `CAMERA_POS`: From where does camera look at the scene. Important note: Coordinate system is a bit weird -> `Vec3D{x, z, y}` instead of regular `Vec3D{x, y, z}`

* `CAMERA_LOOKAT`: Point wich camera is looking at, center of screen.

* `#define __MT__`: Defines multithreading, comment out if undisired(I don't recommend for performance reasons)

* `#define __SMOOTHING__`: Defines whether triangular objects should get "smoothed out"(interpolated triangle normals), disclaimer: lower values break dielectric surfaces for some reason.
  
  # Structure
  
  All code is under `scripts\`directory. Individual files and their responsibilities are:

* `Utilities`: 
  
  * `Utils.h` and `Config.h`, header files for setting up the project.

* `Random`:
  
  * `Random.h`, `Random.cpp`: implementation of independant randomness, which won't break under multithreading

* `LinearAlgebra`:
  
  * `Vector.h`: Implementation of Vector class, which is used a lot throughout the whole project(Vec3D)

* `RayTrace`: Encompasses the whole raytracing process
  
  * `Camera.h`: Camera class with all its parameters, 
  
  * `Ray.h`: Ray class, intersection with objects, reflection, refraction...
  
  * `Objects.h`: Object classes, most importantly TriangleMesh(many stored triangles)
  
  * `Material.h`: Material classes, property of objects
  
  * `Scene.h`: Scene class, where the whole process comes together. Responsible for shooting rays and calculating corresponding pixel color. Also renders and displays the app window and saves the final image(Saving the final image could be arguably in separate file...)
  
  * `PDF.h`: Calculation of probability density function of light received from different surfaces
  
  * `Chessboard.h`: Initialization of chessboard and all its pieces according to configuration
  
  * `BoundingBox.h`: Calculates Bounding Boxes for objects - Ray tracing has to go through all objects for each ray to check intersection, so creating separate big bounding objects, which contain smaller objects is more efficient

* `ObjectLoader`:
  
  * `ObjectLoader.h`: responsible for loading `.obj` triangle mesh object files, which store the custom made chess figures.

* `App.h`, `App.cpp`: SDL window application details, I had a template which I used
  
  # Functions

* `Camera.h`: only Set(), Get() functions and CalculateParams(), which uses given params to calculate other ones

* `Ray.h`: Set(), Get()... 
  
  * `RayIntersect()`: Calculates whether ray intersects with particular object, if yes assigns result to payload
    
    * `RayTriangleIntersect()` and `RayTriangleMeshIntersect()`: one function for different objects type
  
  * `Reflect()`: Reflect the ray around hit normal

* `Objects.h`: Get(), Set()...
  
  * `GetBoundingPoints()`: Returns two points, which form a bounding rectangle of the whole object
  
  * `Fit1x1()`: Fits an object to 1x1 square (x, y coordinates), with multiplier to each as provided
  
  * `Rotate()`: Rotates an object around z axis, input in degrees

* `Material.h`: 
  
  * `Scatter()`: Calculates the scattering of the ray based on individual materials
  
  * `ScatteringPDF()`: Calculates PDF of light obtained from ray according to its angle

* `Scene.h`: 
  
  * `constructor`: Creation of chess board, addiational objects, which should appear on scene
  
  * `ConvertToInt32()`, `SetPixelColor()`, `ConvertToRGB()`: Color manipulation functions, setting up the screen colors
  
  * `CalculateBoundingBoxes()`: Creates a bounding box for each hittable object
  
  * `CreateBmpFile(),``WriteColor()`: Create .bmp file of result image
  
  * `CalculateHitColor()`: The function responsible for the logic behind ray tracing of one pixel. Shoots one ray and calculates light after all the bounces.
  
  * `RayTrace()`: Does ray intersection for all objects on screen(first bounding boxes) - the most computation occurs here
  
  * `Render()`: For each pixel on screen call `CalculateHitColor()` and saves the image after the process is done
  
  * `Display()`: Displays stored color values on app window

* `PDF.h`: 
  
  * `CosinePDF` class for simulating real light dispersion, just implements some math

* `Chessboard.h` and `BoundingBox.h` have object retrieval functions, which are used in scene initialization

# Notes

I didn't manage to finish the rasterization process with camera movement as I specified in project description I sent you. I hope that the extent of the project will suffice even without it.

I used a template for this project to easily set it up in Clion, I leave the TEMPLATE_README.md for info. This is the template git repositary: [GitHub - llanillo/clion-cmake-sdl2-template: Template for Clion/Cmake and SDL2 Setup](https://github.com/llanillo/clion-cmake-sdl2-template)

Just for info, for some reason SDL2 library blocks `std::cout << ...` from appearing in terminal.


