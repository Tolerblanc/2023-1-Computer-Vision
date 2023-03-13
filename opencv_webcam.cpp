#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int ac, char** av) {

	VideoCapture cap(1);
	
	if (!cap.isOpened())
	{
		printf("Can't open the camera");
		return -1;
	}

	Mat img;

	while (1)
	{
		cap >> img;

		imshow("camera img", img);

		if (waitKey(1) == 27)
			break;
	}


	return 0;
}