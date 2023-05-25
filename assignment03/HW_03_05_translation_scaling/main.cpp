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
bool	g_isMousePressed = false;
int		g_mouseStartX = -1;
int		g_mouseStartY = -1;
String	g_strWindowName = "translation";

Mat	get_new_scale(Mat prevScale, double scale, int x, int y)
{
	double	newScaler;
	double	translationX;
	double	translationY;

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
	if (event == EVENT_MOUSEMOVE)
	{
		g_CursorX = x;
		g_CursorY = y;
	}
	if (event == EVENT_LBUTTONDOWN) // Left button pressed
	{
		// Flag on
		g_isMousePressed = true;

		// Record the mouse position
		g_mouseStartX = x;
		g_mouseStartY = y;
	}
	// Left button released
	if (event == EVENT_LBUTTONUP)
	{
		// Flag off
		g_isMousePressed = false;
	}
	if (event == EVENT_MOUSEMOVE && g_isMousePressed)
	{
		g_scale.at<double>(0, 2) += x - g_mouseStartX;
		g_scale.at<double>(1, 2) += y - g_mouseStartY;
		warpAffine(g_origin, g_canvas, g_scale, g_canvas.size());
		g_mouseStartX = x;
		g_mouseStartY = y;
	}
}

int main()
{
	// Create a window
	namedWindow(g_strWindowName);	

	g_canvas = imread("../messi5.jpg");
	g_origin = imread("../messi5.jpg");
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