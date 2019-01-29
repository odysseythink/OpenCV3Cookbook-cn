#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


int main()
{
    
    /* 2. Read input image */
    cv::Mat image= cv::imread("boldt.jpg");
    cv::Mat combineImage = cv::Mat(240, 640, image.type());
    cv::Mat combineImageROI = cv::Mat(combineImage, cv::Rect(0, 0, 320, 240));
    image.copyTo(combineImageROI);

    if (image.empty()) return 0;
    
    /* 3. Set input parameters */
    cv::floodFill(image, // input/ouput image
                  cv::Point(100, 50), // seed point
                  cv::Scalar(255, 255, 255), // repainted color
                  (cv::Rect*)0, // bounding rect of the repainted set
                  cv::Scalar(35, 35, 35), // low/high difference threshold
                  cv::Scalar(35, 35, 35), // identical most of the time
                  cv::FLOODFILL_FIXED_RANGE);// pixels compared to seed

    /* 4. Process the image and display the result */
    cv::namedWindow("result");
    cv::imshow("result",image);
    combineImageROI = cv::Mat(combineImage, cv::Rect(320, 0, 320, 240));
    image.copyTo(combineImageROI);
    cv::imshow("combine image",combineImage);
    //cv::imwrite("boldt-floodfill.jpg", image);
    cv::waitKey();
    return 0;
}
