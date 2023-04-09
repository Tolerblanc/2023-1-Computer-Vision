/*
** 2023 1st Semester : Computer Vision
** HomeWork02-01 : HW_02_01_histogram_equalization
** written by HyunJun KIM (2019204054)
** Image histogram and Histogram equalization
*/

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void	check_type(Mat& img)
{
	cout << "Image width: "  << img.cols << endl;
    cout << "Image height: " << img.rows << endl;
    cout << "Image channles: " << img.channels() << endl;
	switch (img.depth())
    {
        case CV_8U:  cout << "CV_8U"   << endl; break;  // 0
        case CV_8S:  cout << "CV_8S"   << endl; break;  // 1
        case CV_16U: cout << "CV_16U"  << endl; break;  // 2
        case CV_16S: cout << "CV_16S"  << endl; break;  // 3
        case CV_32S: cout << "CV_32S"  << endl; break;  // 4
        case CV_32F: cout << "CV_32F"  << endl; break;  // 5
        case CV_64F: cout << "CV_64F"  << endl; break;  // 6
        case CV_16F: cout << "CV_16F"  << endl; break;  // 7
    }
	switch (img.type())
    {
        case CV_8UC1: cout << "CV_8UC1" << endl; break;
        case CV_8UC2: cout << "CV_8UC2" << endl; break;
        case CV_8UC3: cout << "CV_8UC3" << endl; break;
        case CV_8UC4: cout << "CV_8UC4" << endl; break;

        case CV_8SC1: cout << "CV_8SC1" << endl; break;
        case CV_8SC2: cout << "CV_8SC2" << endl; break;
        case CV_8SC3: cout << "CV_8SC3" << endl; break;
        case CV_8SC4: cout << "CV_8SC4" << endl; break;

        case CV_16UC1: cout << "CV_16UC1" << endl; break;
        case CV_16UC2: cout << "CV_16UC2" << endl; break;
        case CV_16UC3: cout << "CV_16UC3" << endl; break;
        case CV_16UC4: cout << "CV_16UC4" << endl; break;

        case CV_16SC1: cout << "CV_16SC1" << endl; break;
        case CV_16SC2: cout << "CV_16SC2" << endl; break;
        case CV_16SC3: cout << "CV_16SC3" << endl; break;
        case CV_16SC4: cout << "CV_16SC4" << endl; break;

        case CV_32SC1: cout << "CV_32SC1" << endl; break;
        case CV_32SC2: cout << "CV_32SC2" << endl; break;
        case CV_32SC3: cout << "CV_32SC3" << endl; break;
        case CV_32SC4: cout << "CV_32SC4" << endl; break;

        case CV_32FC1: cout << "CV_32FC1" << endl; break;
        case CV_32FC2: cout << "CV_32FC2" << endl; break;
        case CV_32FC3: cout << "CV_32FC3" << endl; break;
        case CV_32FC4: cout << "CV_32FC4" << endl; break;

        case CV_64FC1: cout << "CV_64FC1" << endl; break;
        case CV_64FC2: cout << "CV_64FC2" << endl; break;
        case CV_64FC3: cout << "CV_64FC3" << endl; break;
        case CV_64FC4: cout << "CV_64FC4" << endl; break;
        
        case CV_16FC1: cout << "CV_16FC1" << endl; break;
        case CV_16FC2: cout << "CV_16FC2" << endl; break;
        case CV_16FC3: cout << "CV_16FC3" << endl; break;
        case CV_16FC4: cout << "CV_16FC4" << endl; break;
    }
}

int	main()
{
	// Load a lena image
	Mat srcImage = imread("lena.jpg");
	// Check if image loading is successful
    if (srcImage.empty())
    {
        cout << "Error: No image exists!" << endl;
        exit(-1);
    }

	// Split RGB channels
	vector<Mat> bgrPlanes;
	split(srcImage, bgrPlanes);

	// Create the output histograms, define size and range
	vector<Mat> hists(3);
	int	histSize = 256;
    float range[] = {0, 256}; // the upper boundary is exclusive
    const float* ranges[] = {range};

	// Calculate the image histogram
    for (int c = 0; c < 3; c++)
    {
        calcHist(&bgrPlanes[c], 1, nullptr, noArray(), // do not use mask 
                 hists[c], 1, &histSize, ranges, 
                 true, // the histogram is uniform
                 false // do not accumulate
                );
    }

	// Histogram image
    int hist_w = 512, hist_h = 512;
    int bin_w = cvRound((double) hist_w/histSize);
    Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(255, 255, 255));

    // For each channel
    for (int c = 0; c < 3; c++)
    {
        // Normalize the histogram in [0, hist_h]
        normalize(hists[c], hists[c], 0, hist_h, NORM_MINMAX, -1, noArray());

        // Plot the histogram
        Scalar color(c==0?255:0, c==1?255:0, c==2?255:0);
        for(int b = 1; b < histSize; b++)
        {
            line(histImage, Point(bin_w*(b-1), hist_h - cvRound(hists[c].at<float>(b-1))),
                            Point(bin_w*(b),   hist_h - cvRound(hists[c].at<float>(b))),
                            color, 2);

        }
    }
	
	// Create the horizontal concatenated image
	Mat tempImage1;
	hconcat(srcImage, histImage, tempImage1);

	bgrPlanes.clear();
	hists.clear();
	histImage.release();
	histImage = Mat(hist_h, hist_w, CV_8UC3, Scalar(255, 255, 255));
	// Split RGB channels
	split(srcImage, bgrPlanes);

	// Equalize and Calculate the image histogram
    for (int c = 0; c < 3; c++)
    {
		equalizeHist(bgrPlanes[c], bgrPlanes[c]);
        calcHist(&bgrPlanes[c], 1, nullptr, noArray(), // do not use mask 
                 hists[c], 1, &histSize, ranges, 
                 true, // the histogram is uniform
                 false // do not accumlate
                );
    }

	float prevVal, currVal, prevAccuVal, currAccuVal, scale;
    // For each channel
    for (int c = 0; c < 3; c++)
    {
        // Normalize the histogram in [0, hist_h]
        normalize(hists[c], hists[c], 0, hist_h, NORM_MINMAX, -1, noArray());

		scale = (float)hist_h / (float)sum(hists[c])[0];
		prevVal = cvRound(hists[c].at<float>(0));
    	prevAccuVal = scale * prevVal;

        // Plot the histogram
        Scalar color(c==0?255:0, c==1?255:0, c==2?255:0);
        for(int b = 1; b < histSize; b++)
        {
			// Current iteration
            currVal = cvRound(hists[c].at<float>(b));
            currAccuVal = prevAccuVal + scale * currVal;

            line(histImage, Point(bin_w*(b-1), hist_h - prevAccuVal),
                            Point(bin_w*(b),   hist_h - currAccuVal),
                            color, 2);
			// Next iteration
            prevVal = currVal;
            prevAccuVal = currAccuVal;
        }
    }

	Mat equalizedImage;
	merge(bgrPlanes, equalizedImage);

	// Create the horizontal concatenated image
	Mat tempImage2;
	hconcat(equalizedImage, histImage, tempImage2);

	Mat	dstImage;
	vconcat(tempImage1, tempImage2, dstImage);

	imshow("HW_02_01", dstImage);
    waitKey();

    // Destroy all windows
    destroyAllWindows();
	return (0);
}