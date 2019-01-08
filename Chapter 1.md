# How it works…
One way to define an ROI is to use a `cv::Rect` instance. 定义ROI的一种方法是使用cv :: Rect实例。As the name indicates, it describes a rectangular region by specifying the position of the  upper-left corner (the first two parameters of the constructor) and the size of the rectangle (the width and height are given in the last two parameters). 顾名思义，它描述了通过指定左上角的位置（构造器的前两个参数）和矩形的大小（最后两个参数中给出了宽度和高度）。In our example, we used the size of the image and the size of the logo in order to determine the position where the logo would cover the bottom-right corner of the image. 在我们的例子中，我们使用了图像的大小和logo大小，以确定logo覆盖图像右下角的位置。Obviously, the ROI should always be completely inside the parent image. 显然，ROI应始终完全在父图片内部。

The ROI can also be described using row and column ranges. 还可以使用行和列范围来描述ROI。A range is a continuous sequence from a start index to an end index (excluding both). 范围是连续的从起始索引到结束索引的序列（不包括两者）。The cv::Range structure is used to represent this concept.cv :: Range结构被用来代表这个概念。 Therefore, an ROI can be defined from two ranges; 因此，可以从两个范围定义ROI; in our example, the ROI could have been equivalently defined as follows:在我们的例如，ROI可以等效地定义如下：
```c++
imageROI= image(cv::Range(image.rows-logo.rows,image.rows),
                cv::Range(image.cols-logo.cols,image.cols));
```

In this case, the operator() function of cv ::Mat returns another cv::Mat instance that can then be used in subsequent calls. Any transformation of the ROI will affect the original image in the corresponding area because the image and the ROI share the same image data.Since the definition of an ROI does not include the copying of data, it is executed in a constant amount of time, no matter the size of the ROI. 

在这种情况下，`cv::Mat`的操作符`()`函数返回另一个`cv::Mat`实例，然后可以在后续调用中使用它。 ROI的任何变换都会影响相应区域中的原始图像，因为图像和ROI共享相同的图像数据。由于ROI的定义不包括数据的复制，因此无论ROI多大，它都会在恒定的时间内执行。

If you want to define an ROI made up of some lines of an image, the following call can be
used:
如果要定义由图像的某些行组成的ROI，则可以进行以下调用用过的：
```c++
cv::Mat imageROI= image.rowRange(start,end);
```

Similarly, for an ROI made up of some image columns, the following can be used:
同样，对于由图像某些列组成的ROI，可以使用以下内容：
```c++
cv::Mat imageROI= image.colRange(start,end); 
```

# There's more… 

The OpenCV methods and functions include many optional parameters that are not discussed in the recipes of this book. When you wish to use a function for the first time, you should always take the time to look at the documentation to learn more about the possible options that the function offers. One very common option is the possibility to define image masks. 

OpenCV方法和函数包括许多可选参数，这些参数在本书的内容中没有讨论。当您第一次使用某个功能时，您应该花时间查看文档以了解有关该功能可能提供的选项的更多信息。一种非常常见的选择是定义图像蒙版的可能性。

# Using image masks
Some OpenCV operations allow you to define a mask that will limit the applicability([ap·pli·ca·bil·i·ty || ‚æplɪkə'bɪlətɪ]n.  适用性; 适应性) of a given function or method, which is normally supposed to operate on all the image pixels. A mask is an 8-bit image that should be nonzero at all locations where you want an operation to be applied. At the pixel locations that correspond to the zero values of the mask, the image is untouched. For example, the copyTo method can be called with a mask. We can use it here to copy only the white portion of the logo shown previously, as follows:
一些OpenCV操作允许您定义一个掩码，该掩码将限制给定函数或方法的适用性，通常应该对所有图像像素进行操作。掩码是一个8位图像，在您希望应用操作的所有位置应该是非零的。在对应于掩模的零值的像素位置处，图像未被触摸。例如，可以使用掩码调用copyTo方法。我们可以在这里使用它来仅复制前面显示的徽标的白色部分，如下所示：
```
// define image ROI at image bottom-right
imageROI= image(cv::Rect(image.cols-logo.cols,
image.rows-logo.rows,
logo.cols,logo.rows));
// use the logo as a mask (must be gray-level)
cv::Mat mask(logo);
// insert by copying only at locations of non-zero mask
logo.copyTo(imageROI,mask);
```
The following image is obtained by executing the previous code:
![CH1-image1](D:\Workplace\opencv_learn\OpenCV3Cookbook-cn\CH1-image1.png)

The background of our logo was black (therefore, it had the value 0); this is why it was easy to use it as both the copied image and the mask. Of course, you can define the mask of your choice in your application;most  OpenCV pixel-based operations give you the opportunity to use masks. 

我们的logo背景为黑色（因此，它的值为0）;这就是为什么它很容易用作复制的图像和掩码。当然，您可以在应用程序中定义所选的掩码;大多数基于OpenCV像素的操作使您有机会使用掩码。

# See also 

- The `row` and `col` methods will be used in the *Scanning an image with neighbor access* recipe of **Chapter 2, Manipulating Pixels**. These are a special case of the `rowRange` and `colRange` methods in which the start and end indexes are equal in order to define a single-line or single-column ROI.





