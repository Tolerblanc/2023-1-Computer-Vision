#include <iostream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

int main()
{
    // Load an image in a grayscale
    Mat img = imread("messi5.jpg", IMREAD_GRAYSCALE);

    // Check if image loading is successful
    if (img.empty())
    {
        cout << "Error: No image exists!" << endl;
        exit(-1);
    }

    // Image properties
    cout << "Image width: "  << img.cols << endl;
    cout << "Image height: " << img.rows << endl;
    cout << "Image depth: "  << img.depth() << endl;
    cout << "Image channles: " << img.channels() << endl;
    cout << "Image type: " << img.type() << endl;

    return 0;
}