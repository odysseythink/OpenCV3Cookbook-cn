#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

void sharpen(const cv::Mat &image, cv::Mat &result)
{
    // allocate if necessary
    result.create(image.size(), image.type());
    int nchannels= image.channels(); // get number of channels
    // for all rows (except first and last)
    for (int j= 1; j<image.rows-1; j++)
    {
        const uchar* previous= image.ptr<const uchar>(j-1);// previous row
        const uchar* current= image.ptr<const uchar>(j); // current row
        const uchar* next= image.ptr<const uchar>(j+1); // next row
        uchar* output= result.ptr<uchar>(j); // output row
        for (int i=nchannels; i<(image.cols-1)*nchannels; i++)
        {
            // apply sharpening operator
            *output++= cv::saturate_cast<uchar>(5*current[i]-current[i-nchannels]-current[i+nchannels]-previous[i]-next[i]);
        }
    }
    // Set the unprocessed pixels to 0
    result.row(0).setTo(cv::Scalar(0));
    result.row(result.rows-1).setTo(cv::Scalar(0));
    result.col(0).setTo(cv::Scalar(0));
    result.col(result.cols-1).setTo(cv::Scalar(0));
}

int main()
{
    // read the image
    cv::Mat image = cv::imread("boldt.jpg");
    cv::Mat result;
    // time and process the image
    const int64 start = cv::getTickCount();
    sharpen(image, result);
    //Elapsed time in seconds
    double duration = (cv::getTickCount() - start) / cv::getTickFrequency();

    // display the image
    std::cout << "sharpen function Duration= " << duration << "secs" << std::endl;
    cv::namedWindow("Image");
    cv::imshow("Image original", image);
    cv::imshow("Image sharpen", result);

    cv::waitKey();

    return 0;
}
