# RayTracing-The-Next-Week-Demo

### Chapter1：Output an Image

下载一个ppm软件，我下载的是[XnView](https://www.xnview.com/en/)。

第一章里面给出的代码并不完整，如果想得到PPM格式的文件的话，需要在代码里加入写入txt文件的代码：

```c++
#include "pch.h"
#include <iostream>
#include <fstream>

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
			float r = float(i) / float(nx);
			float g = float(j) / float(ny);
			float b = 0.2;
			int ir = int(255.99*r);
			int ig = int(255.99*g);
			int ib = int(255.99*b);
			outfile << ir << " " << ig << " " << ib << "\n";
			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}

	return 0;
}
```

在VmView中可以看到对应的图片：

![1564110976196](C:\Users\mosswang\AppData\Roaming\Typora\typora-user-images\1564110976196.png)

### Chapter2：The vec3 class

重载了一组关于vec3的运算符。

包括向量间的加减乘除、向量与标量的加减乘除、向量的叉积点积等。

```c++
inline float dot(const vec3 &v1, const vec3 &v2)
{
	return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

inline vec3 cross(const vec3 &v1, const vec3 &v2)
{
	return vec3((v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1]),
		(-(v1.e[0] * v2.e[2] - v1.e[2] * v2.e[0])),
		(v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]));
}
```

### Chapter3：Rays,a simple camera, and background

定义了射线类，并定义了三个函数，分别返回原点、方向以及射线本身。射线本身由一个Vec3表示。

### Chapter4:

在运行的代码中加入了一个检测与球体相交的函数，这个函数可以从2D中射线与圆的相交检测函数出发去理解。

后面会将其抽离到单独的头文件中。

```c++
bool sphere :: hit(const ray& r, float t_min, float t_max, hit_record& rec)const {
	vec3 oc = r.origin() - center;
	
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;

	float discriminant = b * b - a * c;
	if (discriminant > 0)
	{
		float temp = (-b - sqrt(b*b - a * c)) / a;
		if (temp < t_max && temp > t_min)
		{
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			return true;
		}
		temp = (-b + sqrt(b*b - a * c)) / a;
		if (temp<t_max && temp>t_min)
		{
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			return true;
		}
	}
	return false;
}
```

### Chapter5：Surface normals and multiple objects.

加入了对法线方向的运算，并将对应的射线检测函数抽离到了对应的头文件中。

从球心到球上某点的向量即为该点的法线方向。

另外定义了一个类，用来计算并存储距离射线命中物体的一组点中距离射线原点最近的一个点。

### Chapter6：Antialasing

题目意为抗锯齿。

这里的做法是在每个以像素点为中心，向外距离为1的范围内采样n次，将n个采样的值进行平均即为该点最终的像素值。

在数字图像处理中类似的方式被称为均值滤波。

```c++
vec3 col(0, 0, 0);
for (int s = 0; s < ns; s++)
{
	float random = rand() % (100) / (float)(100);
	float u = float(i + random) / float(nx);
	float v = float(j + random) / float(ny);
	ray r = cam.get_ray(u, v);
	vec3 p = r.point_at_parameter(2.0);
	col += color(r, world);
}
col /= float(ns);
```

### Chapter7：Diffuse Materials

题目名为漫反射材质。

> Pick a random point s from the unit radius sphere that is tangent to the hitpoint, and send a ray from the hitpoint p to the random point s. That sphere has center (p+N):
>
> 从与命中点相切的单位半径球中选择一个随机点s，并将一条射线从命中点射向随机点s。球心为（p+N）

这里的做法是将反射方向加上一个随机的方向作为漫反射的方向。

```c++
vec3 random_in_unit_sphere()
{
	vec3 p;
	do {
		float random0 = rand() % (100) / (float)(100);
		float random1 = rand() % (100) / (float)(100);
		float random2 = rand() % (100) / (float)(100);
		p = 2.0*vec3(random0, random1, random2) - vec3(1, 1, 1);
	} while (p.squared_length() >= 1.0);
	
	return p;
}

vec3 color(const ray& r, hitable *world)
{
	hit_record rec;
	if (world->hit(r, 0.001, (numeric_limits<float>::max)(), rec))
	{
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		return 0.5*color(ray(rec.p,target - rec.p), world);
	}
	else
	{
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5*(unit_direction.y() + 1.0);
		return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}
```

### Chapter8：Metal

第八章加上了简单的材质。

// TODO:

### Chapter9：Dielectrics

### Chapter10：Positionable camera

