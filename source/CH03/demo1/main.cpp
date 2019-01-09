#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

// remapping an image by creating wave effects
void wave(const cv::Mat &image, cv::Mat &result) {
    result.create(image.size(), image.type());
    // the map functions
    cv::Mat srcX(image.rows,image.cols,CV_32F);
    cv::Mat srcY(image.rows,image.cols,CV_32F);
    // creating the mapping
    for (int i=0; i<image.rows; i++)
    {
        for (int j=0; j<image.cols; j++) {
            // new location of pixel at (i,j)
            srcX.at<float>(i,j)= j; // remain on same column
            // pixels originally on row i are now
            // moved following a sinusoid
            srcY.at<float>(i,j)= i+5*sin(j/10.0);
        }
    }
    // applying the mapping
    cv::remap(image, // source image
              result, // destination image
              srcX, // x map
              srcY, // y map
              cv::INTER_LINEAR); // interpolation method
}

int main()
{
    cv::Mat image1, result;

    // open images
    image1= cv::imread("boldt.jpg");
    if (image1.empty())
    {
        printf("image1 is empty\n");
        return -1;
    }
    wave(image1, result);

    cv::namedWindow("Image");
    cv::imshow("Image",result);
    cv::imwrite("boldt-wave.jpg", result);

    cv::waitKey();

    return 0;
}
