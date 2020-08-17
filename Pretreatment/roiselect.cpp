#include "roiselect.h"

Mat trackbarImg;
int trackbarX = 100;
int trackbarY = 100;
int trackbarWidth = 100;
int trackbarHeight = 100;

void onTrackbar(int, void*)
{
    Point begin(trackbarX, trackbarY);
    Mat trackbarImgTemp = trackbarImg.clone();
    rectangle(trackbarImgTemp, begin, Point(begin.x + trackbarWidth, begin.y + trackbarHeight), Scalar(255), 1, 1);

    imshow("ROISelect", trackbarImgTemp);
    waitKey(30);
}
void onTrackbarTwo(int, void*)
{
    Point begin(trackbarX, trackbarY);
    Mat trackbarImgTemp = trackbarImg.clone();
    rectangle(trackbarImgTemp, begin, Point(begin.x + trackbarWidth, begin.y + trackbarHeight), Scalar(255), 2, 1);

    imshow("ROISelect", trackbarImgTemp);
    waitKey(30);
}

void roiSelect(Mat input, int& X, int& Y, int& width, int& height)
{
    trackbarImg = input;
    namedWindow("ROISelect", 0);
    string xName = "X: " + to_string(input.cols);
    string yName = "Y: " + to_string(input.rows);
    string widthName = "width: " + to_string(input.cols/2);
    string heightName = "height: " + to_string(input.rows/2);

    createTrackbar(xName, "ROISelect", &trackbarX, input.cols, onTrackbarTwo);
    createTrackbar(yName, "ROISelect", &trackbarY, input.rows, onTrackbarTwo);
    createTrackbar(widthName, "ROISelect", &trackbarWidth, input.cols / 2, onTrackbarTwo);
    createTrackbar(heightName, "ROISelect", &trackbarHeight, input.rows / 2, onTrackbarTwo);

    waitKey();
    X = trackbarX;
    Y = trackbarY;
    width = trackbarWidth;
    height = trackbarHeight;
    destroyWindow("ROISelect");
    //output = input(Rect(X, Y, width, height));
}
void roiSelect(Mat input, Mat& output)
{
    trackbarImg = input;
    namedWindow("ROISelect", 0);
    string xName = "X: " + to_string(input.cols);
    string yName = "Y: " + to_string(input.rows);
    string widthName = "width: " + to_string(input.cols);
    string heightName = "height: " + to_string(input.rows / 2);

    createTrackbar(xName, "ROISelect", &trackbarX, input.cols, onTrackbar);
    createTrackbar(yName, "ROISelect", &trackbarY, input.rows, onTrackbar);
    createTrackbar(widthName, "ROISelect", &trackbarWidth, input.cols , onTrackbar);
    createTrackbar(heightName, "ROISelect", &trackbarHeight, input.rows / 2, onTrackbar);

    waitKey();
    output = input(Rect(trackbarX, trackbarY, trackbarWidth, trackbarHeight));
    destroyWindow("ROISelect");
}


void AdjustFocus()
{
    Mat frame;
    raspicam::RaspiCam_Cv cam;
    cam.set(CV_CAP_PROP_FORMAT, CV_8UC3);
    cam.set(CV_CAP_PROP_EXPOSURE,-1);
    //cam.set(CV_CAP_PROP_GAIN,60);
    cam.set(cv::CAP_PROP_FRAME_WIDTH,2592);
    cam.set(cv::CAP_PROP_FRAME_HEIGHT,1944);
    cam.open();
    waitKey(2000);
    namedWindow("AdjustFocus",0);
    cout<<"在实时的显示窗口按下[q]，退出视频流。"<<endl;
    while(1)
    {
        cam.grab();
        cam.retrieve(frame);
        imshow("AdjustFocus",frame);
        waitKey(30);
        if(char(waitKey(300)=='q'))
        {
            break;
        }
    }
    cam.release();
    destroyAllWindows();
    cout<<"Cam is release!"<<endl;
}

void SelectRoi()
{
    cout<<"在实时的显示窗口按下[ENTER]，显示下一张图像处理效果。"<<endl;
    //找到ROI区域,计算试管宽度，开始起点和终点
    Mat frame,frameROI,temp;
    raspicam::RaspiCam_Cv cam;
    cam.set(CV_CAP_PROP_FORMAT, CV_8UC3);
    cam.set(CV_CAP_PROP_EXPOSURE,-1);
    //cam.set(CV_CAP_PROP_GAIN,60);
    cam.set(cv::CAP_PROP_FRAME_WIDTH,2592);
    cam.set(cv::CAP_PROP_FRAME_HEIGHT,1944);
    cam.open();
    waitKey(5000);
    cam.grab();
    cam.retrieve(frame);
    cam.grab();
    cam.retrieve(frame);
    cam.grab();
    cam.retrieve(frame);
    cam.grab();
    cam.retrieve(frame);

    int roiX = 0;
    int roiY = 0;
    int roiW = 0;
    int roiH = 0;

    int begin = 0;
    int end = 0;

    int tubeWidth = 0;
    roiSelect(frame,roiX,roiY,roiW,roiH);

    unordered_map<int,int>num1;
    unordered_map<int,int>num2;

    for(int i =0;i<30;i++){
        cam.grab();
        cam.retrieve(frame);
        int temp1 = 0;
        int temp2 = 0;
        frameROI = frame(Rect(roiX,roiY,roiW,roiH));
        temp = Kmeans(frameROI,temp1,temp2);
        //imshow("1",temp);
        //waitKey();
        num1[temp1]++;
        num2[temp2]++;
    }
    cam.release();
    destroyAllWindows();
    int tempBegin = maxValue(num1);
    int tempEnd = maxValue(num2);
    tubeWidth = tempEnd-tempBegin;

    roiX = roiX + tempBegin -10;
    roiW = tubeWidth + 20;

    begin = 10 + 0.12*tubeWidth;
    end = 10 + tubeWidth - 0.12*tubeWidth;;
    FileStorage fs("/home/pi/RoiArea.xml", FileStorage::WRITE);
    fs << "X" << roiX;
    fs << "Y" << roiY;
    fs << "Width" << roiW;
    fs << "Height" << roiH;
    fs << "begin" << begin;
    fs << "end" << end;
    fs << "tubeWidth" << tubeWidth;
    fs.release();
}
void MakeTemplate()
{
    //制作模板
    int roiX = 0;
    int roiY = 0;
    int roiW = 0;
    int roiH = 0;

    FileStorage fs("/home/pi/RoiArea.xml", FileStorage::READ);
    fs["X"] >> roiX;
    fs["Y"] >> roiY;
    fs["Width"] >> roiW;
    fs["Height"] >> roiH;
    fs.release();
    cout<<"在实时的显示窗口按下[q]，进入模板制作，框选指定模板区域"<<endl;
    Mat frame,ROISeg,temp;
    Mat templateImg;
    raspicam::RaspiCam_Cv cam;
    cam.set( CV_CAP_PROP_FORMAT, CV_8UC1);
    cam.set(CV_CAP_PROP_EXPOSURE,-1);
    cam.set(cv::CAP_PROP_FRAME_WIDTH,2592);
    cam.set(cv::CAP_PROP_FRAME_HEIGHT,1944);
    cam.open();
    waitKey(5000);
    namedWindow("1",0);


    while(1){
        cam.grab();
        cam.retrieve(frame);
        temp = frame(Rect(roiX,roiY,roiW,roiH));
        ROISeg = otsu(temp);

        //ROISeg = temp(Rect( roiX, roiY,roiW,roiH));
        imshow("1",ROISeg);
        waitKey(20);
        if(char(waitKey(300)=='q')){
            roiSelect(ROISeg,templateImg);
            imwrite("/home/pi/template.bmp", templateImg);
            break;
        }
    }
    destroyAllWindows();
    cam.release();
}

void Testliquid()
{
    int xx = 0;
    int yy= 0;
    int WW = 0;
    int HH = 0;
    int begin = 0;
    int end = 0;
    int tubeWidth = 0;

    FileStorage fs("/home/pi/RoiArea.xml", FileStorage::READ);
    fs["X"] >> xx;
    fs["Y"] >> yy;
    fs["Width"] >> WW;
    fs["Height"] >> HH;
    fs["begin"] >> begin;
    fs["end"] >> end;
    fs["tubeWidth"] >> tubeWidth;
    fs.release();

    Mat templateImg=imread("/home/pi/template.bmp",0);

    Mat frame,temp,roi1,roi2;
    double theta, liquidHigh;

    raspicam::RaspiCam_Cv cam;
    cam.set( CV_CAP_PROP_FORMAT, CV_8UC1);
    cam.set(CV_CAP_PROP_EXPOSURE,-1);
    cam.set(cv::CAP_PROP_FRAME_WIDTH,2592);
    cam.set(cv::CAP_PROP_FRAME_HEIGHT,1944);
    cam.open();
    waitKey(5000);

    namedWindow("1",0);
    for(int i =0;;i++)
    {
        cam.grab();
        cam.retrieve(frame);
        //char chKey = getchar();

        temp = frame(Rect(xx,yy,WW,HH));
        roi1 = otsu(temp);
//        imshow("1",roi1);
//        waitKey();
//        destroyWindow("1");

        edgeDetect::AboveProcess(roi1,theta,begin,end);
        edgeDetect::belowProcess(roi1,templateImg,tubeWidth,liquidHigh);

    }

    cam.release();

}

