#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H
#include "coreAlgorithm.h"

//step1:选取指定的ROI区域，并计算使馆宽度
//step2:试管下部分模板的制作
//step3:进行图像处理，1.返回液面高度 2.试管的朝向，具体角度
//需要指定ROI区域



Mat temoplateImg=imread("/home/pi/template.bmp",0);

struct result
{
    double liquidHigh;
    double tubeAngle;
};

//结果函数
result calculateReslut(cv::Mat& img);
#endif // IMAGEPROCESS_H
