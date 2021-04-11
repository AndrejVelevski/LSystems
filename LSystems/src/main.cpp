#include "MainWindow.h"

int main(int argc, char** argv)
{
	int numWindows = 1;
	int width = 1024;
	int height = 768;
	MainWindow** windows = new MainWindow*[numWindows];

	for (int i = 0; i < numWindows; ++i)
	{
		windows[i] = new MainWindow(width, height, "L-Systems " + std::to_string(i+1));
	}

	for (int i = 0; i < numWindows; ++i)
	{
		windows[i]->exec();
	}

	for (int i = 0; i < numWindows; ++i)
	{
		windows[i]->waitForFinished();
	}

	for (int i = 0; i < numWindows; ++i)
	{
		delete windows[i];
	}

	delete[] windows;

    return 0;
}

#include <iostream>
#include <cmath>
#include <cstdlib>

typedef float Float;
typedef Float Axis[3];
typedef Axis Axes[3];

static void copy(const Axes& from, Axes& to)
{
    for (size_t i = 0; i != 3; ++i) {
        for (size_t j = 0; j != 3; ++j) {
            to[i][j] = from[i][j];
        }
    }
}

static void mul(Axes& mat, Axes& b)
{
    Axes result;
    for (size_t i = 0; i != 3; ++i) {
        for (size_t j = 0; j != 3; ++j) {
            Float sum = 0;
            for (size_t k = 0; k != 3; ++k) {
                sum += mat[i][k] * b[k][j];
            }
            result[i][j] = sum;
        }
    }
    copy(result, mat);
}

static void getAxes(Axes& result, Float yaw, Float pitch, Float roll)
{
    Float x = -pitch;
    Float y = yaw;
    Float z = -roll;
    Axes matX = {
      {1,     0,     0 },
      {0, cos(x),sin(x)},
      {0,-sin(x),cos(x)}
    };
    Axes matY = {
      {cos(y),0,-sin(y)},
      {     0,1,      0},
      {sin(y),0, cos(y)}
    };
    Axes matZ = {
      { cos(z),sin(z),0},
      {-sin(z),cos(z),0},
      {      0,     0,1}
    };
    Axes axes = {
      {1,0,0},
      {0,1,0},
      {0,0,1}
    };

    mul(axes, matX);
    mul(axes, matY);
    mul(axes, matZ);

    copy(axes, result);
}


static void showAxis(const char* desc, const Axis& axis, Float sign)
{
    std::cout << "  " << desc << " = (";
    for (size_t i = 0; i != 3; ++i) {
        if (i != 0) {
            std::cout << ",";
        }
        std::cout << axis[i] * sign;
    }
    std::cout << ")\n";
}

static void showAxes(const char* desc, Axes& axes)
{
    std::cout << desc << ":\n";
    showAxis("front", axes[2], 1);
    showAxis("right", axes[0], -1);
    showAxis("up", axes[1], 1);
}

int temp_main(int, char**)
{
    double pi = 3.1415926535897932384626433;
    Axes axes;
    std::cout.setf(std::ios::fixed);
    std::cout.precision(1);
    getAxes(axes, 0, 0, 0);
    showAxes("yaw=0, pitch=0, roll=0", axes);
    getAxes(axes, pi / 2, 0, 0);
    showAxes("yaw=90, pitch=0, roll=0", axes);
    getAxes(axes, 0, pi / 2, 0);
    showAxes("yaw=0, pitch=90, roll=0", axes);
    getAxes(axes, 0, 0, pi / 2);
    showAxes("yaw=0, pitch=0, roll=90", axes);
    return 0;
}