#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>
#include<vector>
#include <algorithm>
#include <iostream>
#include <opencv2\opencv.hpp>
#include <cmath>
#include <opencv2/core/utils/logger.hpp>
//#include"stretchlim.h"
//#include"imadjust.h"

using namespace cv;
using namespace std;

//暗通道计算
Mat darkChannel(Mat W, int scale)
{
	Mat rgbmin = Mat::zeros(W.rows, W.cols, CV_64FC1);
	int nc1 = W.rows* W.cols;
	for (int j = 0; j<1; j++)
	{
		double* data1 = rgbmin.ptr<double>(j);
		double* data2 = W.ptr<double>(j);
		for (int i = 0; i<nc1; i++)
		{
			*data1++ = min(min(*data2++, *data2++), *data2++);
		}
	}
	//最小值滤波
	Mat D_s(W.rows, W.cols, CV_64FC1);
	int radius = (scale - 1) / 2;
	Mat border;
	copyMakeBorder(rgbmin, border, radius, radius, radius, radius, BORDER_REFLECT_101);
	for (int j = 0; j<W.rows; j++)
	{
		double* data1 = D_s.ptr<double>(j);
		for (int i = 0; i<W.cols; i++)
		{
			//选取兴趣区域
			Mat roi;
			roi = border(Rect(i, j, scale, scale));
			//求兴趣区域的最小值
			double minVal = 0; double maxVal = 0;
			Point minLoc = 0; Point maxLoc = 0;
			minMaxLoc(roi, &minVal, &maxVal, &minLoc, &maxLoc, noArray());
			*data1++ = (double)minVal;
		}
	}

	return D_s;
}

//估计大气光强度
double calculate_A(Mat WW, Mat D_s)
{
	double sum_A = 0;
	double n = (D_s.cols)*(D_s.rows) / 1000;
	Mat temp; D_s.copyTo(temp);
	for (int i = 0; i < n; i++) 
	{
		double minVal = 0; double maxVal = 0;
		Point minLoc = 0; Point maxLoc = 0;
		minMaxLoc(temp, &minVal, &maxVal, &minLoc, &maxLoc, noArray());
		
		sum_A = sum_A + ((WW.at<Vec3d>(maxLoc)[0]+ WW.at<Vec3d>(maxLoc)[1]+ WW.at<Vec3d>(maxLoc)[2])/3.0);
		//sum_A = sum_A + (W.at<double>(maxLoc));
		temp.at<double>(maxLoc) = temp.at<double>(minLoc);
	}
	double A = sum_A / (int)n;
	double max_A =230.0 / 255;
	if (A > max_A) {
		A = max_A;
	}
	return A;
}

//递归滤波
Mat TransformedDomainRecursiveFilter_Horizontal(Mat I, Mat D, double sigma)
{
	Mat F;
	I.copyTo(F);
	double a;
	a = exp(-sqrt(2) / sigma);

	Mat V(D.rows, D.cols, CV_64FC1);
	int nc1 = D.rows* D.cols;
	for (int j = 0; j<1; j++)
	{
		double* data1 = V.ptr<double>(j);
		double* data2 = D.ptr<double>(j);
		for (int i = 0; i<nc1; i++)
		{
			*data1++ = pow(a, (*data2++));
		}
	}

	//从左往右滤波
	int nl2 = I.rows;
	int nc2 = (I.cols - 1);
	for (int j = 0; j<nl2; j++)
	{
		double* data1 = F.ptr<double>(j);
		double* data2 = V.ptr<double>(j);
		for (int i = 0; i<nc2; i++)
		{
			data1[i+1] = data1[i+1] + (data2[i+1]*(data1[i] - data1[i+1]));
		}
	}

	//从右往左滤波
	int nl3 = I.rows;
	int nc3 = (I.cols - 2);
	for (int j = 0; j<nl3; j++)
	{
		double* data1 = F.ptr<double>(j);
		double* data2 = V.ptr<double>(j);
		for (int i = nc3; i>-1; i--)
		{
			data1[i] = data1[i] + (data2[i + 1] * (data1[i + 1] - data1[i]));
		}
	}
	return F;
}

//域变换递归滤波
Mat RF(Mat I, double sigma_s, double sigma_r, double num_iterations, Mat J)
{
	
	//计算定义域变换
	Mat dIcdx(J.rows, (J.cols - 1), CV_64FC3);
	Mat dIcdy((J.rows - 1), J.cols, CV_64FC3);

	int nl1 = dIcdx.rows;
	int nc1 = dIcdx.cols*dIcdx.channels();
	for (int j = 0; j<nl1; j++) 
	{
		double* data1 = dIcdx.ptr<double>(j);
		double* data2 = J.ptr<double>(j);
		for (int i = 0; i<nc1; i++)
		{
			data1[i] = data2[i + 3] - data2[i];
		}
	}
	
	int nl2 = dIcdy.rows;
	int nc2 = dIcdy.cols*dIcdy.channels();
	for (int j = 0; j<nl2; j++)
	{
		double* data1 = dIcdy.ptr<double>(j);
		double* data2 = J.ptr<double>(j);
		double* data3 = J.ptr<double>(j+1);
		for (int i = 0; i<nc2; i++)
		{
			//*data1++ = (*data3++) - (*data2++);
			data1[i] = data3[i] - data2[i];
		}
	}

	Mat dIdx = Mat::zeros(J.rows, J.cols, CV_64FC1);
	Mat dIdy = Mat::zeros(J.rows, J.cols, CV_64FC1);

	int nl3 = dIcdx.rows;
	int nc3 = dIcdx.cols;
	for (int j = 0; j<nl3; j++)
	{
		double* data1 = dIdx.ptr<double>(j);
		double* data2 = dIcdx.ptr<double>(j);
		for (int i = 0; i<nc3; i++)
		{
			data1[i + 1] = data1[i + 1] + abs(*data2++) + abs(*data2++) + abs(*data2++);
		}
	}

	int nl4 = dIcdy.rows;
	int nc4 = dIcdy.cols;
	for (int j = 0; j<nl4; j++)
	{
		double* data1 = dIdy.ptr<double>(j+1);
		double* data2 = dIcdy.ptr<double>(j);
		for (int i = 0; i<nc4; i++)
		{
			data1[i] = data1[i] + abs(*data2++) + abs(*data2++) + abs(*data2++);
		}
	}
	
	Mat dHdx = Mat::zeros(J.rows, J.cols, CV_64FC1);
	Mat dVdy = Mat::zeros(J.rows, J.cols, CV_64FC1);
	int nc5 = J.rows*J.cols;
	for (int j = 0; j<1; j++)
	{
		double* data1 = dHdx.ptr<double>(j);
		double* data2 = dIdx.ptr<double>(j);
		double* data3 = dVdy.ptr<double>(j);
		double* data4 = dIdy.ptr<double>(j);
		for (int i = 0; i<nc5; i++)
		{
			*data1++ = 1 + ((sigma_s / sigma_r) * (*data2++));
			*data3++ = 1 + ((sigma_s / sigma_r) * (*data4++));
		}
	}
    //进行转置
	dVdy = dVdy.t();
	//执行过滤
	double N = num_iterations;
	Mat F;
	I.copyTo(F);

	double sigma_H;
	for (int k = 0; k < 3; k++)
	{
		sigma_H = (sigma_s * sqrt(3) * pow(2, (N - (k + 1)))) / sqrt(pow(4, N) - 1);
		F = TransformedDomainRecursiveFilter_Horizontal(F, dHdx, sigma_H);
		F = F.t();
		
		F = TransformedDomainRecursiveFilter_Horizontal(F, dVdy, sigma_H);
		F = F.t();

	}
	return F;
}

//伽马变换
/*
Mat imadjust(Mat I, double low_in, double high_in, double low_out, double high_out, double gamma) 
{
	int nl = 1;
	int nc = I.cols*I.rows*I.channels();
	Mat J;
	I.copyTo(J);

	for (int j = 0; j < nl; j++)
	{
		double* data1 = J.ptr<double>(j);
		for (int i = 0; i < nc; i++) 
		{
			if (*data1 <= low_in) 
			{
				*data1++ = low_out;
			}
			else if (*data1 >= high_in)
			{
				*data1++ = high_out;
			}
			else
			{
				*data1 = pow(((*data1) - low_in) / (high_in - low_in),gamma);
				*data1 = (*data1)*(high_out - low_out) + low_out;
				*data1++;
			}
			
		}
	}
	return J;
}
*/

//伽马变换
uchar lut[256];
void getlut(double gamma)
{
	for (int i = 0; i < 256; i++)
	{
		lut[i] = saturate_cast<uchar>(pow((float)(i / 255.0), gamma) * 255.0f);
	}
}
Mat imadjust(Mat I, int low_in, int high_in, int low_out, int high_out)
{
	int nl = 1;
	int nc = I.cols*I.rows*I.channels();
	Mat temp;
	I.copyTo(temp);

	for (int j = 0; j < nl; j++)
	{
		uchar* data1 = temp.ptr<uchar>(j);
		for (int i = 0; i < nc; i++)
		{
			if (*data1 <= low_in)
			{
				*data1++ = lut[low_out];
			}
			else if (*data1 >= high_in)
			{
				*data1++ = lut[high_out];
			}
			else
			{
				*data1 = lut[(*data1)];
				*data1++;
			}
		}
	}
	return temp;
}

int main(int argc, char*argv[])
{
	cv::utils::logging::setLogLevel(utils::logging::LOG_LEVEL_SILENT);//不再输出日志

	double start = static_cast<double>(getTickCount());
	Mat image = imread("../picture/图片1.jpg");
	namedWindow("原图", 0);
	imshow("原图", image);
	Mat image1;
	image.convertTo(image1, CV_64FC3, 1.0 / 255, 0); //转化为double形式
	Mat W;
	resize(image1, W, Size(image.cols/2 , image.rows/2 ), INTER_AREA);

	int nl = 1;
	int nc1 = image.rows*image.cols;
	int nc2 = W.rows*W.cols;
	//暗通道
	Mat D_s = darkChannel(W, 5);
	    
	//计算大气光强A
	double A = calculate_A(W, D_s);
	
	//double A = 0.56;
	//域变换递归滤波
	D_s = RF(D_s, 30, 0.8, 3, W);
	
	//求取透射率D_1
	Mat D_1(D_s.rows, D_s.cols, CV_64FC1);
	for (int j = 0; j<nl; j++)
	{
		double* data1 = D_1.ptr<double>(j);
		double* data2 = D_s.ptr<double>(j);
		for (int i = 0; i<nc2; i++)
		{
			*data1++ = 1 - (*data2++);
		}
	}
	resize(D_1, D_1, Size(image.cols, image.rows), INTER_LINEAR);
	
	//图像复原
	Mat dahazed(image.rows, image.cols, CV_64FC3);
	for (int j = 0; j<nl; j++)
	{
		double* data1 = dahazed.ptr<double>(j);
		double* data2 = image1.ptr<double>(j);
		double* data3 = D_1.ptr<double>(j);
		for (int i = 0; i<nc1; i++)
		{
			*data1++ = (((*data2++) - A) / (*data3)) + A;
			*data1++ = (((*data2++) - A) / (*data3)) + A;
			*data1++ = (((*data2++) - A) / (*data3++)) + A;
		}
	}
	
	/*
	//伽马变化，图像增强
	//图像二值化
	Mat bwmap;
	threshold(D_s, bwmap, 0.82, 1, THRESH_BINARY);
	resize(bwmap, bwmap, Size(bwmap.cols / 10, bwmap.rows / 10), INTER_LINEAR);
	resize(bwmap, bwmap, Size(image.cols, image.rows), INTER_LINEAR);
	Mat frame;
	dahazed = imadjust(dahazed, 0.02, 0.98, 0.02, 0.98,1);
	frame = imadjust(image1, 0.02, 0.98, 0.02, 0.98, 1);
	for (int j = 0; j < nl; j++) 
	{
		double* data1 = dahazed.ptr<double>(j);
		double* data2 = frame.ptr<double>(j);
		double* data3 = bwmap.ptr<double>(j);
		for (int i = 0; i < nc1; i++) 
		{
			*data1 = ((*data1++)*(1 - (*data3))) + ((*data2++)*(*data3));
			*data1 = ((*data1++)*(1 - (*data3))) + ((*data2++)*(*data3));
			*data1 = ((*data1++)*(1 - (*data3))) + ((*data2++)*(*data3++));
		}
	}
	*/

	//dahazed.convertTo(dahazed, CV_8UC3, 255, 0); //转化为double形式
	//imwrite("直方图均衡化原始图.jpg", dahazed);

	getlut(0.7);
	dahazed.convertTo(dahazed, CV_8UC3, 255, 0);
	dahazed = imadjust(dahazed, 0, 255, 0, 255);
	//imwrite("temp1.jpg", dahazed);
	namedWindow("src", 0);
	imshow("src", dahazed);
	
	double time = ((double)getTickCount() - start) / getTickFrequency();
	cout << "所用时间为：" << time << "秒" << endl;
	waitKey(0);

	return 0;
}
