#include "pch.h"

#include "utils.h"
#include "Ray.h"
#include "Hitable_list.h"
#include "Sphere.h"
#include "Camera.h"

#include "Vector3.h"
#include "Color.h"

#include "ConsoleRenderer.h"
#include "SoftwareRenderer.h"

#include <chrono>

using namespace std;
double sense = 0;

#define CHUNKS 8
Color ray_color(const Ray& r, Hitable *world, int depth) {
	hit_record rec;
	if (depth <= 0)
		return Color(0, 0, 0);

	if (world->hit(r, 0.001, FLT_MAX, rec)) {
		Vector3 target = rec.p + rec.normal + random_unit_vector();
		return 0.5 * ray_color(Ray(rec.p, target - rec.p), world, depth - 1);
	}
	Vector3 unit_direction = unit_vector(r.direction());
	double t = 0.5*(unit_direction.y() + 1.0);
	return (1.0 - t)* Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

int main()
{
	const double aspect_ratio = 16.0 / 9.0;
	const int image_width = 1920;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int max_depth = 50;
	const int samples_per_pixel = 300;

	Camera cam;

	Hitable *list[2];
	list[0] = new Sphere(Vector3(0, 0, -1), 0.5);
	list[1] = new Sphere(Vector3(0, -100.5, -1), 100);
	Hitable *world = new HitableList(list, 2);

	auto start = chrono::high_resolution_clock::now();
	int id = 0;
	const int max = image_height * image_width;
	cout << "Rendu en cours..." << endl;
	int cpt = 0;
	double scale = 1.0 / samples_per_pixel;

	Color** grid = new Color*[image_width*image_height];
	#pragma omp parallel for num_threads(CHUNKS) 
	for (int y = 0 ; y < image_height; ++y) {
		for (int x = 0; x < image_width; ++x) {
			Color pixel_color(0, 0, 0);
			for (int s = 0; s < samples_per_pixel; ++s) {
				auto u = (x + random_double()) / (image_width - 1);
				auto v = (y + random_double()) / (image_height - 1);
				Ray r = cam.get_ray(u, v);
				pixel_color += ray_color(r, world, max_depth);
			}
			int id = (image_height-1-y) * image_width + x;
			grid[id] = computeColor(x, y, pixel_color, scale);
		}
	}
	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start)/1000.f;


	SoftwareRenderer* display = new SoftwareRenderer(image_width, image_height);
	display->writeGridOfPixels(grid, image_width, image_height);

	for (int i = 0; i < image_width * image_height; i++) {
		delete grid[i];
	}

	cout << "Fin du rendu" << endl;
	cout << "Temps de rendu : " << duration.count() << " secondes" << endl;
	do
	{
		cout << '\n' << "Press a key to continue...";
	} while (cin.get() != '\n');

	delete display;

	return 0;
}
