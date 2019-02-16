#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

int main()
{    
    // Read input image
    cv::Mat image= cv::imread("boldt.jpg");
    if (!image.data)
        return 0;

    // convert into HSV space
    cv::Mat hsv;
    cv::cvtColor(image, hsv, CV_BGR2HSV);

    // split the 3 channels into 3 images
    std::vector<cv::Mat> channels;
    cv::split(hsv,channels);
    // channels[0] is the Hue
    // channels[1] is the Saturation
    // channels[2] is the Value

    cv::Mat combineImage = cv::Mat(240, 960, channels[0].type());
    cv::Mat combineImageROI;

    combineImageROI = cv::Mat(combineImage, cv::Rect(0, 0, 320, 240));
    channels[0].copyTo(combineImageROI);

    combineImageROI = cv::Mat(combineImage, cv::Rect(320, 0, 320, 240));
    channels[1].copyTo(combineImageROI);

    combineImageROI = cv::Mat(combineImage, cv::Rect(640, 0, 320, 240));
    channels[2].copyTo(combineImageROI);

    // display result
    cv::namedWindow("Foreground object");
    cv::imshow("Hue object",channels[0]);
    cv::imshow("Saturation object",channels[1]);
    cv::imshow("Value object",channels[2]);
    cv::imshow("combine object",combineImage);
//    cv::imwrite("CH3-image5.jpg", combineImage);
    cv::waitKey();
    return 0;
}
