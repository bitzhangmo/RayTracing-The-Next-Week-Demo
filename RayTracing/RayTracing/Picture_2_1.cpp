#include "pch.h"
#include <iostream>
#include <fstream>
#include "hitable_list.h"
#include "float.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectrics.h"
#include "moving_sphere.h"

using namespace std;

vec3 color(const ray& r, hitable *world, int depth)
{
	hit_record rec;
	if (world->hit(r, 0.001, (numeric_limits<float>::max)(), rec))
	{
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return attenuation * color(scattered, world, depth + 1);
		else return vec3(0, 0, 0);
	}
	else
	{
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5*(unit_direction.y() + 1.0);
		return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}

hitable *random_scene()
{
	int n = 500;
	hitable **list = new hitable*[n + 1];
	list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
	int i = 1;
	for (int a = -10; a < 10; a++)
	{
		for (int b = -10; b < 10; b++)
		{
			float choose_mat = rand() % (100) / (float)(100);
			vec3 center((a + 0.9*(rand() % (100) / (float)(100))), 0.2, (b + 0.9*(rand() % (100) / (float)(100))));
			if ((center - vec3(4, 0, 2.0)).length() > 0.9)
			{
				if (choose_mat < 0.8)
				{
					float random0 = rand() % (100) / (float)(100);
					float random1 = rand() % (100) / (float)(100);
					float random2 = rand() % (100) / (float)(100);
					float random3 = rand() % (100) / (float)(100);
					float random4 = rand() % (100) / (float)(100);
					float random5 = rand() % (100) / (float)(100);
					list[i++] = new moving_sphere(center,center + vec3(0,0.5*(rand() % (100) / (float)(100)),0),0.0,1.0,0.2, new lambertian(vec3(random0*random1, random2*random3, random4*random5)));
				}
				else if (choose_mat < 0.95)
				{
					float random0 = rand() % (100) / (float)(100);
					float random1 = rand() % (100) / (float)(100);
					float random2 = rand() % (100) / (float)(100);
					float random3 = rand() % (100) / (float)(100);
					list[i++] = new sphere(center, 0.2, new metal(vec3(0.5*(1 + random0), 0.5*(1 + random1), 0.5*(1 + random2)), 0.5*random3));
				}
				else
				{
					list[i++] = new sphere(center, 0.2, new dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
	list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

	return new hitable_list(list, i);
}


int main()
{
	int nx = 400;
	int ny = 200;
	int ns = 10;
	ofstream outfile(".\\result\\Picture_2_1.txt", ios_base::out);
	outfile << "P3\n" << nx << " " << ny << "\n255\n";

	hitable *world = random_scene();
	float persent = 0.0f;
	float count = 0;
	float total = ny * nx;
	vec3 lookfrom(13, 2, 3);
	vec3 lookat(0, 0, 0);
	float dist_to_focus = 10.0;
	float aperture = 0.0;
	camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(nx) / float(ny), aperture, dist_to_focus,0.0,1.0);
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++)
			{
				float random = rand() % (100) / (float)(100);
				float u = float(i + random) / float(nx);
				float v = float(j + random) / float(ny);
				ray r = cam.get_ray(u, v);
				vec3 p = r.point_at_parameter(2.0);
				col += color(r, world, 0);
			}
			count++;
			persent = float(count / total);
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			outfile << ir << " " << ig << " " << ib << "\n";
			//std::cout << ir << " " << ig << " " << ib << "\n";
			std::cout << persent << "\n";
		}
	}

	return 0;
}