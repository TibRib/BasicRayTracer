# Basic Ray tracer
A simple ray-tracer with animated rendering features, inspired by the book "RayTracing in one weekend" by Peter Shirley.
Written in C++(17) as a way of learning the magic behind Ray-tracing.

Currently Windows Only, you can try out the executable yourself. Please note that the calculations are very CPU-intensive.

I am also working on making it a faster with multi-processing, (usage of OpenMP for now). The project also greatly benefits from compiler (Ox) optimizations.

Pixels are written on a texture buffer and then sent to the GPU through the window (use of SDL) so we can benefit from having animated scenes. 
There is also the possibility to write the pixels directly onto the console, but I depreciated it in favor of the technique mentioned previously.

Go ahead and try the executable available in the v0.1 release ->

## Previews

**Static renders**

Quick demo animation of my basic incremental resolution preview system (static image only)![1b](Previews/1b.gif)

This gives this smooth HD image render (36 seconds) - (Using Lambertian diffuse properties) :

![RayTracer](Previews/RayTracer.JPG)



**Animated scene**

Settings used : 320x180 pixels with 5 samples

<img src="Previews/animation.gif" alt="animation" style="zoom:175%;" />



## Quick settings for trying it out

All of the below estimations are made with my current CPU : Ryzen 7 1700 @ 3.00Ghz.

Please note I restricted the image ratio to 16:9, so the user just has to enter the horizontal resolution. For example, if you want a 1280x720 image, you will just have to type 1280.

### Animated: (Animation loop = Y )

~ 45 frames per second animation

- Horizontal resolution : 100
- Samples : 10

~ 25 frames per second animation

- Horizontal resolution : 192
- Samples : 5

~ 12 frames per second animation

- Horizontal resolution : 300
- Samples : 5

~ 10 frames per second animation

- Horizontal resolution : 400
- Samples : 3

### Static: (Animation loop = N )

The static mode benefits from a progressively better quality preview.
**Quick HD preview**:
~ 3.367 seconds at 1:1 resolution

- Horizontal resolution : 1280
- Samples : 10

**Smooth HD image**:
~ 36.085 seconds at 1:1 resolution

- Horizontal resolution : 1280
- Samples : 100

**Smoother Full HD image**:
~ 155.278 seconds at 1:1 resolution

- Horizontal resolution : 1920
- Samples : 200



## Things to come:

- Depth of field camera effect (with blur)
- Ability to move the camera using the mouse/keyboard
- Linux portability
- More shapes ? (Being able to render a triangle would allow for 3D model imports).

