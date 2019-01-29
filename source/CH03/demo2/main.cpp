#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "colordetector.hh"


int main()
{
    /* 1. Create image processor object */
    CColorDetector cdetect;
    
    /* 2. Read input image */
    cv::Mat image= cv::imread("boldt.jpg");
    
    if (image.empty()) return 0;
    
    /* 3. Set input parameters */
    cdetect.SetTargetColor(230,190,130); // here blue sky

    /* 4. Process the image and display the result */
    cv::namedWindow("result");
    cv::Mat result = cdetect.Process(image);
    cv::imshow("result",result);
    cv::waitKey();
    return 0;
}
