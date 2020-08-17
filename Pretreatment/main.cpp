#include <iostream>
#include <roiselect.h>
#include <raspicam/raspicam_cv.h>
using namespace std;
void intToHex(int dec,unsigned char* hex,int length){

    int i = 0;
    for(int i = length -1;i>0;i--){
        hex[i] = (dec%256)&0xFF;
        dec /= 256;
    }

}
int main()
{ 
    cout<<"·-------------预处理流程--------------·"<<endl;
    cout<<"[1]根据视频流手动调整相机焦距"<<endl;
    cout<<"[2]焦距调整后，选取测试ROI区域"<<endl;
    cout<<"[3]根据ROI区域，制作相应的匹配模板"<<endl;

    bool run = true;
    while(run)
    {
        cout<<"·-------------预处理选择--------------·"<<endl;
        cout<<"·--------------请按流程---------------·"<<endl;
        cout<<"Step1：输入字母[q]实时显示视频流，再次按下[q]退出视频流"<<endl;
        cout<<"Step2：输入字母[w]手动选取ROI区域，请按照指定要求选取"<<endl;
        cout<<"Step3：输入字母[e]根据ROI区域制作下模板特征--[ROI区域选择完成后才可制作]"<<endl;
        cout<<"Step4：输入字母[r]进行匹配效果测试，计算角度和液面高度"<<endl;
        cout<<"Close：输入字母[x]程序退出"<<endl;
        char chKey = getchar();
        switch(chKey)
        {
            case'q':
                //调整焦距
                AdjustFocus();
                break;
            case'w':
                //选取ROI区域，并计算试管所占像素
                SelectRoi();
                break;
            case'e':
                //制作模板
                MakeTemplate();
                break;
            case'r':
            //退出程序
                Testliquid();
                break;
            case'x':
                //退出程序
                run =false;
                break;
            default:
                break;
        }
    }
    return 0;
}
