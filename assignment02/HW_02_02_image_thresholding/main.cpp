/*
** 2023 1st Semester : Computer Vision
** HomeWork02-02 : HW_02_02_image_thresholding
** written by HyunJun KIM (2019204054)
** Image thresholding
*/

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int	main()
{
	Mat img_original = imread("opencv-logo-white.png");
	if (img_original.empty())
	{
		cout << "Error: No image exists!" << endl;
        exit(-1);
	}

	vector<Mat>	bgrPlanes;
	split(img_original, bgrPlanes);

	Mat	bgr_thresholding[3];
	threshold(bgrPlanes[0], bgr_thresholding[0], 10, 255, THRESH_BINARY);
	threshold(bgrPlanes[1], bgr_thresholding[1], 10, 255, THRESH_BINARY);
	threshold(bgrPlanes[2], bgr_thresholding[2], 10, 255, THRESH_BINARY);

	Mat	im_result_bg, im_result_bgr, im_result_bgr_converted;
	bitwise_and(bgrPlanes[0], bgrPlanes[1], im_result_bg);
	bitwise_and(im_result_bg, bgrPlanes[2], im_result_bgr);
	cvtColor(im_result_bgr, im_result_bgr_converted, COLOR_GRAY2BGR);

	Mat	bgr_thresholding_converted[3];
	for (int i = 0; i < 3; ++i)
		cvtColor(bgr_thresholding[i], bgr_thresholding_converted[i], COLOR_GRAY2BGR);

	Mat	im_result;
	vector<Mat> imgs = {img_original, bgr_thresholding_converted[2], bgr_thresholding_converted[1], bgr_thresholding_converted[0], im_result_bgr_converted};
	hconcat(imgs, im_result);
	imshow("HW_02_01", im_result);
	waitKey();
	destroyAllWindows();
	return (0);
}