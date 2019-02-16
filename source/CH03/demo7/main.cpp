#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

int main()
{    
    cv::Mat image = cv::imread("boldt.jpg");

    cv::Mat hsv;
    cv::cvtColor(image, hsv, CV_BGR2HSV);
    // split the 3 channels into 3 images
    std::vector<cv::Mat> channels;
    cv::split(hsv,channels);
    // Value channel will be 255 for all pixels
    channels[2]= 255;
    // merge back the channels
    cv::merge(channels,hsv);
    // reconvert to BGR
    cv::Mat newImage;
    cv::cvtColor(hsv,newImage,CV_HSV2BGR);

    // display result
    cv::namedWindow("image object");
    cv::imshow("image object",newImage);
//    cv::imwrite("CH3-image8.jpg", newImage);

    cv::waitKey();
    return 0;
}
