# A fast dehazing method for images based on recursive filtering
一种基于递归滤波的暗通道图像快速去雾方法。
该方法主要包括四个步骤：
1. 根据暗通道图像对大气光强进行快速估计;
2. 采用递归滤波的方法快速估计大气透射率;
3. 根据大气散射模型复原出去雾后的图像;
4. 利用伽马矫正的方法对去雾图像进行图像后处理。
## 有雾图像
<div align="center">
   <img src="https://user-images.githubusercontent.com/80933553/178892187-7d4cfa60-b8a7-4e44-84df-3d9e010f63b8.jpg" width=200 height=160> <img src="https://user-images.githubusercontent.com/80933553/178892228-a0550c96-4ede-4fc7-93af-f9f3d852f57d.jpg" width=200 height=160> <img src="https://user-images.githubusercontent.com/80933553/178892233-c72832e8-ac78-4500-948f-41e8f32d605a.jpg" width=200 height=160>
   </div>
<div align="center">
   <font color="AAAAAA">城市高空图像</font>
	 &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;
	 <font color="AAAAAA">城市道路图像</font>
	 &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;
	 <font color="AAAAAA">城市江景图像</font>
   </div>
<div align="center">
   <img src="https://user-images.githubusercontent.com/80933553/178892239-22fe3eb3-9e1a-4a9b-a608-6f3050a9e719.jpg" width=200 height=160> <img src="https://user-images.githubusercontent.com/80933553/178892241-42b3842e-4217-462a-8f5f-c04772bdb4d7.jpg" width=200 height=160> <img src="https://user-images.githubusercontent.com/80933553/178892247-872be666-220d-4b7d-bf11-94e0360c78d9.jpg" width=200 height=160>
   </div>
<div align="center">
   <font color="AAAAAA">农村田野图像</font>
	 &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;
	 <font color="AAAAAA">山间村庄图像</font>
	 &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;
	 <font color="AAAAAA">深山树林图像</font>
</div>

## 去雾图像
<div align="center">
   <img src="https://user-images.githubusercontent.com/80933553/178894247-57a0581e-9efe-4701-b800-db650c368497.jpg" width=200 height=160> <img src="https://user-images.githubusercontent.com/80933553/178894295-949388e4-dad2-47f2-8cc5-17b10f416e40.jpg" width=200 height=160> <img src="https://user-images.githubusercontent.com/80933553/178894301-be3c9c9f-c88c-4b5f-af5e-c09444475155.jpg" width=200 height=160>
   </div>
<div align="center">
   <font color="AAAAAA">城市高空图像</font>
	 &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;
	 <font color="AAAAAA">城市道路图像</font>
	 &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;
	 <font color="AAAAAA">城市江景图像</font>
   </div>
<div align="center">
   <img src="https://user-images.githubusercontent.com/80933553/178894303-848a5885-f342-4cf9-837b-a3d994871818.jpg" width=200 height=160> <img src="https://user-images.githubusercontent.com/80933553/178894307-3fc7c173-0335-4fd5-9be5-9e0fc7879250.jpg" width=200 height=160> <img src="https://user-images.githubusercontent.com/80933553/178894313-7d664f05-ecf9-42b3-aa7c-eb5a0826a73d.jpg" width=200 height=160>
   </div>
<div align="center">
   <font color="AAAAAA">农村田野图像</font>
	 &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;
	 <font color="AAAAAA">山间村庄图像</font>
	 &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;
	 <font color="AAAAAA">深山树林图像</font>
</div>
