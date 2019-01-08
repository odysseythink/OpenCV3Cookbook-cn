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



## 使用GrabCut算法分割图像
## 转换颜色表示
## 用色调，饱和度和亮度表示颜色