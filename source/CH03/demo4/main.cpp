#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


int main()
{    
    // Read input image
    cv::Mat image= cv::imread("timg.jpg");
    if (!image.data)
        return 0;

//    cv::Mat combineImage = cv::Mat(240, 640, image.type());
//    cv::Mat combineImageROI;

    // Display the image
    cv::namedWindow("Original Image");
    cv::imshow("Original Image",image);

    // define bounding rectangle
    cv::Rect rectangle(216,8,487,566);
    // the models (internally used)
    cv::Mat bgModel,fgModel;
    // segmentation result
    cv::Mat result; // segmentation (4 possible values)

    // GrabCut segmentation
    cv::grabCut(image,    // input image
                result,   // segmentation result
                rectangle,// rectangle containing foreground
                bgModel, fgModel, // models
                10,        // number of iterations
                cv::GC_INIT_WITH_RECT); // use rectangle

    // Get the pixels marked as likely foreground
    cv::compare(result,cv::GC_PR_FGD,result,cv::CMP_EQ);
    // or:
    //	result= result&1;

    // create a white image
    cv::Mat foreground(image.size(), CV_8UC3,
                       cv::Scalar(255, 255, 255));

    image.copyTo(foreground,result); // bg pixels not copied

    // draw rectangle on original image
    cv::rectangle(image, rectangle, cv::Scalar(255,255,255),1);
    cv::namedWindow("Image with rectangle");
    cv::imshow("Image with rectangle",image);
//    combineImageROI = cv::Mat(combineImage, cv::Rect(0, 0, 320, 240));
//    image.copyTo(combineImageROI);

    // display result
    cv::namedWindow("Foreground object");
    cv::imshow("Foreground object",foreground);
//    combineImageROI = cv::Mat(combineImage, cv::Rect(320, 0, 320, 240));
//    foreground.copyTo(combineImageROI);
//    //cv::imwrite("CH3-image4.jpg", combineImage);
    cv::waitKey();
    return 0;
}
