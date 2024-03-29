#include "pch.h"
#include <iostream>
#include <fstream>
#include "vec3.h"
using namespace std;
int main()
{
	int nx = 200;
	int ny = 100;

	ofstream outfile(".\\result\\FirstPicture.txt", ios_base::out);
	outfile << "P3\n" << nx << " " << ny << "\n255\n";
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			vec3 col(float(i) / float(nx), float(j) / float(ny), 0.2);
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			outfile << ir << " " << ig << " " << ib << "\n";
			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}

	return 0;
}