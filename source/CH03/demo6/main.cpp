#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

int main()
{
    cv::Mat hs(128, 360, CV_8UC3);
    for (int h = 0; h < 360; h++) {
        for (int s = 0; s < 128; s++) {
            hs.at<cv::Vec3b>(s, h)[0] = h/2; // all hue angles
            // from high saturation to low
            hs.at<cv::Vec3b>(s, h)[1] = 255-s*2;
            hs.at<cv::Vec3b>(s, h)[2] = 255; // constant value
        }
    }

    // display result
    cv::namedWindow("image object");
    cv::imshow("Hue object",channels[0]);

    cv::waitKey();
    return 0;
}
