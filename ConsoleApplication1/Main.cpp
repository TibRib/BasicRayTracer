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
#include <list>

using namespace std;
double sense = 0;

#define CHUNKS 8

Color ray_color(const Ray& r, Hitable *world, int depth) {
	hit_record rec;
	//Ray limit exceeded, returns black
	if (depth <= 0)
		return Color(0, 0, 0);

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

int highestPowerof2(int n)
{
	int res = 0;
	for (int i = n; i >= 1; i--)
	{
		// If i is a power of 2
		if ((i & (i - 1)) == 0)
		{
			res = i;
			break;
		}
	}
	return res;
}

int main()
{
	const double aspect_ratio = 16.0 / 9.0;
	const int image_width = 340; //96
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int max_depth = 50;
	const int samples_per_pixel = 5;

	Camera cam(90.0, aspect_ratio);

	auto materialGround = make_shared<DiffuseMat>(Color(0.8, 0.8, 0.0));
	auto materialCenter = make_shared<DiffuseMat>(Color(0.7, 0.3, 0.3));
	auto materialBlue = make_shared<DiffuseMat>(Color(0.0, 0.2, 0.75));
	auto materialGold = make_shared<MetalMat>(Color(0.8, 0.6, 0.2));
	auto materialMetal = make_shared<MetalMat>(Color(0.8, 0.8, 0.8));

	Hitable *list[5];
	list[0] = new Sphere(Vector3(0, -100.5, -1), 100, materialGround);
	list[1] = new Sphere(Vector3(0, 0, -1), 0.5, materialCenter);
	list[2] = new Sphere(Vector3(-1, 0, -1), 0.5, materialMetal);
	list[3] = new Sphere(Vector3(1, -0.175, -0.75), 0.25, materialGold);
	list[4] = new Sphere(Vector3(0.368, -0.34, -0.42), 0.16, materialBlue);

	Hitable *world = new HitableList(list, 5);

	int id = 0;
	const int max = image_height * image_width;
	cout << "Rendu en cours..." << endl;

	double scale = 1.0 / samples_per_pixel;

	SoftwareRenderer* display = new SoftwareRenderer(image_width, image_height);

	Color** grid = new Color*[image_width*image_height];
	double dist = 0;

	bool animated = true;
	bool processFrames = true;

	int inc = min(5,highestPowerof2(image_width));

	while (processFrames) {
		auto start = chrono::high_resolution_clock::now();
		cout << "Rendu " << inc << ":1" << endl;
		
	#pragma omp parallel for num_threads(CHUNKS) 
		for (int y = 0; y < image_height; ++y) {
			for (int x = 0; x < image_width; x+=inc) {
				Color pixel_color(0, 0, 0);
				for (int s = 0; s < samples_per_pixel; ++s) {
					auto u = (x + random_double()) / (image_width - 1);
					auto v = (y + random_double()) / (image_height - 1);
					Ray r = cam.get_ray(u, v);
					pixel_color += ray_color(r, world, max_depth);
				}

				int id = (image_height - 1 - y) * image_width + x;
				grid[id] = computeColor(x, y, pixel_color, scale);
				for (int j = 1; j < inc; j++) {
					grid[id + j] = new Color(grid[id]->r(), grid[id]->g(), grid[id]->b());
				}
			}
		}
		auto stop = chrono::high_resolution_clock::now();
		auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start) / 1000.f;
		cout << "Temps de rendu : " << duration.count() << " secondes" << endl;

		display->writeGridOfPixels(grid, image_width, image_height);

		for (int i = 0; i < image_width * image_height; i++) {
			delete grid[i];
		}

		if (animated) {
			cout << "FPS: " << 1 / duration.count() << " FPS" << endl;
			if (display->ProcessInput() == false) {
				//Check for windows closing intention
				break;
			}
		}
		else {
			if (inc == 1) {
				processFrames = false;
			}
			inc = (inc > 1) ? inc / 2 : 1;
		}

		//cam.moveZ(0.015);
		dist += 0.1;
		auto s = (Sphere*)list[1];
		s->center.e[1] = 0.5 + cos(dist)/2;
		//cam.setCoords(Vector3(cos(dist*2.0),0.5 ,-1.0+sin(dist)));

	}

	cout << "Rendu fini" << endl;

	if (animated == false) {
		while (display->ProcessInput()) {
		}
	}

	delete display;

	return 0;
}
