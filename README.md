# A fast dehazing method for images based on recursive filtering
一种基于递归滤波的暗通道图像快速去雾方法。
该方法主要包括四个步骤：
1. 根据暗通道图像对大气光强进行快速估计;
2. 采用递归滤波的方法快速估计大气透射率;
3. 根据大气散射模型复原出去雾后的图像;
4. 利用伽马矫正的方法对去雾图像进行图像后处理。

# 有雾图像
![有雾](https://user-images.githubusercontent.com/80933553/178752729-8282378f-ae8e-4d5d-9492-e16e757cce92.jpg)
# 去雾图像
![去雾](https://user-images.githubusercontent.com/80933553/178752737-f9791afc-095b-4e43-8ad5-84363e86688d.jpg)
