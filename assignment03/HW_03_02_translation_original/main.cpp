#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

// Global variables
Mat		g_canvas;
Mat		g_origin;
bool	g_isMousePressed = false;
int		g_mouseStartX = -1;
int		g_mouseStartY = -1;
int		g_totalShiftX = -1;
int		g_totalShiftY = -1;
String	g_strWindowName = "HW_03_02_translation_original";

// Mouse callback function
void mouse_callback(int event, int x, int y, int flags, void *param)
{
	// Left button pressed
	if (event == EVENT_LBUTTONDOWN)
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
		g_totalShiftX += x - g_mouseStartX;
		g_totalShiftY += y - g_mouseStartY;

		Mat	M = (Mat_<double>(2,3) << 1, 0, g_totalShiftX,
									  0, 1, g_totalShiftY); // translation 
		warpAffine(g_origin, g_canvas, M, g_canvas.size());
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
	}

	// Destroy all windows
	destroyAllWindows();

	return 0;
}