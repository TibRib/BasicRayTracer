#include "pch.h"
#include "utils.h"
#include "Ray.h"
#include "Hitable_list.h"
#include "Sphere.h"
#include "Color.h"
#include "Camera.h"

#include <chrono>

using namespace std;
float sense = 0;

Color ray_color(const Ray& r, Hitable *world, int depth) {
	hit_record rec;
	if (depth <= 0)
		return Color(0, 0, 0);

	if (world->hit(r, 0.001, FLT_MAX, rec)) {
		Vector3 target = rec.p + rec.normal + random_in_unit_sphere();
		return 0.5 * ray_color(Ray(rec.p, target - rec.p), world, depth - 1);
	}
	Vector3 unit_direction = unit_vector(r.direction());
	float t = 0.5*(unit_direction.y() + 1.0);
	return (1.0 - t)* Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

HWND consoleWindow = GetConsoleWindow();    // Get a console handle

int main()
{
	HDC consoleDC = GetDC(consoleWindow);

	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 300;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int max_depth = 50;
	const int samples_per_pixel = 100;

	Camera cam;

	Hitable *list[2];
	list[0] = new Sphere(Vector3(0, 0, -1), 0.5);
	list[1] = new Sphere(Vector3(0, -100.5, -1), 100);
	Hitable *world = new HitableList(list, 2);

	auto start = std::chrono::high_resolution_clock::now();

	for (int y = image_height - 1; y >= 0; --y) {
		for (int x = 0; x < image_width; ++x) {
			Color pixel_color(0, 0, 0);
			for (int s = 0; s < samples_per_pixel; ++s) {
				auto u = (x + random_double()) / (image_width - 1);
				auto v = (y + random_double()) / (image_height - 1);
				Ray r = cam.get_ray(u, v);
				pixel_color += ray_color(r, world, max_depth);
			}
			write_color(x, y, &consoleDC, pixel_color, samples_per_pixel);
		}
	}

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);


	std::cout << "Fin du rendu" << std::endl;
	std::cout << "Temps de processing : " << duration.count() << "secondes" << std::endl;
	ReleaseDC(consoleWindow, consoleDC);
	return 0;
}
