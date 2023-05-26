/*
** 2023 1st Semester : Computer Vision
** HomeWork03-06 : HW_03_06_rotation_translation_scaling
** written by HyunJun KIM (2019204054)
** Image translation, scaling, rotation by mouse events
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
bool	g_isMouseLeftPressed = false;
bool	g_isMouseRightPressed = false;
int		g_mouseLeftStartX = -1;
int		g_mouseLeftStartY = -1;
int		g_mouseRightStartX = -1;
int		g_mouseRightStartY = -1;
int		g_imageCenterX = -1;
int		g_imageCenterY = -1;
String	g_strWindowName = "HW_03_06_rotation_translation_scaling";

// Update scaling matrix
Mat	get_new_scale(Mat prevScale, double scale, int x, int y)
{
	Mat newScale;
	Mat delta = (Mat_<double>(3,3) << scale, 0, x * (1 - scale),
										0, scale, y * (1 - scale),
										0, 0, 1);
	Mat temp = (Mat_<double>(1,3) << 0, 0, 1); // Add to prevScale to make 3 by 3 block matrix

	newScale = prevScale;
	newScale.push_back(temp);
	newScale = delta * newScale; // Multiply matrix to get new scale(transformation) matrix
	newScale.pop_back(); // Make the matrix to 2 by 3

	// Calculate the coordinates of image for smooth center rotation.
	g_imageCenterX = static_cast<int>(round(g_imageCenterX * scale + x * (1 - scale)));
	g_imageCenterY = static_cast<int>(round(g_imageCenterY * scale + y * (1 - scale)));
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
	if (event == EVENT_LBUTTONDOWN) // Left button pressed
	{
		// Flag on
		g_isMouseLeftPressed = true;

		// Record the mouse position
		g_mouseLeftStartX = x;
		g_mouseLeftStartY = y;
	}
	// Left button released
	if (event == EVENT_LBUTTONUP)
	{
		// Flag off
		g_isMouseLeftPressed = false;
	}
	// Left Button dragging
	if (event == EVENT_MOUSEMOVE && g_isMouseLeftPressed)
	{
		g_scale.at<double>(0, 2) += x - g_mouseLeftStartX;
		g_scale.at<double>(1, 2) += y - g_mouseLeftStartY;
		warpAffine(g_origin, g_canvas, g_scale, g_canvas.size());
		g_imageCenterX += x - g_mouseLeftStartX;
		g_imageCenterY += y - g_mouseLeftStartY;
		g_mouseLeftStartX = x;
		g_mouseLeftStartY = y;
	}
	if (event == EVENT_RBUTTONDOWN) // Right button pressed
	{
		// Flag on
		g_isMouseRightPressed = true;

		// Record the mouse position
		g_mouseRightStartX = x;
		g_mouseRightStartY = y;
	}
	// Right button released
	if (event == EVENT_RBUTTONUP)
	{
		// Flag off
		g_isMouseRightPressed = false;
	}
	// Right Button dragging
	if (event == EVENT_MOUSEMOVE && g_isMouseRightPressed)
	{
		// Get two vectors from image center to previous, current cursor position.
		Mat prevVector = (Mat_<double>(1,3) << g_mouseRightStartX, g_mouseRightStartY, 0);
		Mat currVector = (Mat_<double>(1,3) << x, y, 0);

		normalize(prevVector, prevVector);
		normalize(currVector, currVector);

		// if crossedZ > 0, vector moves counterclockwise. else, vector moves clockwise.
		double crossedZ = currVector.cross(prevVector).at<double>(0, 2);
		// Calibrate the orientation by seperating the case of the cursor to the left and right based on the center of the image
		crossedZ = x < g_imageCenterX ? -crossedZ : crossedZ;

		// get the theta between prevVector and currVector.
		double theta = atan2(crossedZ, prevVector.dot(currVector)) * 180.0 / CV_PI;

		if (theta > 0.1 || theta < -0.1)
		{
			// Translate center of the image to origin
			Mat translationToOrigin = (Mat_<double>(3,3) << 1, 0, -g_imageCenterX,
															0, 1, -g_imageCenterY,
															0, 0, 1);
		
			Mat rotation = getRotationMatrix2D(Point2f(0, 0), theta, 1); // rotate around origin
			Mat	temp = (Mat_<double>(1,3) << 0, 0, 1);
			rotation.push_back(temp);
		
			// Translate back to the original center
			Mat translationBack = (Mat_<double>(3,3) << 1, 0, g_imageCenterX,
														0, 1, g_imageCenterY,
														0, 0, 1);
		
			// Combine the transformations by matrix multiplication
			Mat combined = translationBack * rotation * translationToOrigin;
		
			// Apply the combined transformation
			g_scale.push_back(temp);
			g_scale = combined * g_scale;
			g_scale.pop_back();
	
			warpAffine(g_origin, g_canvas, g_scale, g_canvas.size());
		}
		g_mouseRightStartX = x;
		g_mouseRightStartY = y;
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
	g_imageCenterX = static_cast<int>(round(g_origin.cols / 2));
	g_imageCenterY = static_cast<int>(round(g_origin.rows / 2));

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