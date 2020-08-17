#pragma once
#include "histSeg.h"
#include <sstream>
#include <iomanip>

#define PI 3.1415926

using namespace cv;
using namespace std;

struct rowAndNum
{
    int row;
    int num;
};

int judgeByPixels(Mat input,int tubeWidth);

bool compareRowNum(const rowAndNum& A, const rowAndNum& B);

class edgeDetect
{
public:
    edgeDetect();
    ~edgeDetect();

    //-----------------------------上部处理-----------------------------//
    static void AboveProcess(Mat inputAbove,double& slope,int begin,int end);

    //-----------------------------下部处理-----------------------------//
    //输入分割完成后的图像
    static void belowProcess(Mat inputBelow, Mat templateBelow, int tubeWidth,double& liquidHigh);

    //将下部分区域进行模板匹配
    static void templateMatch(Mat input1, Mat input2, int& rowEnd);
    //梯度信息验证
    static void segmentMatch(Mat Scharr, Mat dest1, int tubeWidth, int& tempHigh);


    //-----------------------------再处理-----------------------------//
    //删除过小区域
    static Mat autoEraseLine(Mat input, float EraseLineIndex);
    static Mat autoEraseLine2(Mat input, float* EraseLineIndex);

    ////阈值处理提取轮廓
    //static double medianMat(Mat Input);
    //static Mat autoCanny(Mat &image, Mat &output, float sigma = 0.33);

};
