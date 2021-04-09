#include "pch.h"

#include "utils.h"
#include "Ray.h"
#include "Hitable_list.h"
#include "Hitable.h"
#include "Sphere.h"
#include "Camera.h"

#include "Vector3.h"
#include "Color.h"

#include "ConsoleRenderer.h"
#include "SoftwareRenderer.h"

#include "Material.h"
#include "MetalMat.h"
#include "DiffuseMat.h"

#include <chrono>
#include <memory>

using namespace std;
double sense = 0;

/* Update this value with your CPU-core count. */
#define THREADS_COUNT 8

/* Returns the end color the camera ray has to display */
Color ray_color(const Ray& r, Hitable *world, int depth) {
	hit_record rec;

	if (depth <= 0) //Ray limit exceeded, returns black
		return Color(0, 0, 0);
	//Avoid noisy effects by aproximating zero (0.001)
	if (world->hit(r, 0.001, infinity, rec)) {
		Ray scattered;
		Color attenuation;
		if ( (rec.mat_ptr)->scatter(r, rec, attenuation, scattered)) {
			return attenuation * ray_color(scattered, world, depth - 1);
		}
		return Color(0, 0, 0);
	}
	Vector3 unit_direction = unit_vector(r.direction());
	double t = 0.5*(unit_direction.y() + 1.0);
	return (1.0 - t)* Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

/* Returns the highest power of 2 for a given integer */
int highestPowerof2(int n){
	for (int i = n; i >= 1; i--){
		if ((i & (i - 1)) == 0)
			return i;
	}
	return 0;
}

/* Affects the program's parameters after prompting the user */
bool userInput(int& image_width, int& samples, bool& animate) {
	cout << "Welcome to my C++ Raytracer ! This basic Ray tracer is only software accelerated." << endl
		<< "Inspired by the book 'Raytracing in one weekend' - Peter Shirley." << endl
		<< "Currently makes use of :" << endl 
		<<" - OpenMP for basic parallel processing,"<< endl
		<<" - SDL for creating a window, texture and writing pixels to the GPU" << endl;

	cout << endl << "*Please note this program is very cpu intensive*" << endl
	<< endl << "Options of compilation :" << endl;

	cout << "Horizontal Resolution ? (ex : 640): "; cin >> image_width;
	cout << "Samples ? (1-200 (Slow, low values recommended) ) : "; cin >> samples;
	char animateChar;
	do {
		cout << "Animation loop ? (Y/N) : "; cin >> animateChar;
	} while (animateChar != 'Y' && animateChar != 'N');
	animate = animateChar == 'Y' ? true : false;
	return true;
}

int main(){
	const double aspect_ratio = 16.0 / 9.0;
	const int max_depth = 50;
	//Default values
	bool animated = false;
	int image_width = 800;
	int image_height = static_cast<int>(image_width / aspect_ratio);
	int samples_per_pixel = 50;
	
	/* Display text and ask the user about his parameters */
	if( userInput(image_width, samples_per_pixel, animated))
		image_height = static_cast<int>(image_width / aspect_ratio);

	Camera cam(90.0, aspect_ratio);

	/* Material initialisations, using shared pointers for optimal reusability across the objects */
	auto materialGround = shared_ptr<DiffuseMat>(new DiffuseMat(Color(0.8, 0.8, 0.0)));
	auto materialCenter = shared_ptr<DiffuseMat>(new DiffuseMat(Color(0.7, 0.3, 0.3)));
	auto materialBlue   = shared_ptr<DiffuseMat>(new DiffuseMat(Color(0.0, 0.2, 0.75)));
	auto materialGold   = shared_ptr<MetalMat>(new MetalMat(Color(0.8, 0.6, 0.2)));
	auto materialMetal  = shared_ptr<MetalMat>(new MetalMat(Color(0.8, 0.8, 0.8)));

	/* (Currently static) list of objects in the scene */
	Hitable *list[5];
	/* Instanciations of the objects in the scene */
	list[0] = new Sphere(Vector3(0, -100.5, -1), 100, materialGround);
	list[1] = new Sphere(Vector3(0, 0, -1), 0.5, materialCenter);
	list[2] = new Sphere(Vector3(-1, 0, -1), 0.5, materialMetal);
	list[3] = new Sphere(Vector3(1, -0.175, -0.75), 0.25, materialGold);
	list[4] = new Sphere(Vector3(0.368, -0.34, -0.42), 0.16, materialBlue);

	Hitable *world = new HitableList(list, 5);

	// Renderer object initialization
	SoftwareRenderer* display = new SoftwareRenderer(image_width, image_height);

	// The grid of colors that will be sent to the display
	Color** grid = new Color*[image_width*image_height]; 

	double animTime = 0.0; //real counter for easy animation movements
	
	bool processFrames = true; // Render next frame boolean

	/* Base ratio increment initialization : 
	*  Makes precision jumps of 32, then 16, 8, 4, ..., 1 for non-animated renders */
	int inc = min(32,highestPowerof2(image_width));

	/* Animation related properties */
	if (animated) {
		inc = 1; //The increment is currently not supported in animation mode, we reset it as 1.
	}

	// importance of pixels based on the given amount of samples
	double scale = 1.0 / samples_per_pixel; 

	cout << endl << "Processing started..." << endl;

	while (processFrames) {
		auto start = chrono::high_resolution_clock::now(); //Counter for frametime estimation
		
		if(!animated) cout << "Rendering at " << inc << ":1 resolution" << endl;
	
	//Parallel processing instruction
	#pragma omp parallel for num_threads(THREADS_COUNT) 
		for (int y = 0; y < image_height; ++y) {
			for (int x = 0; x < image_width; x+=inc) { //x can jump from 1 to the increment ratio
				Color pixel_color(0, 0, 0);
				/* For each pixel, we define a vector, and compute the color of the Ray(u,v) */
				for (int s = 0; s < samples_per_pixel; ++s) {
					auto u = (x + random_double()) / (image_width - 1);
					auto v = (y + random_double()) / (image_height - 1);
					Ray r = cam.get_ray(u, v);
					pixel_color += ray_color(r, world, max_depth);
				}
				//Affect the corresonding (y,x) pixel on the 1D array to the ARGB color
				int id = (image_height - 1 - y) * image_width + x;
				grid[id] = computeColor(x, y, pixel_color, scale);
				//Initialize the next n pixels if the increment ratio > 1
				for (int j = 1; j < inc; j++) { 
					grid[id + j] = new Color(grid[id]->r(), grid[id]->g(), grid[id]->b());
				}
			}
		}
		auto stop = chrono::high_resolution_clock::now(); //Take the time at the end of the frame process;
		auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start) / 1000.f; //duration = frametime

		//Send the processed color array to the texture buffer
		display->writeGridOfPixels(grid, image_width, image_height);
		//clean up the memory taken by the color array
		for (int i = 0; i < image_width * image_height; i++) {
			delete grid[i];
		}
		//Display different informations depending on the type of render
		if (animated) {
			cout << "Current FPS: " << 1 / duration.count() << " FPS" << endl;
			if (display->ProcessInput() == false) {
				break;
			}
			animTime += 0.1; //update the cpu-time counter for animations
			auto s = (Sphere*)list[1]; //Polymorphism to get sphere attributes
			s->center.e[1] = 0.5 + cos(animTime)/2; //Movement of the sphere
		}
		else {
			cout << "Rendering took " << duration.count() << " seconds." << endl;
			//Once the frame is processed, if the ratio is not 1:1, divide the ratio by 2
			if (inc == 1) {
				processFrames = false;
			} 
			else { inc = inc / 2; }
		}
	}
	cout << "Rendering ended. Close window to quit the program"<< endl;

	/* Destruction of the shapes in the scene */
	for (int i = 0; i < (sizeof(list)/sizeof(Hitable*)); i++) {
		delete list[i];
	}

	/* In case rendering is ended, we want to keep the display on until the user willingly closes it*/
	if (animated == false) { 
		//ProcessInput will return false if the window is closed.
		while (display->ProcessInput()) { 
			//do nothing!
		}
	}
	//Free the renderer
	delete display;

	return 0;
}
