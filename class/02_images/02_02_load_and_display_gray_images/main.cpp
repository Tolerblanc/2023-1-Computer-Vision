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

    // Display the image in a window
    imshow("Lionel Messi in Grayscale", img);

    // Wait for a key to be pressed
    waitKey(0);

    // Destroy all windows
    destroyAllWindows();

    return 0;
}