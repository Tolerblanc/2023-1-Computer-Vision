/*
** 2023 1st Semester : Computer Vision
** HomeWork03-04 : HW_03_04_scaling_center
** written by HyunJun KIM (2019204054)
** Image scaling by mouse events considering cursor position
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
int		g_CursorX = -1;
int		g_CursorY = -1;
String	g_strWindowName = "HW_03_04_scaling_center";


// Update scaling matrix
Mat	get_new_scale(Mat prevScale, double scale, int x, int y)
{
	double	newScaler;
	double	translationX;
	double	translationY;

	// Translate from mouse cursor postion to (0,0)
	// And Scale the matrix.
	// Undo translation from (0,0) to mouse cursor position.
	newScaler = prevScale.at<double>(0, 0) * scale;
	translationX = prevScale.at<double>(0, 2) * scale + x * (1 - scale);
	translationY = prevScale.at<double>(1, 2) * scale + y * (1 - scale);
	Mat newScale = (Mat_<double>(2,3) << newScaler, 0, translationX,
										0, newScaler, translationY);
	return (newScale);
}

// Mouse callback function
void mouse_callback(int event, int x, int y, int flags, void *param)
{
	if (event == EVENT_MOUSEWHEEL && flags < 0) // pull wheel
	{
		g_scale = get_new_scale(g_scale, 1.2, x, y);
		warpAffine(g_origin, g_canvas, g_scale, g_canvas.size());
	}
	if (event == EVENT_MOUSEWHEEL && flags > 0) // push wheel
	{
		g_scale = get_new_scale(g_scale, 0.8, x, y);
		warpAffine(g_origin, g_canvas, g_scale, g_canvas.size());
	}
	if (event == EVENT_MOUSEMOVE) // Record current coordinates of cursor to test in MacOS
	{
		g_CursorX = x;
		g_CursorY = y;
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
			g_scale = get_new_scale(g_scale, 1.2, g_CursorX, g_CursorY);
			warpAffine(g_origin, g_canvas, g_scale, g_canvas.size());
		}
		if (key == 45) // push wheel (= '-' key)
		{
			g_scale = get_new_scale(g_scale, 0.8, g_CursorX, g_CursorY);
			warpAffine(g_origin, g_canvas, g_scale, g_canvas.size());
		}
	}

	// Destroy all windows
	destroyAllWindows();

	return 0;
}