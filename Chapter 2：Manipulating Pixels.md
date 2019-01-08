# 第二章 操作像素

In this chapter, we will cover the following recipes:
本章包含下列内容：
- Accessing pixel values访问像素值
- Scanning an image with pointers使用指针扫描图像
- Scanning an image with iterators使用迭代器扫描图像
- Writing efficient image-scanning loops编写高效的图像扫描循环
- Scanning an image with neighbor access使用邻居访问扫描图像
- Performing simple image arithmetic执行简单的图像算术
- Remapping an image 重新映射图像

## 简介
In order to build computer vision applications, you need to be able to access the image content and eventually modify or create images. This chapter will teach you how to manipulate the picture elements (also known as **pixels**). You will learn how to scan an image and process each of its pixels. You will also learn how to do this efficiently, since even images of modest dimensions can contain hundreds of thousands of pixels.

为了构建计算机视觉应用程序，您需要能够访问图像内容并最终修改或创建图像。本章将教您如何操作图像元素（也称为**像素**）。您将学习如何扫描图像并处理每个像素。您还将学习如何有效地执行此操作，因为即使是适度尺寸的图像也可包含数十万个像素。

Fundamentally, an image is a matrix of numerical values. This is why, as we learned in Chapter 1, Playing with Images, OpenCV manipulates them using the cv::Mat data structure. Each element of the matrix represents one pixel. For a gray-level image (a blackand-white image), pixels are unsigned 8-bit values (that is, of type unsigned char) where 0 corresponds to black and 255 corresponds to white.

从根本上说，图像是数值矩阵。这就是为什么，正如我们在第1章“使用图像”中所学到的，OpenCV使用cv :: Mat数据结构来操作它们。矩阵的每个元素代表一个像素。对于灰度图像（黑白图像），像素是无符号的8位值（即，无符号字符的类型），其中0对应于黑色，255对应于白色。

In the case of color images, three primary color values are required in order to reproduce the different visible colors. This is a consequence of the fact that our human visual system is trichromatic([traɪkrəˈmætɪk]三色版; 三色的; 三元色白); three types of cone([kәun]n.   圆锥体, 球果vt.   使成锥形) cells on our retinae( ['retinә]n.   视网膜) convey([kәn'vei]vt.   传达, 运输, 转让) the color information to our brain. This means that for a color image, three values must be associated to each pixel. In photography and digital imaging, the commonly used primary color channels are red,green, and blue. A matrix element is, therefore, made of a triplet(['triplit]n.   三个一组, 三件一套, 三胞胎中的一个) of 8-bit values in this case.Note that even if 8-bit channels are generally sufficient, there are specialized applications where 16-bit channels are required (medical imaging, for example).

在彩色图像的情况下，需要三个基色值以再现不同的可见颜色。这是因为我们的人类视觉系统是三色的;视网膜上有三种类型的视锥细胞将颜色信息传递给我们的大脑。这意味着对于彩色图像，必须将三个值与每个像素相关联。在摄影和数字成像中，常用的原色通道是红色，绿色和蓝色。因此，在这种情况下，矩阵元素由8位值的三元组组成。注意，即使8位通道通常足够，也需要16位通道的专用应用（例如，医学成像） 。

As we saw in the previous chapter, OpenCV also allows you to create matrices (or images) with pixel values of other types, for example, integer (CV_32U or CV_32S) and floating point (CV_32F) numbers. These are very useful to store, for example, intermediate values in some image-processing tasks. Most operations can be applied on matrices of any type; others require a specific type or work only with a given number of  channels. Therefore, a good understanding of a function's precondition is essential in order to avoid common programming errors. 

正如我们在前一章中看到的，OpenCV还允许您使用其他类型的像素值创建矩阵（或图像），例如，整数（CV_32U或CV_32S）和浮点（CV_32F）数字。这些对于存储某些图像处理任务中的中间值非常有用。大多数操作可以应用于任何类型的矩阵;其他人需要特定类型或仅使用给定数量的频道。因此，为了避免常见的编程错误，充分理解函数的前提条件是必不可少的。

Throughout this chapter, we use the following color image as the input (refer to the book's graphics PDF or to the book's website to view this image in color):

在本章中，我们使用以下彩色图像作为输入（参考书籍的图形PDF或书籍的网站以彩色方式查看此图像）：

![](D:\Workplace\opencv_learn\OpenCV3Cookbook-cn\CH2-image1.jpg)

## 访问像素值
In order to access each individual element of a matrix, you just need to specify its row and column numbers. The corresponding element, which can be a single numerical value or a vector of values in the case of a multi-channel image, will be returned. 

为了访问矩阵的每个单独元素，您只需指定其行号和列号。将返回相应的元素，其可以是单个数值或在多通道图像的情况下的值向量。

### Getting ready
To illustrate([il·lus·trate || 'ɪləstreɪt]v.  举例说明, 阐明, 作图解;  举例) the direct access to pixel values, we will create a simple function that adds **salt-and-pepper** noise to an image. As the name suggests, salt-and-pepper noise is a particular type of noise in which some randomly selected pixels are replaced by a white or a black pixel. This type of noise can occur in faulty communications when the value of some pixels is lost during the transmission. In our case, we will simply randomly select a few pixels and assign them a white color.

为了说明对像素值的直接访问，我们将创建一个简单的函数，为图像添加**salt-and-pepper**噪声。顾名思义，salt-and-pepper噪音是一种特别的噪声，一些随机选择的像素被白色或黑色替换。当某些像素的值时，这种类型的噪声可能发生在错误的通信中在传输过程中丢失了。在我们的例子中，我们将简单地随机选择一些像素和为它们指定白色。

### How to do it…
We create a function that receives an input image. This is the image that will be modified by our function. The second parameter is the number of pixels on which we want to overwrite white values: 

```c++
void salt(cv::Mat image, int n) {

    // C++11 random number generator
    std::default_random_engine generator;
    std::uniform_int_distribution<int> randomRow(0, image.rows - 1);
    std::uniform_int_distribution<int> randomCol(0, image.cols - 1);

    int i,j;
    for (int k=0; k<n; k++) {

        // random image coordinate
        i= randomCol(generator);
        j= randomRow(generator);

        if (image.type() == CV_8UC1) { // gray-level image

            // single-channel 8-bit image
            image.at<uchar>(j,i)= 255;

        } else if (image.type() == CV_8UC3) { // color image

            // 3-channel image
            image.at<cv::Vec3b>(j,i)[0]= 255;
            image.at<cv::Vec3b>(j,i)[1]= 255;
            image.at<cv::Vec3b>(j,i)[2]= 255;

            // or simply:
            // image.at<cv::Vec3b>(j, i) = cv::Vec3b(255, 255, 255);
        }
    }
}
```



The preceding function is made of a single loop that assigns n times the value 255 to randomly selected pixels. Here, the pixel column i and row j are selected using a random number generator. Note that using the type method, we distinguish the two cases of graylevel and color images. In the case of a gray-level image, the number 255 is assigned to the single 8-bit value. For a color image, you need to assign 255 to the three primary color channels in order to obtain a white pixel. 

You can call this function by passing it an image you have previously opened. Refer to the following code: 

```c++
// open the image
cv::Mat image= cv::imread("boldt.jpg",1);
// call function to add noise
salt(image,3000);
// display result
cv::namedWindow("Image");
cv::imshow("Image",image);
```

The resulting image will look as follows: 

![](D:\Workplace\opencv_learn\OpenCV3Cookbook-cn\CH2-image2.bmp)

### How it works…
The `cv::Mat` class includes several methods to access the different attributes of an image.The public member variables, cols and rows, give you the number of columns and rows in the image. For element access, `cv::Mat` has the `at(int y，int x)` method, in which x is the column number and y is the row number.However, the type returned by a method must be known at compile time, and since `cv::Mat` can hold elements of any type, the programmer needs to specify the return type that is expected. This is why the `at` method has been implemented as a template method. So, when you call it, you must specify the image element type as follows: 

`cv::Mat`类包括几种访问图像的不同属性的方法。公共成员变量，cols和rows，为您提供图像中的列数和行数。对于元素访问，`cv::Mat`具有`at(int y，int x)`方法，其中x是列号，y是行号。但是，方法返回的类型必须在编译时才知道，并且由于`cv::Mat`可以包含任何类型的元素，程序员需要指定预期的返回类型。这就是为什么at方法已经作为模板方法实现的原因。因此，当您调用它时，必须指定图像元素类型，如下所示：

```c++
image.at<uchar>(j,i)= 255;
```

It is important to note that it is the programmer's responsibility to make sure that the type specified matches the type contained in the matrix. The at method does not perform any type conversion. 

重要的是要注意，程序员有责任确保类型指定匹配矩阵中包含的类型。 at方法不执行任何操作类型转换。

In color images, each pixel is associated with three components: the red, green, and blue channels.Therefore, a `cv::Mat` class that contains a color image will return a vector of three 8-bit values.OpenCV has defined a type for such short vectors, and it is called cv::Vec3b. This is a vector of three unsigned characters. This explains why the element access to the pixels of a color pixel is written as follows: 

在彩色图像中，每个像素与三个组件相关联：红色，绿色和蓝色通道。因此，包含彩色图像的`cv::Mat`类将返回三个8位值的向量.OpenCV定义了一个类型对于这样的短向量，它被称为`cv::Vec3b`。这是三个无符号字符的向量。这解释了为什么元素访问彩色像素的像素写如下：

```c++
image.at<cv::Vec3b>(j,i)[channel]= value;
```

The `channel` index designates one of the three color channels. OpenCV stores the `channel` values in the order blue, green, and red (blue is, therefore, channel 0). You can also use the short vector data structure directly and write: 

通道索引指定三个颜色通道中的一个。OpenCV按蓝色，绿色和红色的顺序存储通道值（因此蓝色为通道0）。您也可以直接使用短矢量数据结构并编写：

```c++
image.at<cv::Vec3b>(j, i) = cv::Vec3b(255, 255, 255);
```

Similar vector types also exist for 2-element and 4-element vectors (`cv::Vec2b` and `cv::Vec4b`) as well as for other element types. For example, for a 2-element float vector, the last letter of the type name would be replaced by an `f`, that is, `cv::Vec2f`. In the case of a short integer, the last letter is replaced with `s`. This letter is an i for an integer, and a d for a double precision floating point vector. All of these types are defined using the cv::Vec<T,N> template class, where T is the type and N is the number of vector elements. 

类似的矢量类型也存在于2元素和4元素向量（`cv::Vec2b`和`cv::Vec4b`）以及其他元素类型。例如，对于2元素浮点向量，类型名称的最后一个字母将替换为`f`，即cv :: Vec2f。在短整数的情况下，最后一个字母用`s`替换。这个字母是一个整数的`i`，一个双精度浮点矢量的`d`。所有这些类型都是使用`cv::Vec<T，N>`模板类定义的，其中T是类型，N是向量元素的数量。

As a last note, you might have been surprised by the fact that our image-modifying function uses a pass-by-value image parameter. This works because when images are copied, they still share the same image data.So, you do not necessarily have to transmit images by references when you want to modify their content. Incidentally, pass-by-value parameters often make code optimization easier for the compiler. 

作为最后一点，您可能会对我们的图像修改功能使用按值传递图像参数这一事实感到惊讶。这是有效的，因为在复制图像时，它们仍然共享相同的图像数据。因此，当您要修改其内容时，不一定必须通过引用传输图像。顺便提一下，按值传递参数通常会使编译器更容易进行代码优化。

### There's more…
The cv::Mat class has been made generic by defining it using C++ templates. 

### The cv::Mat_ template class
Using the `at` method of the cv::Mat class can sometimes be cumbersome because the
returned type must be specified as a template argument in each call. In cases where the
matrix type is known, it is possible to use the `cv::Mat_` class, which is a template subclass
of `cv::Mat`. This class defines a few extra methods but no new data attributes so that
pointers or references to one class can be directly converted to another class. Among the
extra methods, there is operator(), which allows direct access to matrix elements.
Therefore, if image is a cv::Mat variable that corresponds to a uchar matrix, then you can
write the following code:

```
// use image with a Mat_ template
cv::Mat_<uchar> img(image);_
img(50,100)= 0; // access to row 50 and column 100
```
Since the type of the `cv::Mat_` elements is declared when the variable is created, the
`operator()` method knows at compile time which type is to be returned. Other than the
fact that it is shorter to write, using the `operator()` method provides exactly the same
result as the at method. 

### See also
- The There's more… section of the Scanning an image with pointers recipe explains how to create a function with input and output parameters
- The Writing efficient image-scanning loops recipe proposes a discussion on the efficiency of the at method

## 使用指针扫描图像
In most image-processing tasks, you need to scan all pixels of the image in order to perform a computation. Considering the large number of pixels that will need to be visited, it is essential that you perform this task in an efficient way. This recipe, and the next one, will show you different ways of implementing efficient scanning loops. This recipe uses the pointer arithmetic([a·rith·me·tic || ə'rɪθmətɪk]n.  计算, 算术; 打算, 估计;  算术知识; 算术论著, 算术课本adj.  算术的;  根据算术的). 

在大多数图像处理任务中，您需要扫描图像的所有像素才能执行计算。考虑到需要访问的大量像素，必须以有效的方式执行此任务。这节和下一节将向您展示不同的高效扫描循环的方法。这节使用指针算法。

### Getting ready
We will illustrate the image-scanning process by accomplishing a simple task: reducing the number of colors in an image. 

我们将通过完成一项简单的任务来说明图像扫描过程：减少图像中的颜色数量。

Color images are composed of 3-channel pixels. Each of these channels corresponds to the intensity value of one of the three primary colors, red, green, and blue. Since each of these values is an 8-bit unsigned character, the total number of colors is 256x256x256, which is more than 16 million colors. Consequently, to reduce the complexity of an analysis, it is sometimes useful to reduce the number of colors in an image. One way to achieve this goal is to simply subdivide([,sub·di'vide || ‚sʌbdɪ'vaɪd]v.  把...再分, 把...细分;  把分成几块出售; 再分, 细分) the RGB space into cubes of equal sizes. For example, if you reduce the number of colors in each dimension by 8, then you would obtain a total of 32x32x32 colors. Each color in the original image is then assigned a new color value in the colorreduced image that corresponds to the value in the center of the cube to which it belongs. 

彩色图像由3通道像素组成。这些通道中的每一个对应于三原色之一（红色，绿色和蓝色）的强度值。由于这些值中的每一个都是8位无符号字符，因此颜色总数为256x256x256，超过1600万种颜色。因此，为了降低分析的复杂性，有时减少图像中的颜色数量是有用的。实现此目标的一种方法是简单地将RGB空间细分为相同大小的立方体。例如，如果将每个维度中的颜色数减少8，那么您将获得总共32x32x32颜色。然后在原始图像中的每种颜色在颜色减少的图像中分配一个新的颜色值，该颜色值对应于它所属的立方体中心的值。

Therefore, the basic color reduction([re·duc·tion || rɪ'dʌkʃn]n.  减少; 缩小; 削减; 下降,  降低) algorithm is simple. If N is the reduction factor, divide by N the value of each pixel (integer division is assumed here, therefore, the reminder is lost) then multiply the result by N. This will give you the multiple of N just below the input pixel value. Add N/2 and you obtain the central position of the interval between two adjacent multiples of N. If you repeat this process for each 8-bit channel value, then you will obtain a total of 256/N x 256/N x 256/N possible color values. 

因此，基本的色彩缩减算法很简单。如果N是缩减因子，则每个像素的值（这里假设整数除法，因此，提醒丢失）除以N，然后将结果乘以N.这将给出刚好低于输入像素值的N的倍数。加上N / 2并获得两个相邻N的倍数之间的中心位置。如果对每个8位通道值重复此过程，则将获得总计256 / N x 256 / N x 256 / N个可能的颜色值。

### How to do it…
The signature( ['signәtʃә]n.   签字, 识别标志, 调号) of our color reduction function will be as follows: 

我们的色彩还原功能的函数名如下：

```c++
void colorReduce(cv::Mat image, int div=64);
```

The user provides an image and the per-channel reduction factor. Here, the processing is done in-place, that is, the pixel values of the input image are modified by the function. See the There's more… section of this recipe for a more general function signature with input and output arguments. 

用户提供图像和每通道缩减因子。这里，处理是就地完成的，即，通过该功能修改输入图像的像素值。有关具有输入和输出参数的更通用的函数签名，请参阅“还有更多...”部分内容。

The processing is simply done by creating a double loop that goes over all pixel values as follows: 

通过创建一个遍历所有像素值的双层循环来完成处理，如下所示：

```c++
void colorReduce(cv::Mat image, int div=64) {
    int nl= image.rows; // number of lines
    // total number of elements per line
    int nc= image.cols * image.channels();
    for (int j=0; j<nl; j++) {
        // get the address of row j
        uchar* data= image.ptr<uchar>(j);
        for (int i=0; i<nc; i++) {
            // process each pixel ---------------------
            data[i]= data[i]/div*div + div/2;
            // end of pixel processing ----------------
        } // end of line
    }
}
```

This function can be tested using the following code snippet( ['snipit]n.   小片, 片断, 摘录):

可以使用以下代码段测试此函数：

```c++
// read the image
image= cv::imread("boldt.jpg");
// process the image
colorReduce(image,64);
// display the image
cv::namedWindow("Image");
cv::imshow("Image",image); 
```

This will give you, for example, the following image: 

![](D:\Workplace\opencv_learn\OpenCV3Cookbook-cn\CH2-image3.jpg)

### How it works…
In a color image, the first three bytes of the image data buffer are the 3 channel values of the upper-left pixel, the next three bytes are the values of the second pixel of the first row, and so on (remember that OpenCV uses, by default, the BGR channel order). An image of width W and height H would then require a memory block of $W*H*3$ uchars. However, for efficiency reasons, the length of a row can be padded with a few extra pixels. This is because image processing can sometimes be made more efficient when rows are multiples of 8 for example; this way they better align with the local memory configuration. Obviously, these extra pixels are not displayed or saved; their exact values are ignored. OpenCV designates([des·ig·nate || 'dezɪgneɪt]v.  指明; 任命; 指出adj.  指定的, 选定的) the length of a padded row as the effective width. Obviously, if the image has not been padded with extra pixels, the effective width will be equal to the real image width. We have already learned that the cols and rows attributes give you the image's width and height;similarly, the step data attribute gives you the effective width in number of bytes. Even if your image is of a type other than uchar, the step data will still give you the number of bytes in a row. The size of a pixel element is given by the elemSize method (for example,for a  3-channel short integer matrix (CV_16SC3), elemSize will return 6). Recall that the number of channels in the image is given by the nchannels method (which will be 1 for a gray-level image and 3 for a color image). Finally, the total method returns the total number of pixels (that is, the matrix entries) in the matrix. 

在彩色图像中，图像数据缓冲区的前三个字节是左上角像素的3个通道值，接下来的三个字节是第一行的第二个像素的值，依此类推（请记住，OpenCV使用，默认情况下，BGR通道顺序）。然后，宽度W和高度H的图像将需要$W*H*3$ uchars的存储空间。但是，出于效率原因，可以用一些额外的像素填充行的长度。这是因为当行是8的倍数时，有时可以使图像处理更有效;这样他们就更好地与本地内存配置保持一致。显然，这些不显示或保存额外的像素;他们的确切值被忽略了。 OpenCV将填充行的长度指定为有效宽度。显然，如果图像没有用额外像素填充，则有效宽度将等于实际图像宽度。我们已经了解到cols和rows属性为您提供了图像的宽度和高度;类似地，step data属性为您提供了有效的字节数宽度。即使您的图像的类型不是uchar，step data仍然会为您提供行中的字节数。像素元素的大小由elemSize方法给出（例如，对于3通道短整数矩阵（CV_16SC3），elemSize将返回6）。回想一下，图像中的通道数由nchannels方法给出（对于灰度图像为1，对于彩色图像为3）。最后，total方法返回矩阵中的像素总数（即矩阵条目）。

The number of pixel values per row is then given by the following code:
```
int nc= image.cols * image.channels();
```
To simplify the computation of the pointer arithmetic, the `cv::Mat` class offers a method that directly gives you the starting address of an image row. This is the ptr method. It is a template method that returns the address of row number j:

为了简化指针算术的计算，`cv::Mat`类提供了一种直接为您提供图像行起始地址的方法。这是ptr方法。它是一个模板方法，返回行号j的地址：
```
uchar* data= image.ptr<uchar>(j);
```
Note that in the processing statement, we could have equivalently used the pointer arithmetic to move from column to column. So, we could have written the following code:

请注意，在处理语句中，我们可以等效地使用指针算法从列移动到列。所以，我们可以编写以下代码：
```
*data++= *data/div*div + div/2;
```
### There's more…
The color reduction function presented in this recipe provides just one way of accomplishing this task. You could also use other color reduction formulas. A more general version of the function would also allow the specification of distinct input and output images. The image scanning can also be made more efficient by taking into account the continuity of the image data. Finally, it is also possible to use regular low-level pointer arithmetic to scan the image buffer. All of these elements are discussed in the following subsections.

本节中提供的减色功能仅提供了完成此任务的一种方法。您还可以使用其他颜色缩减公式。该函数的更通用版本还允许指定不同的输入和输出图像。通过考虑图像数据的连续性，还可以使图像扫描更有效。最后，还可以使用常规的低级指针算法来扫描图像缓冲区。所有这些元素将在以下小节中讨论。

### Other color reduction formulas
In our example, color reduction is achieved by taking advantage of integer division that floors the division result to the nearest lower integer as follows:

在我们的示例中，通过利用将除法结果置于最接近的较低整数的整数除法来实现颜色缩减，如下所示：

```c++
data[i]= (data[i]/div)*div + div/2;
```
The reduced color could have also been computed from the modulo([ˈmɔdjuləu]模; 按模计算) operator using which we can obtain the multiple of div immediately below as follows:

还可以使用模运算符来计算减少的颜色我们可以在下面获得div的倍数如下：

```c++
data[i]= data[i] - data[i]%div + div/2;
```

Another option would be to use bitwise([计] 按位) operators. Indeed, if we restrict the reduction factor to a power of 2, that is, `div=pow(2,n)`, then masking the first n bits of the pixel value would give us the nearest lower multiple of div. This mask would be computed by a simple bit shift as follows:

另一种选择是使用按位运算符。实际上，如果我们将缩减因子限制为2的幂，即`div = pow（2，n）`，那么屏蔽像素值的前n位将给出最近的div的较低倍数。该掩码将通过简单的位移计算，如下所示：

```c++
// mask used to round the pixel value
uchar mask= 0xFF<<n; // e.g. for div=16, mask= 0xF0 
```

The color reduction would be given by the following code:

```c++
*data &= mask; // masking
*data++ += div>>1; // add div/2;
// bitwise OR could also be used above instead of + 
```

In general, bitwise operations might lead to very efficient code, so they could constitute([con·sti·tute || 'kɒnstɪtjuːt]v.  制定; 组成;  建立) a powerful alternative when efficiency is a requirement. 

通常，按位运算可能会导致非常高效的代码，因此当需要效率时，它们可以构成强大的替代方案。

### Having input and output arguments
In our color reduction example, the transformation is directly applied to the input image,which is called an in-place transformation. This way, no extra image is required to hold the output result, which could save on the memory usage when this is a concern. However, in some applications, the user might want to keep the original image intact. The user would then be forced to create a copy of the image before calling the function. Note that the easiest way to create an identical deep copy of an image is to call the clone() method; for example, take a look at the following code: 

在我们的颜色缩减示例中，转换直接应用于输入图像，称为就地转换。这样，不需要额外的图像来保存输出结果，这可以节省内存使用时间。但是，在某些应用程序中，用户可能希望保持原始图像的完整性。然后，在调用函数之前，用户将被迫创建图像的副本。请注意，创建图像的相同深层副本的最简单方法是调用clone（）方法;例如，看看下面的代码：

### Efficient scanning of continuous images 



## 使用迭代器扫描图像

In object-oriented programming, looping over a data collection is usually done using iterators. Iterators are specialized classes that are built to go over each element of a collection, hiding how the iteration over each element is specifically done for a given collection. This application of the information-hiding principle makes scanning a collection easier and safer. In addition, it makes it similar in form no matter what type of collection is used. The Standard Template Library (STL) has an iterator class associated with each of its
collection classes. OpenCV then offers a cv::Mat iterator class that is compatible with the standard iterators found in the C++ STL. 

在面向对象的编程中，通常使用迭代器来完成数据集合的遍历。迭代器是专门的类，它们构建用于遍历集合的每个元素，隐藏如何针对给定集合专门完成对每个元素的迭代。信息隐藏原理的这种应用使扫描集合更容易和更安全。此外，无论使用何种类型的集合，它都使其形式相似。标准模板库（STL）具有与其每个相关联的迭代器类
集合类。然后，OpenCV提供了一个`cv::Mat`迭代器类，它与C ++ STL中的标准迭代器兼容。

### Getting ready
In this recipe, we again use the color reduction example described in the previous recipe. 

### How to do it…
An iterator object for a cv::Mat instance can be obtained by first creating a `cv::MatIterator_` object. As is the case with `cv::Mat_`, the underscore indicates that this is a template subclass. Indeed, since image iterators are used to access the image elements,the return type must be known at the time of compilation.The iterator for a color image is then declared as follows: 

```c++
cv::MatIterator_<cv::Vec3b> it;
```

Alternatively, you can also use the iterator type defined inside the Mat_ template class as follows:

```c++
cv::Mat_<cv::Vec3b>::iterator it; 
```

You then loop over the pixels using the usual begin and end iterator methods, except that these ones are, again, template methods. Consequently, our color reduction function is now written as follows: 

```c++
void colorReduce(cv::Mat image, int div=64) {
    // div must be a power of 2
    int n= staic_cast<int>(
        log(static_cast<double>(div))/log(2.0) + 0.5);
    // mask used to round the pixel value
    uchar mask= 0xFF<<n; // e.g. for div=16, mask= 0xF0
    uchar div2 = div >> 1; // div2 = div/2
    // get iterators
    cv::Mat_<cv::Vec3b>::iterator it= image.begin<cv::Vec3b>();
    cv::Mat_<cv::Vec3b>::iterator itend= image.end<cv::Vec3b>();
    // scan all pixels
    for ( ; it!= itend; ++it) {
        (*it)[0]&= mask;
        (*it)[0]+= div2;
        (*it)[1]&= mask;
        (*it)[1]+= div2;
        (*it)[2]&= mask;
        (*it)[2]+= div2;
    }
}
```

Remember that the iterator here returns a `cv::Vec3b` instance because we are processing a color image.Each color channel element is accessed using the dereferencing operator [].Note that you could also rely on the `cv::Vec3b` overloaded operators and simply write: 

```c++
*it= *it/div*div+offset;
```

## 编写高效的图像扫描循环

In the previous recipes of this chapter, we presented different ways of scanning an image in order to process its pixels. In this recipe, we will compare the efficiency of these different approaches.

When you write an image-processing function, efficiency is often a concern. When you design your function, you will frequently need to check the computational efficiency of your code in order to detect any bottleneck in your processing that might slow down your program. 

编写图像处理功能时，效率通常是一个问题。在设计函数时，您经常需要检查代码的计算效率，以便检测处理过程中可能会降低程序速度的任何瓶颈。

However, it is important to note that unless necessary, optimization should not be done at the price of reducing code clarity. Simple code is indeed, always easier to debug and maintain. Only code portions that are critical to a program's efficiency should be heavily optimized. 

但是，重要的是要注意，除非必要，否则不应以降低代码清晰度为代价进行优化。确实，简单的代码总是更容易调试和维护。只应严格优化对程序效率至关重要的代码部分。



## Scanning an image with neighbor access

In image processing, it is common to have a processing function that computes a value at each pixel location based on the value of the neighboring pixels. When this neighborhood includes pixels of the previous and next lines, you then need to simultaneously scan several lines of the image. This recipe shows you how to do it. 

在图像处理中，通常具有处理功能，该处理功能基于相邻像素的值来计算每个像素位置处的值。当此邻域包含前一行和下一行的像素时，您需要同时扫描图像的多行。这节告诉你如何做到这一点。

### Getting ready
To illustrate this recipe, we will apply a processing function that sharpens an image. It is based on the Laplacian operator (which will be discussed in Chapter 6, Filtering the Images).It is indeed, a well-known result in image processing that if you subtract the Laplacian from an image, the image edges are amplified, thereby giving a sharper image.

为了说明这个技术，我们将应用锐化图像的处理功能。它基于拉普拉斯算子（将在第6章“滤波图像”中讨论）。确实，图像处理中众所周知的结果是，如果从图像中减去拉普拉斯算子，图像边缘会被放大，从而给出更清晰的图像。

This sharpened value is computed as follows: 
此锐化值计算如下：
$$
\text{sharpened_pixel}= 5*current-left-right-up-down;
$$
Here, left is the pixel that is immediately on the left-hand side of the current one, up is the corresponding one on the previous line, and so on.

这里，left是紧邻当前行的左侧的像素，up是前一行的相应像素，依此类推。

### How to do it…
This time, the processing cannot be accomplished in-place. Users need to provide an output image. The image scanning is done using three pointers, one for the current line, one for the line above, and another one for the line below. Also, since each pixel computation requires access to the neighbors, it is not possible to compute a value for the pixels of the first and last row of the image as well as the pixels of the first and last column. The loop can then be written as follows:
这次，处理不能就地完成。用户需要提供输出图像。图像扫描使用三个指针完成，一个用于当前行，一个用于上面的行，另一个用于下面的行。而且，由于每个像素计算需要访问邻居，因此不可能计算图像的第一行和最后一行的像素以及第一列和最后一列的像素的值。然后可以按如下方式编写循环：
```c++
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
```

Note how we wrote the function such that it would work on both gray-level and color images. If we apply this function on a gray-level version of our test image, the following result is obtained: 

请注意我们如何编写函数，使其适用于灰度图像和彩色图像。如果我们在测试图像的灰度版本上应用此函数，则会得到以下结果（下面的图片左边的是锐化过的，右边的是原始图像）：
![](D:\Workplace\opencv_learn\OpenCV3Cookbook-cn\CH2-image4.jpg)![CH2-image5](D:\Workplace\opencv_learn\OpenCV3Cookbook-cn\CH2-image5.jpg)



### How it works…
In order to access the neighboring pixels of the previous and next row, you must simply define additional pointers that are jointly incremented. You then access the pixels of these lines inside the scanning loop.

要访问上一行和下一行的相邻像素，必须简单地定义共同递增的其他指针。然后，您可以在扫描循环内访问这些行的像素。

In the computation of the output pixel value, the cv::saturate_cast template function is called on the result of the operation. This is because, often, a mathematical expression applied on pixels leads to a result that goes outside the range of the permitted pixel values (that is, below 0 or over 255). The solution is then to bring the values back inside this 8-bit range. This is done by changing negative values to 0 and values over 255 to 255. This is exactly what the `cv::saturate_cast<uchar>` function is doing. In addition, if the input argument is a floating point number, then the result is rounded to the nearest integer. You can obviously use this function with other types in order to guarantee that the result will remain within the limits defined by this type. 

在计算输出像素值时，在操作结果上调用`cv::saturate_cast`模板函数。这是因为，通常，应用于像素的数学表达式导致超出允许像素值范围的结果（即，低于0或高于255）。然后解决方案是将值恢复到这个8位范围内。这是通过将负值更改为0并将值更改为255到255来完成的。这正是`cv::saturate_cast<uchar>`函数所做的。另外，如果输入参数是一个浮点数，然后将结果四舍五入到最接近的整数。显然，您可以将此函数与其他类型一起使用，以确保结果保持在此类型定义的限制范围内。

Border pixels that cannot be processed because their neighborhood is not completely defined need to be handled separately. Here, we simply set them to 0. In other cases, it could be possible to perform a special computation for these pixels, but most of the time,there is no point in spending time to process these very few pixels. In our function, these border pixels are set to 0 using two special methods, row and col. They return a special cv::Mat instance composed of a single-line ROI (or a single-column ROI) as specified in a parameter (remember, we discussed region of interest in the previous chapter). No copy is made here; therefore if the elements of this 1D matrix are modified, they will also be modified in the original image. This is what we do when the setTo method is called. This method assigns a value to all elements of a matrix, as follows:

由于邻域未完全定义而无法处理的边框像素需要单独处理。在这里，我们只需将它们设置为0.在其他情况下，可以对这些像素执行特殊计算，但大多数情况下，花费时间处理这些极少数像素是没有意义的。在我们的函数中，使用两个特殊方法row和col将这些边框像素设置为0。它们返回一个特殊的`cv::Mat`实例，该实例由参数中指定的单行ROI（或单列ROI）组成（请记住，我们在前一章中讨论了感兴趣的区域）。这里没有副本;因此，如果修改了这个1D矩阵的元素，它们也将在原始图像中被修改。这就是我们在调用setTo方法时所做的事情。此方法为矩阵的所有元素分配值，如下所示：
```
result.row(0).setTo(cv::Scalar(0));
```
The preceding statement assigns the value of 0 to all pixels of the first line of the result image. In the case of a 3-channel color image, you would use cv::Scalar(a,b,c) to specify the three values to be assigned to each channel of the pixel.

前面的语句将值0赋给结果图像第一行的所有像素。对于3通道彩色图像，您可以使用`cv::Scalar(a，b，c)`指定要分配给像素的每个通道的三个值。

### There's more…
When a computation is done over a pixel neighborhood, it is common to represent this with a kernel matrix. This kernel describes how the pixels involved in the computation are combined in order to obtain the desired result. For the sharpening filter used in this recipe,the kernel would be as follows:

当在像素邻域上进行计算时，通常用核矩阵表示这一点。该内核描述了如何组合计算中涉及的像素以获得期望的结果。对于本节中使用的锐化过滤器，内核如下：

![](D:\Workplace\opencv_learn\OpenCV3Cookbook-cn\CH2-image6.jpg)

Unless stated otherwise, the current pixel corresponds to the center of the kernel. The value in each cell of the kernel represents a factor that multiplies the corresponding pixel. The result of the application of the kernel on a pixel is then given by the sum of all these multiplications. The size of the kernel corresponds to the size of the neighborhood (here,3x3). 

除非另有说明，否则当前像素对应于内核的中心。内核的每个单元格中的值表示将相应像素相乘的因子。然后，通过所有这些乘法的总和给出在像素上应用内核的结果。内核的大小对应于邻域的大小（这里是3x3）。

Using this representation, it can be seen that, as required by the sharpening filter, the four horizontal and vertical neighbors of the current pixel are multiplied by -1, while the current pixel is multiplied by 5.Applying  a kernel to an image is more than a convenient representation; it is the basis for the concept of convolution in signal processing. The kernel defines a filter that is applied to the image. 

使用此表示，可以看出，根据锐化滤波器的要求，当前像素的四个水平和垂直邻居乘以-1，而当前像素乘以5.将内核应用于图像更多而不是方便的表示;它是信号处理中卷积概念的基础。内核定义了一个应用于图像的过滤器。

Since filtering is a common operation in image processing, OpenCV has defined a special function that performs this task: the cv::filter2D function. To use this, you just need to define a kernel (in the form of a matrix). The function is then called with the image and the kernel, and it returns the filtered image. Using this function, it is therefore easy to redefine our sharpening function as follows:

由于过滤是图像处理中的常见操作，OpenCV定义了一个执行此任务的特殊函数：`cv::filter2D`函数。要使用它，您只需要定义一个内核（以矩阵的形式）。然后使用图像和内核调用该函数，并返回过滤后的图像。使用此功能，可以很容易地重新定义我们的锐化功能，如下所示：
```
void sharpen2D(const cv::Mat &image, cv::Mat &result)
{
    /* Construct kernel (all entries initialized to 0) */
    cv::Mat kernel(3, 3, CV_32F, cv::Scalar(0));
    kernel.at<float>(0, 1) = -1.0;
    kernel.at<float>(1, 0) = -1.0;
    kernel.at<float>(1, 2) = -1.0;
    kernel.at<float>(2, 1) = -1.0;
    kernel.at<float>(1, 1) = 5.0;

    /* filter the image */
    cv::filter2D(image, result, image.depth(), kernel);
}
```
This implementation produces exactly the same result as the previous one (and with the same efficiency). If you input a color image, then the same kernel will be applied to all three channels. Note that it is particularly advantageous to use the filter2D function with a large kernel, as it uses, in this case, a more efficient algorithm.

该实现产生与前一个完全相同的结果（并且具有相同的效率）。如果输入彩色图像，则相同的内核将应用于所有三个通道。注意，将filter2D函数与大内核一起使用是特别有利的，因为在这种情况下，它使用更有效的算法。

### See also
- Chapter 6, Filtering the Images, provides more explanations on the concept of image filtering

## 执行简单的图像算术
Images can be combined in different ways. Since they are regular matrices, they can be added, subtracted,multiplied, or divided. OpenCV offers various image arithmetic operators, and their use is discussed in this recipe. 

图像可以以不同方式组合。由于它们是常规矩阵，因此可以对它们进行加，减，乘或除。 OpenCV提供各种图像算术运算符，在本文中讨论了它们的用法。

### Getting ready
Let's work with a second image that we will combine with our input image using an arithmetic operator. The following represents this second image: 

让我们使用第二个图像，我们将使用算术运算符将其与输入图像结合起来。以下代表第二张图片：

![](D:\Workplace\opencv_learn\OpenCV3Cookbook-cn\CH2-image7.jpg)

### How to do it…
Here, we add two images. This is useful when we want to create some special effects or to overlay information over an image. We do this by calling the cv::add function, or more precisely here, the cv::addWeighted function, since we want a weighted sum as follows: 

在这里，我们添加两个图像。当我们想要创建一些特殊效果或在图像上叠加信息时，这非常有用。我们这样做是通过调用`cv::add`函数，或者更确切地说是`cv::addWeighted`函数，因为我们想要一个加权和，如下所示：

```
cv::addWeighted(image1,0.7,image2,0.9,0.,result);
```
The operation results in a new image:

![](D:\Workplace\opencv_learn\OpenCV3Cookbook-cn\CH2-image8.jpg)

### How it works…
All binary arithmetic functions work the same way. Two inputs are provided and a third parameter specifies the output. In some cases, weights that are used as scalar multipliers in the operation can be specified.Each  of these functions comes in several flavors; cv::add is a good example of a function that is available in many forms: 

所有二进制算术函数的工作方式相同。提供两个输入，第三个参数指定输出。在某些情况下，可以指定在操作中用作标量乘数的权重。这些函数中的每一个都有几种形式; `cv::add`是一个很好的以多种形式提供的函数示例：

```c++
// c[i]= a[i]+b[i];
cv::add(imageA,imageB,resultC);
// c[i]= a[i]+k;
cv::add(imageA,cv::Scalar(k),resultC);
// c[i]= k1*a[i]+k2*b[i]+k3;
cv::addWeighted(imageA,k1,imageB,k2,k3,resultC);
// c[i]= k*a[i]+b[i];
cv::scaleAdd(imageA,k,imageB,resultC);
```

For some functions, you can also specify a mask: 

```c++
// if (mask[i]) c[i]= a[i]+b[i];
cv::add(imageA,imageB,resultC,mask);
```

If you apply a mask, the operation is performed only on pixels for which the mask value is not null (the mask must be 1-channel). Have a look at the different forms of `cv::subtract`,`cv::absdiff`, `cv::multiply`, and `cv::divide` functions. Bitwise operators (operators applied to each individual bit of the pixels' binary representation) are also available:`cv::bitwise_and`, `cv::bitwise_or`, `cv::bitwise_xor`, and `cv::bitwise_not`. The `cv::min` and `cv::max` operators, which find the per-element maximum or minimum pixel value, are also very useful. 

如果应用掩码，则仅对掩码值不为空的像素执行操作（掩码必须为1通道）。看看`cv::subtract`，`cv::absdiff`，`cv::multiply`和`cv::divide`函数的不同形式。按位运算符（运算符应用于像素的二进制表示的每个位）也可用：`cv::bitwise_and`，`cv::bitwise_or`，`cv::bitwise_xor`和`cv::bitwise_not`。找到每个元素的最大或最小像素值的`cv::min`和`cv::max`运算符也非常有用。

In all cases, the `cv::saturate_cast` function (see the preceding recipe) is always used to make sure that the results stay within the defined pixel value domain (that is, to avoid overflow or underflow).

在所有情况下，始终使用`cv::saturate_cast`函数（参见前面的内容）来确保结果保持在定义的像素值域内（即，避免上溢或下溢）。

The images must have the same size and type (the output image will be reallocated if it does not match the input size). Also, since the operation is performed per-element, one of the input images can be used as the output.

图像必须具有相同的大小和类型（如果输出图像与输入大小不匹配，则将重新分配输出图像）。此外，由于每个元素执行操作，因此输入图像之一可以用作输出。

Several operators that take a single image as the input are also available: `cv::sqrt`,`cv::pow`, `cv::abs`, `cv::cuberoot`, `cv::exp`, and `cv::log`. In fact, there exists an OpenCV function for almost any operation you have to apply on image pixels.

### There's more…
It is also possible to use the usual C++ arithmetic operator on the cv::Mat instances or on the individual channels of cv::Mat instances. The two following subsections explain how to do this.

也可以在`cv::Mat`实例或`cv::Mat`实例的各个通道上使用通常的C ++算术运算符。以下两个小节将解释如何执行此操作。

### Overloaded image operators
Very conveniently, most arithmetic functions have their corresponding operator overloaded in OpenCV. Consequently, the call to cv::addWeighted can instead be written as follows: 

非常方便的是，大多数算术函数在OpenCV中都有相应的运算符重载。因此，对`cv::addWeighted`的调用可以改为编写如下：

```
result= 0.7*image1+0.9*image2;
```
The preceding code is a more compact form that is also easier to read. These two ways of writing the weighted sum are equivalent. In particular, the cv::saturate_cast function will still be called in both cases.

前面的代码是一个更紧凑的形式，也更容易阅读。这两种写加权和的方法是等价的。特别是，在两种情况下仍将调用`cv::saturate_cast`函数。

Most C++ operators have been overloaded. Among them are the bitwise operators `&`, `|`, `^`, and `~`; the `min`, `max`, and `abs` functions. The comparison operators `<`, `<=`, `==`, `!=`, `>`, and `>=` have also been overloaded, and they return an 8-bit binary image. You will also find the $ m1*m2 $ matrix multiplication (where m1 and m2 are both `cv::Mat` instances), the `m1.inv()` matrix inversion, the `m1.t()` transpose, the `m1.determinant()` determinant, the `v1.norm()` vector norm, the `v1.cross(v2)` cross-product, the `v1.dot(v2)` dot product,and so on. When this makes sense, you also have the corresponding compound assignment operator defined (the `+=` operator, as an example).

大多数C ++运算符都已经过载。其中有按位运算符`＆`，`|`，`^`和`~`; `min`，`max`和`abs`函数。比较运算符`<`，`<=`，`==`，`！=`，`>`和`> =`也被重载，它们返回一个8位二进制图像。您还可以找到$ m1 * m2 $矩阵乘法（其中m1和m2都是`cv::Mat`实例），`m1.inv()`矩阵求逆，`m1.t()`转置， `m1.determinant()`行列式，`v1.norm()`向量范数，`v1.cross(v2)`交叉乘积，`v1.dotv2(v2)`点乘积，依此类推。当这有意义时，您还定义了相应的复合赋值运算符（例如`+=`运算符）。

In the *Writing efficient image-scanning loops* recipe, we presented a color-reduction function that was written using loops that scan the image pixels to perform some arithmetic operations on them. From what we learned here, this function could be rewritten simply using arithmetic operators on the input image as follows:

在*编写高效的图像扫描循环*一节中，我们提出了一种使用循环编写的减色功能，该循环扫描图像像素以对它们执行一些算术运算。根据我们在这里学到的，可以使用输入图像上的算术运算符重写此函数，如下所示：

```
image=(image&cv::Scalar(mask,mask,mask))+cv::Scalar(div/2,div/2,div/2);
```
The use of cv::Scalar is due to the fact that we are manipulating a color image. Using the image operators makes the code so simple, and the programmer so productive, that you should consider their use in most situations.

`cv::Scalar`的使用是由于我们正在操纵彩色图像。使用图像操作符可以使代码变得如此简单，并且程序员的工作效率也很高，因此在大多数情况下应该考虑使用它们。

### Splitting the image channels
Sometimes you want to process the different channels of an image independently. For example, you might want to perform an operation only on one channel of the image. You can, of course, achieve this in an image-scanning loop. However, you can also use the `cv::split` function that will copy the three channels of a color image into three distinct `cv::Mat` instances. Suppose we want to add our rain image to the blue channel only. The following is how we would proceed: 

有时您希望独立处理图像的不同通道。例如，您可能只想在图像的一个通道上执行操作。当然，您可以在图像扫描循环中实现此目的。但是，您也可以使用`cv::split`函数将彩色图像的三个通道复制到三个不同的`cv::Mat`实例中。假设我们只想将雨图像添加到蓝色通道。以下是我们将如何进行：

```c++
// create vector of 3 images
std::vector<cv::Mat> planes;
// split 1 3-channel image into 3 1-channel images
cv::split(image1,planes);
// add to blue channel
planes[0]+= image2;
// merge the 3 1-channel images into 1 3-channel image
cv::merge(planes,result);
```

The cv::merge function performs the inverse operation, that is, it creates a color image from three 1-channel images. 

`cv :: merge`函数执行逆操作，即，它从三个1通道图像创建彩色图像。

## 重新映射图像

In the recipes of this chapter, you learned how to read and modify the pixel values of an image. The last recipe will teach you how to modify the appearance of an image by moving its pixels. The pixel values are not changed by this process; it is rather the position of each pixel that is remapped to a new location. This is useful in order to create special effects on an image or to correct image distortions caused, for example,by a lens. 

在本章的内容中，您学习了如何读取和修改图像的像素值。最后这节将教你如何通过移动像素来修改图像的外观。此过程不会更改像素值;更确切地说，每个像素的位置被重新映射到新位置。这对于在图像上产生特殊效果或校正由例如镜头引起的图像失真是有用的。


### How to do it…
In order to use the OpenCV remap function, you simply have to first define the map to be used in the remapping process. Second, you have to apply this map on an input image.Obviously, it is the way you define your map that will determine the effect that will be produced. In our example, we define a transformation function that will create a wavy effect on the image:

要使用OpenCV重映射功能，您只需首先定义要在重映射过程中使用的映射。其次，您必须在输入图像上应用此贴图。显然，您定义地图的方式将决定将产生的效果。在我们的示例中，我们定义了一个转换函数，它将在图像上创建波浪效果：

```

// remapping an image by creating wave effects
void wave(const cv::Mat &image, cv::Mat &result) {
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
```
The result is as follows:
![](D:\Workplace\opencv_learn\OpenCV3Cookbook-cn\CH2-image9.jpg)

### How it works…
The objective of remapping is to produce a new version of an image in which pixels have changed in position. To construct this new image, we need to know what the original position is for each pixel in the destination image. The mapping function that is needed is therefore the one that will give us the original pixel positions as a function of the new pixel positions. This is called backward mapping because the transformation describes how the pixels of the new images are mapped back to the original image. In OpenCV, backward mapping is described using two maps: one for the x-coordinates and one for the ycoordinates. They are both represented by floating point cv::Mat instances: 

重新映射的目的是生成图像的新版本，其中像素的位置发生了变化。要构造这个新图像，我们需要知道目标图像中每个像素的原始位置。因此，所需的映射函数是将给出作为新像素位置的函数的原始像素位置的映射函数。这称为后向映射，因为变换描述了新图像的像素如何映射回原始图像。在OpenCV中，使用两个映射描述了向后映射：一个用于x坐标，一个用于y坐标。它们都由浮点cv :: Mat实例表示：

```c++
// the map functions
cv::Mat srcX(image.rows,image.cols,CV_32F); // x-map
cv::Mat srcY(image.rows,image.cols,CV_32F); // y-map
```
The size of these matrices will define the size of the destination image. The value of the (i,j) pixel of the destination image can then be read in the source image using the following line of code:

这些矩阵的大小将定义目标图像的大小。然后可以使用以下代码行在源图像中读取目标图像的（i，j）像素的值：

```c++
( srcX.at<float>(i,j) , srcY.at<float>(i,j) )
```
For example, a simple image flip effect like the one we demonstrated in Chapter 1, Playing with Images, can be created by the following maps:

例如，可以通过以下映射创建一个简单的图像翻转效果，如我们在第1章“使用图像”中演示的那样：

```c++
// creating the mapping
for (int i=0; i<image.rows; i++) {
    for (int j=0; j<image.cols; j++) {
        // horizontal flipping
        srcX.at<float>(i,j)= image.cols-j-1;
        srcY.at<float>(i,j)= i;
    }
}
```
To generate the resulting image, you simply call the OpenCV remap function:

```c++
// applying the mapping
cv::remap(image, // source image
          result, // destination image
          srcX, // x map
          srcY, // y map
          cv::INTER_LINEAR); // interpolation method
```
It is interesting to note that the two maps contain floating-point values. Consequently, a pixel in the destination can map back to a non-integral value (that is, a location between pixels). This is very convenient because this allows us to define the mapping function of our choice. For instance, in our remapping example, we used a sinusoidal function to define our transformation. However, this also means that we have to interpolate the value of virtual pixels in between real pixels. There exist different ways of performing pixel interpolation, and the last parameter of the remap function allows us to select the method that will be used. Pixel interpolation is an important concept in image processing; this subject will be discussed in Chapter 6, Filtering the Images.

有趣的是，这两个映射包含浮点值。因此，目标图像中的像素可以映射成非整数值（即，像素之间的位置）。这非常方便，因为这允许我们定义我们选择的映射函数。例如，在我们的重映射示例中，我们使用正弦函数来定义我们的转换。然而，这也意味着我们必须在真实像素之间插入虚拟像素的值。存在执行像素插值的不同方式，并且重映射函数的最后一个参数允许我们选择将要使用的方法。像素插值是图像处理中的一个重要概念;这个主题将在第6章“过滤图像”中讨论。

### See also
- The There's more… section of the Filtering images using low-pass filters recipe of Chapter 6, Filtering the Images, explains the concept of pixel interpolation
- The Calibrating a camera recipe of Chapter 11, Reconstructing 3D Scenes, uses remapping to correct lens distortions in an image
- The *Computing a homography between two images* recipe of Chapter 10, *Estimating  Projective Relations in Images*, uses perspective image warping to build an image panorama 

