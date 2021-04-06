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

#define CHUNKS 4

Vector3 random_in_hemisphere(const Vector3& normal) {
	Vector3 in_unit_sphere = random_in_unit_sphere();
	if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
		return in_unit_sphere;
	else
		return -in_unit_sphere;
}

Color ray_color(const Ray& r, Hitable *world, int depth) {
	hit_record rec;
	//Ray limit exceeded, returns black
	if (depth <= 0)
		return Color(0, 0, 0);

	if (world->hit(r, 0.001, DBL_MAX, rec)) {
		Ray scattered;
		Color attenuation;
		if ( (rec.mat_ptr)->scatter(r, rec, attenuation, scattered)) {
			return attenuation * ray_color(scattered, world, depth - 1);
		}
		return Color(0, 0, 0);

		//Vector3 target = rec.p + rec.normal + random_unit_vector(); //Lambertian rendering
		//Vector3 target = rec.p + random_in_hemisphere(rec.normal); //Diffuse rendering
		//return 0.5 * ray_color(Ray(rec.p, target - rec.p), world, depth - 1);
	}
	Vector3 unit_direction = unit_vector(r.direction());
	double t = 0.5*(unit_direction.y() + 1.0);
	return (1.0 - t)* Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

int main()
{
	const double aspect_ratio = 16.0 / 9.0;
	const int image_width = 500;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int max_depth = 50;
	const int samples_per_pixel = 8;

	Camera cam(aspect_ratio, 2.0, 1.25);

	auto materialGround = make_shared<DiffuseMat>(Color(0.8, 0.8, 0.0));
	auto materialCenter = make_shared<DiffuseMat>(Color(0.7, 0.3, 0.3));
	auto materialBlue = make_shared<DiffuseMat>(Color(0.0, 0.2, 0.75));
	auto materialMetal = make_shared<MetalMat>(Color(0.8, 0.3, 0.8));

	Hitable *list[5];
	list[0] = new Sphere(Vector3(0, -100.5, -1), 100, materialGround);
	list[1] = new Sphere(Vector3(0, 0, -1), 0.5, materialCenter);
	list[2] = new Sphere(Vector3(-1, 0, -1), 0.5, materialMetal);
	list[3] = new Sphere(Vector3(1, -0.175, -0.75), 0.25, materialMetal);
	list[4] = new Sphere(Vector3(0.275, -0.15, -0.3), 0.3, materialBlue);

	Hitable *world = new HitableList(list, 5);

	int id = 0;
	const int max = image_height * image_width;
	cout << "Rendu en cours..." << endl;

	double scale = 1.0 / samples_per_pixel;

	SoftwareRenderer* display = new SoftwareRenderer(image_width, image_height);

	Color** grid = new Color*[image_width*image_height];

	while (true) {
		auto start = chrono::high_resolution_clock::now();
		
		#pragma omp parallel for num_threads(CHUNKS) 
		for (int y = 0; y < image_height; ++y) {
			for (int x = 0; x < image_width; ++x) {
				Color pixel_color(0, 0, 0);
				for (int s = 0; s < samples_per_pixel; ++s) {
					auto u = (x + random_double()) / (image_width - 1);
					auto v = (y + random_double()) / (image_height - 1);
					Ray r = cam.get_ray(u, v);
					pixel_color += ray_color(r, world, max_depth);
				}
				int id = (image_height - 1 - y) * image_width + x;
				grid[id] = computeColor(x, y, pixel_color, scale);
			}
		}
		auto stop = chrono::high_resolution_clock::now();
		auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start) / 1000.f;
		cout << "Temps de rendu : " << duration.count() << " secondes" << endl;
		cout << "FPS: " << 1/duration.count() << " FPS" << endl;

		display->writeGridOfPixels(grid, image_width, image_height);

		for (int i = 0; i < image_width * image_height; i++) {
			delete grid[i];
		}

		cam.moveZ(0.015);
		cam.moveY(0.01);

		if (display->ProcessInput() == false) {
			//Check for windows closing intention
			break;
		}
	}

	delete display;

	return 0;
}
