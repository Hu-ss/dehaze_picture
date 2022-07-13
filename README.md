# A fast dehazing method for images based on recursive filtering
一种基于递归滤波的暗通道图像快速去雾方法。
该方法主要包括四个步骤：
1. 根据暗通道图像对大气光强进行快速估计;
2. 采用递归滤波的方法快速估计大气透射率;
3. 根据大气散射模型复原出去雾后的图像;
4. 利用伽马矫正的方法对去雾图像进行图像后处理。

<figure class="third">
    <img src="https://img-blog.csdn.net/2018061215200776?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxODA4OTYx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70" width="200"/><img src="https://img-blog.csdn.net/20180612152032532?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxODA4OTYx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70" width="200"/><img src="https://img-blog.csdn.net/20180612152100203?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxODA4OTYx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70" width="200"/>
</figure>

![1](https://user-images.githubusercontent.com/80933553/178745665-a351303e-7948-41e0-957d-df54fd4914fd.jpg)
