# RayTracing-The-Next-Week-Demo

### Chapter1：

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

### Chapter2：

重载了一组关于vec3的运算符。

### Chapter3：

定义了射线类，并定义了三个函数，分别返回原点、方向以及射线本身。

