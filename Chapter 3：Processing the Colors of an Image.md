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
## 使用GrabCut算法分割图像
## 转换颜色表示
## 用色调，饱和度和亮度表示颜色