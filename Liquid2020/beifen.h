////定义变量
////滑动条变量
//Mat trackbarImg;
//int trackbarX = 100;
//int trackbarY = 100;
//int trackbarWidth = 100;
//int trackbarHeight = 100;

//edgeDetect::edgeDetect()
//{
//}


//Mat edgeDetect::autoEraseLine(Mat input, float EraseLineIndex)
//{
//    if (EraseLineIndex <= 0)
//    {
//        cout << "minArea is too small,the threshold value should be more appropriate!" << endl;
//        return input;
//    }

//    vector<vector<Point>> ContoursBelow;
//    findContours(input, ContoursBelow, RETR_LIST, CHAIN_APPROX_NONE);
//    ContoursBelow.erase(std::remove_if(ContoursBelow.begin(), ContoursBelow.end(),
//        [=](const std::vector<cv::Point>& c) {return c.size() < EraseLineIndex; }), ContoursBelow.end());

//    if (ContoursBelow.size() > 1)
//    {
//        //ContoursBelow.clear();
//        return autoEraseLine(input, EraseLineIndex + 10);
//    }
//    else
//    {
//        Mat temp = Mat::zeros(input.rows, input.cols, CV_8UC1);
//        drawContours(temp, ContoursBelow, -1, cv::Scalar(255), cv::FILLED);
//        return temp;
//    }
//}

//Mat edgeDetect::autoEraseLine2(Mat input, float EraseLineIndex)
//{
//    if (EraseLineIndex <= 0)
//    {
//        cout << "minArea is too small,the threshold value should be more appropriate!" << endl;
//        return input;
//    }

//    vector<vector<Point>> ContoursBelow;
//    findContours(input, ContoursBelow, RETR_LIST, CHAIN_APPROX_NONE);
//    ContoursBelow.erase(std::remove_if(ContoursBelow.begin(), ContoursBelow.end(),
//        [=](const std::vector<cv::Point>& c) {return c.size() < EraseLineIndex; }), ContoursBelow.end());

//    if (ContoursBelow.size() <= 0)
//    {
//        //ContoursBelow.clear();
//        return autoEraseLine(input, EraseLineIndex - 5);
//    }
//    else if (ContoursBelow.size() >= 3)
//    {
//        //ContoursBelow.clear();
//        return autoEraseLine(input, EraseLineIndex + 5);
//    }
//    else
//    {
//        Mat temp = Mat::zeros(input.rows, input.cols, CV_8UC1);
//        drawContours(temp, ContoursBelow, -1, cv::Scalar(255), cv::FILLED);
//        return temp;
//    }


//}

//double edgeDetect::medianMat(Mat Input)
//{
//    Input = Input.reshape(0, 1);// spread Input Mat to single row，设置为一行n列
//    std::vector<double> vecFromMat;
//    Input.copyTo(vecFromMat); // Copy Input Mat to vector vecFromMat

//    std::nth_element(vecFromMat.begin(), vecFromMat.begin() + vecFromMat.size() / 2, vecFromMat.end());
//    //返回中间值的像素值
//    return vecFromMat[vecFromMat.size() / 2];
//}
/////提取大致轮廓，阈值越高筛选掉的线段越多/
//Mat edgeDetect::autoCanny(cv::Mat &image, cv::Mat &output, float sigma)
//{
//    //apply small amount of Gaussian blurring
//    cv::GaussianBlur(image, output, cv::Size(3, 3), 0, 0);

//    //get the median value of the matrix
//    double v = medianMat(output);
//    //generate the thresholds

//    int lower = (int)std::max(0.0, (1, 0 - sigma)*v);
//    int upper = (int)std::min(255.0, (1, 0 + sigma)*v);
//    //std::cout << lower << std::endl;
//    //std::cout << upper << std::endl;
//    //apply canny operator
//    Canny(output, output, lower, upper, 3);
//    return output;
//}

//edgeDetect::~edgeDetect()
//{
//}

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

////bool compareSize(const vector<Point>& A, const vector<Point>& B)
////{
////    return (A.size() > B.size());
////}
//bool compareRowNum(const rowAndNum& A, const rowAndNum& B)
//{
//    return (A.num > B.num);
//}


//void onTrackbar(int, void*)
//{
//    Point begin(trackbarX, trackbarY);
//    Mat trackbarImgTemp = trackbarImg.clone();
//    rectangle(trackbarImgTemp, begin, Point(begin.x + trackbarWidth, begin.y + trackbarHeight), Scalar(255), 2, 8);

//    imshow("ROISelect", trackbarImgTemp);
//    waitKey(30);
//}

//void roiSelect(Mat input, int& X, int& Y, int& width, int& height)
//{
//    trackbarImg = input;
//    namedWindow("ROISelect", 0);
//    string xName = "X: " + to_string(input.cols);
//    string yName = "Y: " + to_string(input.rows);
//    string widthName = "width: " + to_string(input.cols/2);
//    string heightName = "height: " + to_string(input.rows/2);

//    createTrackbar(xName, "ROISelect", &trackbarX, input.cols, onTrackbar);
//    createTrackbar(yName, "ROISelect", &trackbarY, input.rows, onTrackbar);
//    createTrackbar(widthName, "ROISelect", &trackbarWidth, input.cols / 2, onTrackbar);
//    createTrackbar(heightName, "ROISelect", &trackbarHeight, input.rows / 2, onTrackbar);

//    waitKey();
//    X = trackbarX;
//    X = trackbarY;
//    width = trackbarWidth;
//    height = trackbarHeight;
//    destroyWindow("ROISelect");
//    //output = input(Rect(X, Y, width, height));
//}
//void roiSelect(Mat input, Mat& output)
//{
//    trackbarImg = input;
//    namedWindow("ROISelect", 0);
//    string xName = "X: " + to_string(input.cols);
//    string yName = "Y: " + to_string(input.rows);
//    string widthName = "width: " + to_string(input.cols / 2);
//    string heightName = "height: " + to_string(input.rows / 2);

//    createTrackbar(xName, "ROISelect", &trackbarX, input.cols, onTrackbar);
//    createTrackbar(yName, "ROISelect", &trackbarY, input.rows, onTrackbar);
//    createTrackbar(widthName, "ROISelect", &trackbarWidth, input.cols / 2, onTrackbar);
//    createTrackbar(heightName, "ROISelect", &trackbarHeight, input.rows / 2, onTrackbar);

//    waitKey();
//    output = input(Rect(trackbarX, trackbarY, trackbarWidth, trackbarHeight));
//    destroyWindow("ROISelect");
//}

////-----------------------------上部处理-----------------------------//
//void edgeDetect::AboveProcess(Mat inputAbove,double& slope)
//{
//    if (!inputAbove.data)
//    {
//        cout << "inputAbove is empty!" << endl;
//        return;
//    }
//    Mat dest, dest1, dest2, dest3;
//    //计算试管宽度
//    edgeDetect::autoCanny(inputAbove, dest);
//    int timeNum = 0;
//    int begin = 0;
//    int end = 0;
//    for (int i = 0; i != dest.cols; i++)
//    {
//        if (dest.at<uchar>(0, i) == 255)
//        {
//            ++timeNum;
//            if (timeNum == 1)
//            {
//                begin = i;
//            }
//            end = i;
//        }
//    }
//    int tubeWidth = end - begin;
//    cout << "tubeWidth :" << tubeWidth << endl;

//    //////将输入区域处理为一个整体
//    double minv = 0.0, maxv = 0.0;
//    double* minp = &minv;
//    double* maxp = &maxv;
//    minMaxIdx(inputAbove, minp, maxp);

//    threshold(inputAbove, dest, *maxp-5, 255, CV_THRESH_BINARY);

//    double temp255Num=0;
//    for(int i=0;i!=inputAbove.rows;i++)
//    {
//        for(int j=0;j!=inputAbove.cols;j++)
//        {
//            if(dest.at<uchar>(i,j)==255)
//            {
//                temp255Num++;
//            }
//        }
//    }

//    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
//    if(temp255Num/(inputAbove.rows*inputAbove.rows)>0.28)
//    {
//        erode(dest, dest1, kernel);
//        dilate(dest1, dest1, kernel);

//    }
//    else
//    {
//        dilate(dest, dest1, kernel);
//        erode(dest1, dest1, kernel);
//        erode(dest, dest1, kernel);
//        dilate(dest1, dest1, kernel);
//    }


//    dest2 = edgeDetect::autoEraseLine(dest1, 100);



//    namedWindow("11",0);
//    imshow("11",dest2);
//    waitKey();
//    destroyWindow("11");



//    //计算像素中位点
//    vector<rowAndNum> Num;
//    midPixelProcess(dest2, Num);

//    //根据像素中为点拟合直线

//    leastSquaresFitLine(dest2, Num, slope);
//    //cout << "11" << endl;
//}

//void edgeDetect::midPixelProcess(Mat input, vector<rowAndNum>& Num)
//{
//    //计算最大像素值
//    //double minv = 0.0, maxv = 0.0;
//    //double* minp = &minv;
//    //double* maxp = &maxv;
//    //minMaxIdx(input, minp, maxp);

//    int tempNum = 0;
//    int begin1 = 0;
//    int end1 = 0;

//    vector<rowAndNum> num(input.rows);
//    for (int i = 0; i != input.rows; i++)
//    {
//        num[i].row = i;
//        for (int j = 0; j != input.cols; j++)
//        {

//            if (input.at<uchar>(i, j) == 255)
//            {
//                ++tempNum;
//                end1 = j;
//                if (tempNum == 1)
//                {
//                    begin1 = j;
//                }
//            }
//        }
//        num[i].num = tempNum;
//        num[i].midNum = begin1 + (end1 - begin1) / 2;
//        tempNum = 0;
//    }
//    Num = num;
//}

//void edgeDetect::leastSquaresFitLine(Mat input, vector<rowAndNum> num,double& Slope )
//{
//    Mat tempZero = Mat::zeros(input.rows, input.cols, CV_8UC1);

//    //将点信息进行转化
//    vector<Point>points;
//    for (int i = 0; i != num.size(); i++)
//    {
//        Point temp(num[i].midNum, num[i].row);
//        points.push_back(temp);
//    }
//    for (int i = 0; i < tempZero.rows; i++)
//    {
//        //在原图上画出点
//        tempZero.at<uchar>(i, num[i].midNum) = 255;
//    }
//    Vec4f line_para;
//    fitLine(points, line_para, CV_DIST_L2, 0, 0.01, 0.01);

//    cv::Point point0;

//    point0.x = line_para[2];
//    point0.y = line_para[3];
//    double k = line_para[1] / line_para[0];
//    cout <<"K :"<< -k << endl;
//    Slope = -k;

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


////-----------------------------下部处理-----------------------------//
//void edgeDetect::belowProcess(Mat inputBelow, Mat templateBelow,double& liquidHigh)
//{
//    if(!inputBelow.data && !templateBelow.data)
//    {
//        cout << "inputBelow is empty!" << endl;
//        return;
//    }
//    //为了更好的提取液面，将图像倒置进行处理
//    Mat input, inputTemplate;
//    flip(inputBelow, input, 0);
//    flip(templateBelow, inputTemplate, 0);

//    //进行模板匹配，得出液面锥部位置
//    int rowHigh = 0;
//    templateMatch(input, inputTemplate, rowHigh);
//    cout << "rowAbove :" << rowHigh << endl;

//    //进行形态学处理
//    Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
//    Mat dest, dest1, dest2, dest3;
//    erode(input, dest, kernel);
//    dilate(dest, dest, kernel);

//    /*需要完善
//    threshold(dest1, dest2, 252, 255, CV_THRESH_BINARY);
//    adaptiveThreshold(dest1, dest2, 255, ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, 3, 3);
//    */
//    //提取梯度信息
//    Scharr(dest, dest1, CV_8U, 0, 1, 1, 0 ,BORDER_DEFAULT);

//    //这里可能存在问题
//    //阈值化处理，筛出无效区域
//    threshold(dest1, dest2, 254, 255, CV_THRESH_BINARY);

//    //至少保证存在2条梯度直线即可
//    //初步提取出液面位置线

//    int AboveHigh = rowHigh + 1.15*templateBelow.rows;
//    Rect Area(0, AboveHigh ,inputBelow.cols, inputBelow.rows - AboveHigh);
//    dest3 = edgeDetect::autoEraseLine2(dest2(Area), 20);

//    vector<vector<Point>> Contours;
//    findContours(dest3, Contours, RETR_LIST, CHAIN_APPROX_NONE);
//    if(Contours.size()==0)
//    {
//        cout << "detect liquid level is error!Find no counter!" << endl;
//        return;
//    }

//    //然后根据同分割图片的匹配情况确定最后的液面高度
//    int liquidAbove = rowHigh - 1.4 * templateBelow.rows;

//    int rowBegin = 0;
//    segmentMatch(input(Area), dest3 , AboveHigh ,rowBegin);

//    cout << "Liquid high：" << rowBegin - rowHigh << endl;
//    rectangle(input, Point(0, rowHigh), Point(inputBelow.cols, rowBegin), cv::Scalar(255), 1, 8);
//    imshow("效果图", input);
//    waitKey();
//    destroyWindow("效果图");
//}

//void edgeDetect::segmentMatch(Mat inputSeg, Mat inputScharr, int liquidDelete, int& rowBegin)
//{
//    //记录初始轮廓线的row位置
//    vector<int>rowLocation;
//    for(int i=0; i!= inputScharr.rows;i++)
//    {
//        for(int j=0;j!= inputScharr.cols;j++)
//        {
//            if(inputScharr.at<uchar>(i,j)==255)
//            {
//                rowLocation.push_back(i);
//                break;
//            }
//        }
//    }
//    //记录其对应分割图像

//    double minv = 0.0, maxv = 0.0;
//    double* minp = &minv;
//    double* maxp = &maxv;
//    minMaxIdx(inputSeg, minp, maxp);
//    //cout << *maxp << endl;

//    int temp = 0;
//    vector<rowAndNum>rowNum(rowLocation.size());
//    for(int i = 0;i!= rowLocation.size();i++)
//    {
//        int rowLine = rowLocation[i];
//        for(int j = 0; j != inputScharr.cols; j++)
//        {
//            if(inputSeg.at<uchar>(rowLine, j) == (*maxp))
//            {
//                temp++;
//            }
//        }
//        rowNum[i].row = rowLine;
//        rowNum[i].num = temp;
//        temp = 0;
//    }
//    ////液面高度约为圆锥高低的0.35倍
//    //rowNum.erase(std::remove_if(rowNum.begin(), rowNum.end(),
//    //	[=](const rowAndNum& c) {return c.row >= liquidAbove; }), rowNum.end());

//    sort(rowNum.begin(), rowNum.end(), compareRowNum);
//    //cout << "111" << endl;
//    rowBegin = rowNum[0].row + liquidDelete;
//}

//void edgeDetect::templateMatch(Mat input1, Mat input2, int& rowEnd)
//{
//    Mat result = Mat::zeros(input1.rows, input1.cols, input1.type());
//    matchTemplate(input1, input2, result, TM_CCOEFF_NORMED);

//    normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

//    Point minPoint;
//    Point maxPoint;
//    double *minVal = 0;
//    double *maxVal = 0;
//    minMaxLoc(result, minVal, maxVal, &minPoint, &maxPoint);

//    //cv::rectangle(input1, maxPoint, cv::Point(maxPoint.x + input2.cols, maxPoint.y + input2.rows), cv::Scalar(255), 1, 8);
//    //cv::imshow("【匹配后的图像】", input1);
//    //waitKey();
//    //destroyWindow("【匹配后的图像】");

//    rowEnd = maxPoint.y;
//}
