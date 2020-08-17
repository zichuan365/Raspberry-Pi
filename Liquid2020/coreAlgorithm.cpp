#include"coreAlgorithm.h"




void rotatePoint(double angle, Point2d &rotate_pt, Point2d origin_pt, Point2d center_pt)
{
    double x0 = center_pt.x;
    double y0 = center_pt.y;

    double x = origin_pt.x;
    double y = origin_pt.y;

    rotate_pt.x = (x - x0) * cos(angle* PI / 180) - (y - y0) * sin(angle* PI / 180) + x0;
    rotate_pt.y = (x - x0) * sin(angle* PI / 180) + (y - y0) * cos(angle* PI / 180) + y0;
}

edgeDetect::edgeDetect()
{
}

int judgeByPixels(Mat input, int tubeWidth)
{
    for (int i = input.rows - 1; i >= 0; i--)
    {
        int temp = 0;
        for(int j = 0;j < input.cols;j++)
        {
            if (input.at<uchar>(i,j) == 255)
                temp++;
            if (temp >= 0.9*tubeWidth)
            {
                //cout<<"I:"<<i<<endl;
                return i;
            }

        }
    }
    cout<<"judgeByPixels error!"<<endl;
    return -1;
}


Mat edgeDetect::autoEraseLine(Mat input, float EraseLineIndex)
{
    if (EraseLineIndex <= 0)
    {
        cout << "minArea is too small,the threshold value should be more appropriate!" << endl;
        return input;
    }

    vector<vector<Point>> ContoursBelow;
    findContours(input, ContoursBelow, RETR_LIST, CHAIN_APPROX_NONE);
    ContoursBelow.erase(std::remove_if(ContoursBelow.begin(), ContoursBelow.end(),
        [=](const std::vector<cv::Point>& c) {return c.size() < EraseLineIndex; }), ContoursBelow.end());

    if (ContoursBelow.size() > 1)
    {
        //ContoursBelow.clear();
        return autoEraseLine(input, EraseLineIndex + 10);
    }
    else
    {
        Mat temp = Mat::zeros(input.rows, input.cols, CV_8UC1);
        drawContours(temp, ContoursBelow, -1, cv::Scalar(255), cv::FILLED);
        return temp;
    }
}

Mat edgeDetect::autoEraseLine2(Mat input, float* EraseLineIndex)
{
    if (*EraseLineIndex <= 0)
    {
        cout << "minArea is too small,the threshold value should be more appropriate!" << endl;
        return input;
    }

    vector<vector<Point>> ContoursBelow;
    findContours(input, ContoursBelow, RETR_LIST, CHAIN_APPROX_NONE);
    ContoursBelow.erase(std::remove_if(ContoursBelow.begin(), ContoursBelow.end(),
        [=](const std::vector<cv::Point>& c) {return c.size() < *EraseLineIndex; }), ContoursBelow.end());
    //cout<<" ContoursBelow.size():"<<ContoursBelow.size()<<endl;

    //ContoursBelow.size() = 1 2 返回
    if (ContoursBelow.size() <= 0)
    {
        //ContoursBelow.clear();
        return autoEraseLine2(input, EraseLineIndex - 2);
    }
    else if (ContoursBelow.size() >= 3)
    {
        //ContoursBelow.clear();
        return autoEraseLine2(input, EraseLineIndex + 2);
    }
    else
    {
        Mat temp = Mat::zeros(input.rows, input.cols, CV_8UC1);
        drawContours(temp, ContoursBelow, -1, cv::Scalar(255), cv::FILLED);
        //cout<<" ContoursBelow.size():"<<ContoursBelow.size()<<endl;

        return temp;
    }


}

//double edgeDetect::medianMat(Mat Input)
//{
//    Input = Input.reshape(0, 1);// spread Input Mat to single row，设置为一行n列
//    std::vector<double> vecFromMat;
//    Input.copyTo(vecFromMat); // Copy Input Mat to vector vecFromMat
//
//    std::nth_element(vecFromMat.begin(), vecFromMat.begin() + vecFromMat.size() / 2, vecFromMat.end());
//    //返回中间值的像素值
//    return vecFromMat[vecFromMat.size() / 2];
//}
/////提取大致轮廓，阈值越高筛选掉的线段越多/
//Mat edgeDetect::autoCanny(cv::Mat &image, cv::Mat &output, float sigma)
//{
//    //apply small amount of Gaussian blurring
//    cv::GaussianBlur(image, output, cv::Size(3, 3), 0, 0);
//
//    //get the median value of the matrix
//    double v = medianMat(output);
//    //generate the thresholds
//
//    int lower = (int)std::max(0.0, (1, 0 - sigma)*v);
//    int upper = (int)std::min(255.0, (1, 0 + sigma)*v);
//    //std::cout << lower << std::endl;
//    //std::cout << upper << std::endl;
//    //apply canny operator
//    Canny(output, output, lower, upper, 3);
//    //return output;
//}





edgeDetect::~edgeDetect()
{
}

//void edgeDetect::roiAboveBelow(Mat input, Mat& outAbove, Mat& outBelow, float aboveHeight, float BelowHeight)
//{
//    if (!input.data)
//    {
//        cout << "input image is empty!Please check!" << endl;
//        return;
//    }
//    int frameWidth = input.cols;
//    int frameHeigth = input.rows;
//    outAbove = input(Rect(0, 0,frameWidth, frameHeigth *aboveHeight));

//    outBelow = input(Rect(0,(1 - BelowHeight) * frameHeigth,
//        frameWidth , frameHeigth * BelowHeight));
//}

//bool compareSize(const vector<Point>& A, const vector<Point>& B)
//{
//    return (A.size() > B.size());
//}
bool compareRowNum(const rowAndNum& A, const rowAndNum& B)
{
    return (A.num > B.num);
}

//-----------------------------上部处理-----------------------------//
void edgeDetect::AboveProcess(Mat inputAbove,int& slpoeTemp,int begin,int end)
{
    if (!inputAbove.data)
    {
        cout << "inputAbove is empty!" << endl;
        return;
    }
    Mat dest, dest1, dest2, dest3;

    Mat kernel1 = getStructuringElement(MORPH_RECT, Size(7, 7));
    Mat kernel2 = getStructuringElement(MORPH_RECT, Size(3, 3));
    erode(inputAbove, dest1, kernel1);
    dilate(dest1, dest2, kernel2);

    dest3 = autoEraseLine(dest2,80);

    //根据图像的矩，计算图像重心以及倾斜角度
    Moments m = moments(dest1, true);
    //中心点
    Point2d center(m.m10 / m.m00, m.m01 / m.m00);
    Point2d centerLeft(m.m10 / m.m00 - 60, m.m01 / m.m00);

    double a = m.m20 / m.m00 - center.x*center.x;
    double b = m.m11 / m.m00 - center.x*center.y;
    double c = m.m02 / m.m00 - center.y*center.y;
    double theta = fastAtan2(2 * b, (a - c)) / 2;//此为形状的方向
    //cout<<"theta"<<theta<<endl;
    if(theta < 0 || theta > 180)
    {
        //cout<<"theta detect error!theta is:"<<theta<<endl;
        slpoeTemp = 0;
        return ;
    }

    int centerR = (end - begin) / 2;
    int centerPosition = begin + centerR;
    double xPosition = center.x - center.y / tan(theta*PI/180);
//    cout<<xPosition<<endl;
//    cout<<end<<endl;
//    cout<<centerR*2<<endl;

    if (xPosition <= begin + 1)
    {
        slpoeTemp = 0;
        cout << "slpoeTemp:" << slpoeTemp << endl;
//        Point2d centerRotate;
//        rotatePoint(theta,centerRotate,centerLeft,center);
//        line(dest3,center,centerRotate,Scalar(0));


//        circle(dest3,Point(begin,2),2,Scalar(255));
//        circle(dest3,Point(end,2),2,Scalar(255));
//        circle(dest3,Point(centerPosition,2),2,Scalar(255));


//        imshow("1",dest3);
//        waitKey();
//        destroyWindow("1");

        return;
    }
    if (xPosition >= end - 1)
    {
        slpoeTemp = 180;
        cout << "slpoeTemp:" << slpoeTemp << endl;
//        Point2d centerRotate;
//        rotatePoint(theta,centerRotate,centerLeft,center);
//        line(dest3,center,centerRotate,Scalar(0));


//        circle(dest3,Point(begin,2),2,Scalar(255));
//        circle(dest3,Point(end,2),2,Scalar(255));
//        circle(dest3,Point(centerPosition,2),2,Scalar(255));


//        imshow("1",dest3);
//        waitKey();
//        destroyWindow("1");
        return;
    }

    //cout <<"begin:"<< begin <<" centerR:"<< centerR <<" end:"<< end<<endl;
    //cout<<"centerPosition:"<< centerPosition <<endl;
    //cout << xPosition - centerPosition << endl;
    slpoeTemp = 90 + asin((xPosition - centerPosition)/ centerR) *(180/PI);
    cout << "slpoeTemp:" << slpoeTemp << endl;

//    Point2d centerRotate;
//    rotatePoint(theta,centerRotate,centerLeft,center);
//    line(dest3,center,centerRotate,Scalar(0));


//    circle(dest3,Point(begin,2),2,Scalar(255));
//    circle(dest3,Point(end,2),2,Scalar(255));
//    circle(dest3,Point(centerPosition,2),2,Scalar(255));


//    imshow("1",dest3);
//    waitKey();
//    destroyWindow("1");

}

//void edgeDetect::midPixelProcess(Mat input, vector<rowAndLocation>& Num)
//{
//    //计算最大像素值
//    //double minv = 0.0, maxv = 0.0;
//    //double* minp = &minv;
//    //double* maxp = &maxv;
//    //minMaxIdx(input, minp, maxp);

//    int tempNum = 0;
//    int numNum = 0;
//    //int begin1 = 0;
//    //int end1 = 0;

//    vector<rowAndLocation> num(input.rows);
//    for (int i = 0; i != input.rows; i++)
//    {
//        num[i].row = i;
//        for (int j = 1; j != input.cols; j++)
//        {
//            if (input.at<uchar>(i, j - 1) != input.at<uchar>(i, j))
//            {
//                tempNum = j;
//                num[i].location.push_back(tempNum);
//            }
//            if (input.at<uchar>(i, j) == 255)
//            {
//                numNum++;
//            }
//        }
//        num[i].numPixel255 = numNum;
//        numNum = 0;
//    }
//    //进一步筛选，筛选出最符合条件的中间点
//    for (vector<rowAndLocation>::iterator it = num.begin(); it != num.end(); it++)
//    {
//        if(((*it).location.size())> 2)
//        {
//            int groupNum = (*it).location.size() / 2;
//            vector<int>midNum(groupNum);
//            int NUM = 0;
//            for(int i = 0;i!= groupNum ;i++)
//            {
//                int begin = (*it).location[NUM++];
//                int end   = (*it).location[NUM++];
//                int MID = begin + (end - begin)/ 2;
//                (*it).midNum.push_back(MID);
//            }
//        }
//        if (((*it).location.size()) == 2)
//        {
//            int MidTemp = (((*it).location[1] - (*it).location[0]) / 2);
//            (*it).midNum.push_back ( (*it).location[0] + MidTemp  );
//        }
//        if (((*it).location.size()) == 0)
//        {
//            it = num.erase(it);
//        }
//    }


//    Num = num;
//    //cout << "11" << endl;
//}

//void edgeDetect::leastSquaresFitLine(Mat input, vector<rowAndLocation> num)
//{
//    Mat tempZero = Mat::zeros(input.rows, input.cols, CV_8UC1);
//    //vector<int>::iterator biggest;
//    //for(int i= 0;i= num.size();i++)
//    //{
//    //	biggest = std::max_element(std::begin(num[i].midNum), std::end(num[i].midNum));
//    //}
//    //cout << *biggest << endl;
//    //将点信息进行转化
//    vector<Point>points;
//    for (int i = 0; i != num.size(); i++)
//    {
//        vector<int>temp = num[i].midNum;
//        for(int j = 0 ;j!=temp.size();j++)
//        {
//            Point tempPoint(temp[j], num[i].row);
//            points.push_back(tempPoint);
//        }
//    }

//    //for (int i = 0; i != num.size(); i++)
//    //{
//    //	vector<int>temp = num[i].midNum;
//    //	if(temp.size() == 1)
//    //	{
//    //		Point tempPoint(temp[0], num[i].row);
//    //		points.push_back(tempPoint);
//    //	}
//    //}

//    for (int i = 0; i < points.size(); i++)
//    {
//        //在原图上画出点
//        tempZero.at<uchar>(points[i].y, points[i].x) = 255;
//    }

//    imshow("111", tempZero);
//    waitKey();
//    //aX+bY=0,返回a，b以及直线上一点
//    Vec4f line_para;
//    fitLine(points, line_para, CV_DIST_L2, 0, 0.01, 0.01);

//    cv::Point point0;

//    point0.x = line_para[2];
//    point0.y = line_para[3];
//    double k = line_para[1] / line_para[0];
//    cout <<"K :"<< -k << endl;
//    double Slope = -k;
//    if(Slope <= 13&& Slope>=0)
//    {
//        cout << "试管在右侧!" << endl;
//    }
//    else if(Slope >= -13 && Slope <= 0)
//    {
//        cout << "试管在左侧!" << endl;
//    }
//    else
//    {
//        cout << "试管在中央!" << endl;
//    }

//    cv::Point point1, point2;
//    point1.x = 0;
//    point1.y = k * (0 - point0.x) + point0.y;
//    point2.x = input.cols;
//    point2.y = k * (input.cols - point0.x) + point0.y;

//    cv::line(input, point1, point2, cv::Scalar(0), 1, 8, 0);
//    imshow("Line", input);
//    waitKey();
//    destroyWindow("Line");
//    //最小二乘法拟合
//    /*
//    //构建A矩阵
//    int N = 2;
//    Mat A = Mat::zeros(N, N, CV_64FC1);
//    for (int row = 0; row < A.rows; row++)
//    {
//        for (int col = 0; col < A.cols; col++)
//        {
//            for (int k = 0; k < points.size(); k++)
//            {
//                A.at<double>(row, col) = A.at<double>(row, col) + pow(points[k].x, row + col);
//            }
//        }
//    }
//    //构建B矩阵
//    Mat B = Mat::zeros(N, 1, CV_64FC1);
//    for (int row = 0; row < B.rows; row++)
//    {
//        for (int k = 0; k < points.size(); k++)
//        {
//            B.at<double>(row, 0) = B.at<double>(row, 0) + pow(points[k].x, row)*points[k].y;
//        }
//    }
//    //A*X=B，计算矩阵X
//    Mat X;
//    solve(A, B, X, DECOMP_LU);

//    vector<Point>lines;
//    //cout << input.size().width << endl;
//    for (int x = 0; x < input.cols; x++)
//    {
//        // y = b + ax;
//        double y = X.at<double>(0, 0) + X.at<double>(1, 0)*x;
//        lines.push_back(Point(x, y));
//    }
//    int y1 = 0;
//    double x1 = (y1 - X.at<double>(0, 0)) / X.at<double>(1, 0);
//    cout << x1 << "   " << y1 << endl;
//    int y2 = input.rows;
//    double x2 = (y2 - X.at<double>(0, 0)) / X.at<double>(1, 0);
//    cout << x2 << "   " << y2 << endl;

//    double kk = -input.rows / (x2 - x1);
//    cout << "斜率K :"<< kk << endl;
//    polylines(input, lines, false, Scalar(0), 1, 8);
//    Mat ddd = input;
//    imshow("Line", input);
//    waitKey();
//    destroyWindow("Line");
//    */
//}


//-----------------------------下部处理-----------------------------//
void edgeDetect::belowProcess(Mat inputBelow, Mat templateBelow,int tubeWidth,double& liquidHigh)
{

    if(!inputBelow.data || !templateBelow.data)
    {
        cout << "inputBelow is empty!" << endl;
        liquidHigh = 0;
        return;
    }
    //为了更好的提取液面，将图像倒置进行处理
    double tubeWidthMM = 14.5; //14.5mm
    double perPixelHigh = tubeWidthMM / tubeWidth;

    Mat input, inputTemplate;
    flip(inputBelow, input, 0);
    flip(templateBelow, inputTemplate, 0);

    //进行模板匹配，得出液面锥部位置
    //模板的宽度即试管宽度
    int rowHigh = 0;
    templateMatch(input, inputTemplate, rowHigh);
    //cout << "rowAbove :" << rowHigh << endl;

    //进行形态学处理
    Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
    Mat dest, dest1, dest2, dest3;
    dilate(input, dest, kernel);
    erode(dest, dest, kernel);
    //阈值化处理，筛出无效区域
    //threshold(dest1, dest2, 254, 255, CV_THRESH_BINARY);

    //将上模板区域剔除，rowHigh上尖头
    int AboveHigh = rowHigh + 1.2*templateBelow.rows;
    if(AboveHigh >=0.8*dest.rows)
    {
        cout<<"ROI area is error!"<<endl;
        liquidHigh = 0;
        return;
    }


    Rect Area(0, AboveHigh, inputBelow.cols, inputBelow.rows - AboveHigh);
    dest1 = dest(Area);

    //提取梯度信息
    Scharr(dest1, dest2, CV_8U, 0, 1, 1, 0 ,BORDER_DEFAULT);

    //删除过小区域
    //vector<vector<Point>> ContoursBelow;
    //findContours(dest2, ContoursBelow, RETR_LIST, CHAIN_APPROX_NONE);
    //ContoursBelow.erase(std::remove_if(ContoursBelow.begin(), ContoursBelow.end(),
    //	[=](const std::vector<cv::Point>& c) {return c.size() < 15; }), ContoursBelow.end());
    //Mat tempImage = Mat::zeros(dest2.rows, dest2.cols, CV_8UC1);
    //drawContours(tempImage, ContoursBelow, -1, cv::Scalar(255), cv::FILLED);
    Mat kernel2 = getStructuringElement(MORPH_RECT, Size(5, 1));
    Mat tempImage;
    erode(dest2, tempImage, kernel2);

//    imshow("效果图", tempImage);
//    waitKey();
//    destroyWindow("效果图");

    vector<vector<Point>> Contours;
    findContours(tempImage, Contours, RETR_LIST, CHAIN_APPROX_NONE);

    int tempHigh = 0;
    if(Contours.size() == 0)
    {
        cout << "No Scharr information!!!" << endl;
        //直接根据像素值判断液面高度
        tempHigh = judgeByPixels(dest1, tubeWidth);
        if(tempHigh == -1){
            liquidHigh = 0;
            cout << "Liquid high：" << 0<< endl;
            return;
        }

        liquidHigh = (tempHigh + 0.2*templateBelow.rows)*perPixelHigh;
        liquidHigh = static_cast<int>(liquidHigh*10);
        liquidHigh = liquidHigh/10;
        cout << "Liquid high：" << liquidHigh << endl;


//        rectangle(input, Point(0, rowHigh),
//        Point(inputBelow.cols, rowHigh + templateBelow.rows+ (tempHigh + 0.2*templateBelow.rows)), cv::Scalar(255), 1, 8);
//        imshow("效果图", input);
//        waitKey();
//        destroyWindow("效果图");
        return;
    }

    //找到最可能的两个区域
    float temp = 10;
    float* tempErase= &temp;
    dest2 = edgeDetect::autoEraseLine2(tempImage,tempErase);

    segmentMatch(dest2,dest1, tubeWidth, tempHigh);
    if(tempHigh == -1){
        liquidHigh = 0;
        cout << "Liquid high：" << 0 << endl;
        return;
    }

    liquidHigh = (tempHigh + 0.2*templateBelow.rows)* perPixelHigh;
    liquidHigh = static_cast<int>(liquidHigh*10);
    liquidHigh = liquidHigh/10;
    //然后根据同分割图片的匹配情况确定最后的液面高度
    //int liquidAbove = rowHigh - 1.4 * templateBelow.rows;

    ////输入：1.分割后图像 2.分割后梯度信息 3.分割后图像Y位置AboveHigh
    /// 输出：下液面高度
    //int rowBegin = 0;
    //segmentMatch(input(Area), dest3 , AboveHigh ,rowBegin);



//    rectangle(input, Point(0, rowHigh),
//        Point(inputBelow.cols, rowHigh + templateBelow.rows + (tempHigh + 0.2*templateBelow.rows)), cv::Scalar(255), 1, 8);
//    imshow("效果图", input);
//    waitKey();
//    destroyWindow("效果图");
    cout << "Liquid high：" << liquidHigh << endl;
}

void edgeDetect::segmentMatch(Mat Scharr,Mat dest1,int tubeWidth, int& tempHigh)
{
    //记录初始轮廓线的row位置
    vector<int>rowLocation;
    for(int i=0; i!= Scharr.rows;i++)
    {
        for(int j=0;j!= Scharr.cols;j++)
        {
            if(Scharr.at<uchar>(i,j)==255)
            {
                rowLocation.push_back(i);
                break;
            }
        }
    }

    int temp = 0;
    vector<rowAndNum>rowNum(rowLocation.size());
    for(int i = 0;i!= rowLocation.size();i++)
    {
        int rowLine = rowLocation[i];
        for(int j = 0; j != dest1.cols; j++)
        {
            if(dest1.at<uchar>(rowLine, j) == 255)
            {
                temp++;
            }
        }
        rowNum[i].row = rowLine;
        rowNum[i].num = temp;
        temp = 0;
    }

    sort(rowNum.begin(), rowNum.end(), compareRowNum);
    //cout << "111" << endl;
    if (rowNum[0].num >= 0.8*tubeWidth)
    {
        tempHigh = rowNum[0].row;
        //cout << "1" << endl;
    }
    else
    {
        tempHigh = judgeByPixels(dest1, tubeWidth);
        //cout << "2" << endl;
    }
}

void edgeDetect::templateMatch(Mat input1, Mat input2, int& rowEnd)
{
    Mat result = Mat::zeros(input1.rows, input1.cols, input1.type());
    matchTemplate(input1, input2, result, TM_CCOEFF_NORMED);

    normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

    Point minPoint;
    Point maxPoint;
    double *minVal = 0;
    double *maxVal = 0;
    minMaxLoc(result, minVal, maxVal, &minPoint, &maxPoint);

    //cv::rectangle(input1, maxPoint, cv::Point(maxPoint.x + input2.cols, maxPoint.y + input2.rows), cv::Scalar(255), 1, 8);
    //cv::imshow("【匹配后的图像】", input1);
    //waitKey();
    //destroyWindow("【匹配后的图像】");

    rowEnd = maxPoint.y;
    //上端点
    //cout << "11" << endl;
}
