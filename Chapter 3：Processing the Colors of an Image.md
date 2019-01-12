# 第三章 处理图像的颜色

In this chapter, we will cover the following recipes:

- Comparing colors using the Strategy design pattern使用策略设计模式比较颜色
- Segmenting an image with the GrabCut algorithm使用GrabCut算法分割图像
- Converting color representations转换颜色表示
- Representing colors with hue, saturation, and brightness 用色调，饱和度和亮度表示颜色

## 简介

The ability to see the world in colors is one of the important characteristics of the human visual system.The retina([ret·i·na || 'retɪnə]n.  视网膜) of the human eye includes specialized photoreceptors([ˌfəutəuriˈseptə]
光感受器; 光敏接收器件; 光受器; 感光器; 感光受器), called cones([kəʊn]n.  圆锥体, 球果v.  使成锥形。这里翻译成"视锥细胞"), which are responsible for the perception([per·cep·tion || pər'sepʃn /pə'-]n.  知觉, 领悟力,  感觉) of colors. There are three types of cones that differ in the wavelength range of light they absorb([ab·sorb || əb'sɔːb]v.  吸收; 使全神贯注; 汲取,  理解); using the stimuli(['stim·u·lus || stɪmjələs /-mjʊl-]n.  刺激, 刺激品,  激励) from these different cells, the human brain is able to create color perception. Most other animals only have rod([rɑd /rɒd]n.  竿, 小枝,  笞鞭) cells, which are photoreceptors with better light sensitivity but that cover the full spectrum of visible light without color discrimination. In the human eye, rods are mainly located at the periphery of the retina, while the cones are concentrated in the central part. 

用颜色看世界的能力是人类视觉系统的重要特征之一。人眼的视网膜包括专门的光感受器，称为视锥细胞，负责感知颜色。有三种类型的锥体在它们吸收的光的波长范围内不同;利用来自这些不同细胞的刺激，人类的大脑能够产生色彩感知。大多数其他动物只有杆细胞，它们是光感受器，具有更好的光敏感性，但覆盖了全部可见光，没有颜色辨别。在人眼中，杆主要位于视网膜的周边，而锥体集中在视网膜的中央部分。

In digital imaging, colors are generally reproduced by using the red, green, and blue additive([ad·di·tive || 'ædɪtɪv]n.  添加剂; 添加物; 加法) primary colors.These have been selected because when they are combined together, they can produce a wide gamut(['gæmәt]
n.  音阶, 整个范围, 全部, 音域) of different colors. In fact, this choice of primaries mimics well the trichromatic color perception of the human visual system as the different cone cells have sensitivity located around the red, green, and blue spectrum. In this chapter,you will play with the pixel color and see how an image can be segmented based on the color information. In addition, you will learn that it can sometimes be useful to use a different color representation when performing color image processing. 

在数字成像中，通常使用红色，绿色和蓝色这几种原色组合再现颜色。选择这些颜色是因为当它们组合在一起时，它们可以产生不同颜色的宽色域。实际上，这种原色的选择很好地模拟了人类视觉系统的三色颜色感知，因为不同的锥形细胞具有位于红色，绿色和蓝色光谱周围的灵敏度。在本章中，您将使用像素颜色，并查看如何根据颜色信息分割图像。此外，您将了解在执行彩色图像处理时使用不同的颜色表示有时会很有用。





## 使用策略设计模式比较颜色

Let's say we want to build a simple algorithm that will identify all of the pixels in an image that have a given color. For this, the algorithm has to accept an image and a color as input and will return a binary image showing the pixels that have the specified color. The tolerance with which we want to accept a color will be another parameter to be specified before running the algorithm. 

假设我们想要构建一个简单的算法来识别图像中具有给定颜色的所有像素。为此，算法必须接受图像和颜色作为输入，并返回显示具有指定颜色的像素的二进制图像。我们想要接受颜色的容差将是在运行算法之前指定的另一个参数。

In order to accomplish this objective, this recipe will use the Strategy design pattern. This object-oriented design pattern constitutes an excellent way of encapsulating an algorithm in a class. It becomes then easier to replace a given algorithm with another one, or to chain several algorithms together in order to build a more complex process. In addition, this pattern facilitates the deployment of an algorithm by hiding as much of its complexity as possible behind an intuitive programming interface. 

为了实现这一目标，该配方将使用策略设计模式。这种面向对象的设计模式构成了将算法封装在类中的极好方法。然后更容易用另一个算法替换给定的算法，或者将几个算法链接在一起以构建更复杂的过程。此外，该模式通过在直观的编程接口后面隐藏尽可能多的复杂性来促进算法的部署。

### How to do it…
Once an algorithm has been encapsulated in a class using the Strategy design pattern, it can be deployed by creating an instance of this class. Typically, the instance will be created when the program is initialized.At  the time of construction, the class instance will initialize the different parameters of the algorithm with their  default values so that it will immediately be ready to be used. The algorithm's parameter values can also be read and set using appropriate methods. In the case of an application with a GUI, these parameters can be displayed and modified using different widgets (text fields, sliders, and so on) so that a user can easily play with them. 

使用策略设计模式将算法封装在类中后，可以通过创建此类的实例来部署它。通常，在初始化程序时将创建实例。在构造时，类实例将使用其默认值初始化算法的不同参数，以便立即准备好使用它。也可以使用适当的方法读取和设置算法的参数值。对于具有GUI的应用程序，可以使用不同的小部件（文本字段，滑块等）显示和修改这些参数，以便用户可以轻松地使用它们。

We will show you the structure of a `Strategy` class in the next section; let's start with an example of how it can be deployed and used. Let's write a simple `main` function that will run our proposed color detection algorithm: 

我们将在下一节向您展示`Strategy`类的结构;让我们从一个如何部署和使用它的例子开始。让我们编写一个简单的`main`函数来运行我们提出的颜色检测算法：

```
int main()
{
    //1. Create image processor object
    ColorDetector cdetect;
    //2. Read input image
    cv::Mat image= cv::imread("boldt.jpg");
    if (image.empty()) return 0;
    //3. Set input parameters
    cdetect.setTargetColor(230,190,130); // here blue sky
    //4. Process the image and display the result
    cv::namedWindow("result");
    cv::Mat result = cdetect.process(image);
    cv::imshow("result",result);
    cv::waitKey();
    return 0;
}
```

Running this program to detect a blue sky in the colored version of the Castle image presented in the previous chapter produces the following output: 

运行此程序以检测前一章中呈现的城堡图像的彩色版本中的蓝天，产生以下输出：

![]()

Here, a white pixel indicates a positive detection of the sought color, and black indicates negative.

这里，白色像素表示所寻找颜色的正检测，黑色表示负。

Obviously, the algorithm we encapsulated in this class is relatively simple (as we will see next, it is composed of just one scanning loop and one tolerance parameter). The Strategy design pattern becomes really powerful when the algorithm to be implemented is more complex, has many steps, and includes several parameters. 

显然，我们封装在这个类中的算法相对简单（我们将在下面看到，它只由一个扫描循环和一个容差参数组成）。当要实现的算法更复杂，步骤多，并且包含多个参数时，策略设计模式变得非常强大。

### How it works…
The core process of this algorithm is easy to build. It is a simple scanning loop that goes over each pixel,comparing its color with the target color. Using what we learned in the *Scanning an image with iterators* recipe of the previous chapter, this loop can be written as follows: 

该算法的核心过程易于构建。这是一个简单的扫描循环，遍历每个像素，将其颜色与目标颜色进行比较。使用我们在上一章的使用迭代器配方扫描图像中学到的知识，可以按如下方式编写此循环：

```c++
// get the iterators
cv::Mat_<cv::Vec3b>::const_iterator it= image.begin<cv::Vec3b>();
cv::Mat_<cv::Vec3b>::const_iterator itend= image.end<cv::Vec3b>();
cv::Mat_<uchar>::iterator itout= result.begin<uchar>();
//for each pixel
for ( ; it!= itend; ++it, ++itout) {
    // compute distance from target color
    if (getDistanceToTargetColor(*it)<=maxDist) {
    	*itout= 255;
    } else {
    	*itout= 0;
    }
}
```

The `cv::Mat` variable `image` refers to the input image, while `result` refers to the binary output image.Therefore, the first step consists of setting up the required iterators. The scanning loop then becomes easy to implement. Note that the input image iterators are declared `const` as the values of their elements are not modified. The distance between the current pixel color and the target color is evaluated for each pixel in order to check whether it is within the tolerance parameter defined by `maxDist`. If that is the case, the value `255` (white) is then assigned to the output image; if not, 0 (black) is assigned. To compute  the distance to the target color, the `getDistanceToTargetColor` method is used. There are different ways to compute this distance. 

`cv::Mat`变量`image`是指输入图像，而`result`是指二进制输出图像。因此，第一步是设置所需的迭代器。然后扫描循环变得易于实现。请注意，输入图像迭代器被声明为“const”，因为它们的元素值不会被修改。针对每个像素评估当前像素颜色和目标颜色之间的距离，以检查它是否在由“maxDist”定义的容差参数内。如果是这种情况，则将值“255”（白色）分配给输出图像;如果不是，则分配0（黑色）。要计算到目标颜色的距离，使用`getDistanceToTargetColor`方法。有不同的方法来计算这个距离。

One could, for example, calculate the Euclidean distance between the three vectors that contain the RGB color values. To keep this computation simple, we sum the absolute differences of the RGB values (this is also known as the **city-block distance**). Note that in modern architecture, a floating-point Euclidean distance  can be faster to compute than a simple city-block distance (in addition, you can also use squared Euclidean distances to avoid the costly square-root); this is also something to take into consideration in your design.Also, for more flexibility, we write the `getDistanceToTargetColor` method in terms of a getColorDistance method, as follows: 

例如，可以计算包含RGB颜色值的三个矢量之间的欧几里德距离。为了简化这个计算，我们总结了RGB值的绝对差值（这也称为**city-block distance**）。请注意，在现代建筑中，浮点欧几里德距离的计算速度可能比简单的城块距离更快（此外，您还可以使用平方欧几里德距离来避免昂贵的平方根）;这也是你的设计中需要考虑的因素。另外，为了更加灵活，我们根据`getColorDistance`方法编写`getDistanceToTargetColor`方法，如下所示：

```c++
// Computes the distance from target color.
int getDistanceToTargetColor(const cv::Vec3b& color) const {
	return getColorDistance(color, target);
}
// Computes the city-block distance between two colors.
int getColorDistance(const cv::Vec3b& color1,
const cv::Vec3b& color2) const {
    return abs(color1[0]-color2[0])+
           abs(color1[1]-color2[1])+
           abs(color1[2]-color2[2]);
}
```

Note how we used `cv::Vec3d` to hold the three unsigned chars that represent the RGB values of a color.The `target` variable obviously refers to the specified target color, and as we will see, it is defined as a member variable in the class algorithm that we will define.Now, let's complete the definition of the processing method. Users will provide an input image, and the result will be returned once the image scanning is completed: 

注意我们如何使用`cv::Vec3d`来保存表示颜色的RGB值的三个无符号字符。“target”变量显然是指指定的目标颜色，正如我们将看到的，它被定义为成员我们将定义的类算法中的变量。现在，让我们完成处理方法的定义。用户将提供输入图像，并在图像扫描完成后返回结果：

```c++
cv::Mat ColorDetector::process(const cv::Mat &image) {
    // re-allocate binary map if necessary
    // same size as input image, but 1-channel
    result.create(image.size(),CV_8U);
    // processing loop above goes here
    return result;
}
```

Each time this method is called, it is important to check if the output image that contains the resulting binary map needs to be reallocated to fit the size of the input image. This is why we use the `create` method of `cv::Mat`. Remember that this method will only proceed to reallocation if the specified size and/or depth do not correspond to the current image structure. 

每次调用此方法时，检查包含生成的二进制映射的输出图像是否需要重新分配以适合输入图像的大小非常重要。这就是我们使用`cv::Mat`的`create`方法的原因。请记住，如果指定的大小和/或深度与当前图像结构不对应，则此方法将仅继续重新分配。

Now that we have the core processing method defined, let's see what additional methods should be added in order to deploy this algorithm. We have previously determined what input and output data our algorithm requires. Therefore, we define the class attributes that will hold this data:

现在我们已经定义了核心处理方法，让我们看看为了部署这个算法应该添加哪些额外的方法。我们之前已经确定了算法所需的输入和输出数据。因此，我们定义将保存此数据的类属性：
```c++
class ColorDetector {
    private:
    // minimum acceptable distance
    int maxDist;
    // target color
    cv::Vec3b target;
    // image containing resulting binary map
    cv::Mat result;
```

In order to create an instance of the class that encapsulates our algorithm (which we have named `ColorDetector`), we need to define a constructor. Remember that one of the objectives of the Strategy design pattern is to make algorithm deployment as easy as possible. The simplest constructor that can be defined is an empty one. It will create an instance of the class algorithm in a valid state. We then want the constructor to initialize all the input parameters to their default values (or the values that are known to generally give a good result). In our case, we decided that a distance of 100 is generally an acceptable tolerance parameter. We also set the default target color. We chose black for no particular reason. The idea is to make sure we always start with predictable and valid input values: 

为了创建封装我们的算法（我们命名为`ColorDetector`）的类的实例，我们需要定义一个构造函数。请记住，策略设计模式的目标之一是尽可能简化算法部署。可以定义的最简单的构造函数是空的。它将在有效状态下创建类算法的实例。然后，我们希望构造函数将所有输入参数初始化为其默认值（或者已知通常会产生良好结果的值）。在我们的例子中，我们认为距离100通常是可接受的公差参数。我们还设置了默认目标颜色。我们选择黑色没有特别的原因。我们的想法是确保始终以可预测且有效的输入值开头：

```c++
// empty constructor
// default parameter initialization here
ColorDetector() : maxDist(100), target(0,0,0) {}
```
Another option would have been not create an empty constructor and rather force the user to input a target color and a color distance in a more elaborated([i'læbәreit]a.  精细的, 详尽的, 精心计划(或制作)的vt.  详细地说明, 用心地制作, 发挥vi.  变复杂, 作详细说明) constructor:

另一种选择不是创建一个空构造函数，而是强制用户在更精细的构造函数中输入目标颜色和颜色距离：

```c++
// another constructor with target and distance
ColorDetector(uchar blue, uchar green, uchar red, int mxDist);
```
At this point, a user who creates an instance of our class algorithm can immediately call the process method with a valid image and obtain a valid output. This is another objective of the Strategy pattern, that is, to make sure that the algorithm always runs with valid parameters. Obviously, the users of this class will want to use their own settings. This is done by providing the user with the appropriate getters and setters. Let's start with the color tolerance parameter:

此时，创建类算法实例的用户可以立即使用有效图像调用process方法并获取有效输出。这是策略模式的另一个目标，即确保算法始终使用有效参数运行。显然，这个类的用户会想要使用自己的设置。这是通过向用户提供适当的getter和setter来完成的。让我们从颜色容差参数开始：

```c++
// Sets the color distance threshold
// Threshold must be positive,
// otherwise distance threshold is set to 0.
void setColorDistanceThreshold(int distance) {
    if (distance<0)
    	distance=0;
    maxDist= distance;
}
// Gets the color distance threshold
int getColorDistanceThreshold() const {
	return maxDist;
}
```
Note how we first check the validity( [va·lid·i·ty || və'lɪdətɪ]n.  有效性; 正确性; 合法性) of the input. Again, this is to make sure that our algorithm will never be run in an invalid state. The target color can be set in a similar manner, as follows:

请注意我们如何首先检查输入的有效性。同样，这是为了确保我们的算法永远不会在无效状态下运行。可以以类似的方式设置目标颜色，如下所示：

```c++
// Sets the color to be detected
void setTargetColor(uchar blue, uchar green, uchar red) {
    // BGR order
    target = cv::Vec3b(blue, green, red);
}
// Sets the color to be detected
void setTargetColor(cv::Vec3b color) {
    target= color;
}
// Gets the color to be detected
cv::Vec3b getTargetColor() const {
    return target;
}
```
This time, it is interesting to note that we have provided the user with two definitions of the `setTargetColor` method. In the first version of the definition, the three color components are specified as three arguments, while in the second version, `cv::Vec3b` is used to hold the color values. Again, the objective is to facilitate the use of our class algorithm. The users can simply select the setter that best fits their needs.

这一次，有趣的是我们已经为用户提供了`setTargetColor`方法的两个定义。在定义的第一个版本中，三个颜色组件被指定为三个参数，而在第二个版本中，`cv::Vec3b`用于保存颜色值。同样，目标是促进我们的类算法的使用。用户可以简单地选择最适合他们需求的setter。

### There's more…
The example algorithm used in this recipe consisted of identifying the pixels of an image that has a color sufficiently close to a specified target color. This computation could have been done otherwise.Interestingly, an OpenCV function performs a similar task in order to extract a connected component of a given color. Also, the implementation of a Strategy design pattern could be complemented using function objects. Finally, OpenCV has defined a base class, `cv::Algorithm`, that implements the Strategy design pattern concepts.

这节中使用的示例算法包括识别具有足够接近指定目标颜色的颜色的图像的像素。这种计算可能已经完成。有趣的是，OpenCV函数执行类似的任务以便提取给定颜色的连通分量。此外，可以使用函数对象来补充策略设计模式的实现。最后，OpenCV定义了一个基类`cv::Algorithm`，它实现了Strategy设计模式的概念。

### Computing the distance between two color vectors 

To compute the distance between two color vectors, we used the following simple formula: 

为了计算两个颜色向量之间的距离，我们使用了以下简单公式：

```c++
return abs(color[0]-target[0])+abs(color[1]-target[1])+abs(color[2]-target[2]);
```
However, OpenCV includes a function to compute the Euclidean norm of a vector.Consequently, we could have computed our distance as follows:

然而，OpenCV包含一个计算向量的欧几里德范数的函数。因此，我们可以计算我们的距离如下：

```c++
return static_cast<int>(cv::norm<int,3>(cv::Vec3i(color[0]-target[0],color[1]-target[1],color[2]-target[2])));
```
A very similar result would then be obtained using this definition of the getDistance method. Here, we use cv::Vec3i (a 3-vector array of integers) because the result of the subtraction is an integer value.

然后使用getDistance方法的这个定义获得非常相似的结果。这里，我们使用cv :: Vec3i（一个3向量的整数数组），因为减法的结果是一个整数值。

It is also interesting to recall from Chapter 2, Manipulating Pixels, that the OpenCV matrix and vector data structures include a definition of the basic arithmetic operators.Consequently, one could have proposed the following definition for the distance computation:

从第2章，操纵像素回忆起，OpenCV矩阵和矢量数据结构包括基本算术运算符的定义也是有趣的。因此，可以为距离计算提出以下定义：

```c++
return static_cast<int>( cv::norm<uchar,3>(color-target));// wrong!
```
This definition may look right at the first glance; however, it is wrong. This is because all these operators always include a call to saturate_cast (see the Scanning an image with neighbor access recipe in the previous chapter) in order to ensure that the results stay within the domain of the input type (here, it is uchar). Therefore, in the cases where the target value is greater than the corresponding color value, the value 0 will be assigned instead of the negative value that one would have expected. A correct formulation would then be as follows:

乍一看，这个定义可能看起来正确;但是，这是错误的。这是因为所有这些运算符总是包含对saturate_cast的调用（请参阅上一章中使用邻居访问扫描图像的内容），以确保结果保持在输入类型的域内（此处为uchar）。因此，在目标值大于相应颜色值的情况下，将分配值0而不是人们预期的负值。那么正确的表述如下：
```c++
cv::Vec3b dist;
cv::absdiff(color,target,dist);
return cv::sum(dist)[0];
```



## 使用GrabCut算法分割图像
## 转换颜色表示
## 用色调，饱和度和亮度表示颜色