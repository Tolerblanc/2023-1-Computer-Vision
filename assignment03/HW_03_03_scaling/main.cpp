/*
** 2023 1st Semester : Computer Vision
** HomeWork03-03 : HW_03_03_scaling
** written by HyunJun KIM (2019204054)
** Image scaling by mouse events
** In MacOS, Mouse Wheel event doesn't work. test with '-' and '+' key.
*/

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

// Global variables
Mat		g_canvas;
Mat		g_origin;
Mat		g_scale;
String	g_strWindowName = "HW_03_03_scaling";

// Mouse callback function
void mouse_callback(int event, int x, int y, int flags, void *param)
{
	if (event == EVENT_MOUSEWHEEL)
	{
		double	nextScale = flags < 0 ? 1.2 : 0.8;
		g_scale = g_scale * nextScale; // scalar multiplication to scaling matrix
		warpAffine(g_origin, g_canvas, g_scale, g_canvas.size());
	}
}

int main()
{
	// Create a window
	namedWindow(g_strWindowName);	

	g_canvas = imread("../messi5.jpg");
	g_origin = imread("../messi5.jpg");

	// scaling matrix
	g_scale = (Mat_<double>(2,3) << 1, 0, 0,
									0, 1, 0);

	// Register the mouse callback function
	setMouseCallback(g_strWindowName, mouse_callback);
	// Infinite loop
	while (true)
	{
		// Display the image
		imshow(g_strWindowName, g_canvas);

		// Get user input
		char key = waitKey(1);

		// ESC
		if (key == 27) break;

		if (key == 61) // pull wheel (= '+' key)
		{
			g_scale = g_scale * 1.2;
			warpAffine(g_origin, g_canvas, g_scale, g_canvas.size());
		}
		if (key == 45) // push wheel (= '-' key)
		{
			g_scale = g_scale * 0.8;
			warpAffine(g_origin, g_canvas, g_scale, g_canvas.size());
		}
	}

	// Destroy all windows
	destroyAllWindows();

	return 0;
}