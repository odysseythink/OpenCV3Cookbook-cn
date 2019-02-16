import cv2 as cv

cv.namedWindow('Image')

image = cv.imread('timg.jpg);
logo = cv.imread('smalllogo.png');

imageROI(image, cv::Rect(image.cols - logo.cols, // ROI coordinates
image.rows - logo.rows,
logo.cols, logo.rows)); // ROI size



