/*
** 2023 1st Semester : Computer Vision
** HomeWork01-04 : HW_01_04_draw_rectangles_and_ellipses_and_brush.cpp
** written by HyunJun KIM (2019204054)
** Summary : draw rectangles, ellipses, and brushes toggle by key.
** Comments beginning '##' are handwritten
*/

#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

// Global variables
Mat    g_imgColor;
bool   g_isMousePressed = false;
int    g_mouseStartX = -1;
int    g_mouseStartY = -1;
Scalar g_rectColor; // ## Rectangle's Color to draw
int    g_prevMouseX; // ## record previous Mouse_X
int    g_prevMouseY; // ## record previous Mouse_Y
Mat    g_prevCanvas; // ## record previous Canvas
int    g_drawingMode; // ## toggle drawing shape. 0: rectangles, 1: ellipses, 2: brushes

// OpenCV Random Number Generator
RNG g_rng(getTickCount());
Scalar randomColor(RNG &g_rng)
{
    int icolor = (unsigned) g_rng;
    return Scalar(icolor&255, (icolor>>8)&255, (icolor>>16)&255);
}

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

		// ## Initialize the previous mouse position
		g_prevMouseX = x;
		g_prevMouseY = y;

		// ## determine the color of rectangle
		g_rectColor = randomColor(g_rng);

		// ## Capture Current Canvas
		g_prevCanvas = g_imgColor.clone();
    }
    // Left button released
    if (event == EVENT_LBUTTONUP)
    {
        // Flag off
        g_isMousePressed = false;
		switch (g_drawingMode)
		{
		case 0: // ## drawing Rectangles
			rectangle(g_imgColor, Point(g_mouseStartX, g_mouseStartY), Point(x, y), g_rectColor, -1);
			break;
		case 1: // ## drawing Ellipses
			ellipse(g_imgColor, RotatedRect(Point(g_mouseStartX, g_mouseStartY), \
				Size(abs(x - g_mouseStartX), abs(y - g_mouseStartY)), 0), g_rectColor, -1, 16);
			break;
		case 2: // ## brushes, if circle's linetype is 16(LINE_AA), draw a antialiased circle 
			circle(g_imgColor, Point(g_mouseStartX, g_mouseStartY), 5, g_rectColor, -1, 16);
			break;
		}
    }
	// ## Listen New Event : Mouse Cursor is moving on canvas 
	if (event == EVENT_MOUSEMOVE)
	{
		if (g_isMousePressed && g_drawingMode == 2) // ## brush doesn't have to cover the previously canvas
		{
			circle(g_imgColor, Point(x, y), 5, g_rectColor, -1, 16);
		}
		else if (g_isMousePressed) // ## Cursor is moving on canvas and Left Button is pressed
		{
			// ## Cover the previously drawn shapes with captured canvas
			uchar *src, *dst;
			int   n_channels = g_imgColor.channels();
			for (int row = 0; row <= g_imgColor.rows; ++row)
			{
				src = g_prevCanvas.ptr<uchar>(row);
				dst = g_imgColor.ptr<uchar>(row);
				for (int col = 0; col <= g_imgColor.cols; ++col)
				{
					dst[n_channels * col + 0] = src[n_channels * col + 0];
					dst[n_channels * col + 1] = src[n_channels * col + 1];
					dst[n_channels * col + 2] = src[n_channels * col + 2];
				}
			}
			switch (g_drawingMode)
			{
			case 0: // ## drawing Rectangles
				rectangle(g_imgColor, Point(g_mouseStartX, g_mouseStartY), Point(x, y), g_rectColor, -1);
				break;
			case 1: // ## drawing Ellipses
				ellipse(g_imgColor, RotatedRect(Point(g_mouseStartX, g_mouseStartY), \
					Size(abs(x - g_mouseStartX), abs(y - g_mouseStartY)), 0), g_rectColor, -1, 16);
				break;
			}
		}
		// ## Update the previous mouse position
		g_prevMouseX = x;
		g_prevMouseY = y;
	}
}

int main()
{
    // Create a black image
    const int rows = 480;
    const int cols = 640;
    g_imgColor = Mat::zeros(rows, cols, CV_8UC3);

    // Create a window
    String strWindowName = "Mouse Events";
    namedWindow(strWindowName);

    // Register the mouse callback function
    setMouseCallback(strWindowName, mouse_callback);

    // Infinite loop
    while (true)
    {
        // Display the image
        imshow(strWindowName, g_imgColor);

        // Get user input
        char key = waitKey(1);

        // ESC
        if (key == 27) break;
		else if (key == 'm') // ## Listen 'm' key event
			g_drawingMode = (g_drawingMode + 1) % 3 ;
    }

    // Destroy all windows
    destroyAllWindows();

    return 0;
}