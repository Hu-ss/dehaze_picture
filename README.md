# dehaze_picture
A fast dehazing method for images based on recursive filtering
一种基于递归滤波的暗通道图像快速去雾方法。
该方法主要包括四个步骤：
1. 根据暗通道图像对大气光强进行快速估计
2. 采用递归滤波的方法快速估计大气透射率
3. 根据大气散射模型复原出去雾后的图像
4. 利用伽马矫正的方法对去雾图像进行图像后处理。
