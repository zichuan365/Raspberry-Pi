#include <histSeg.h>
#include <raspicam/raspicam_cv.h>
#include <coreAlgorithm.h>

void onTrackbar(int, void*);
void onTrackbarTwo(int, void*);
void roiSelect(Mat input,int& X,int& Y,int& width,int& height);
void roiSelect(Mat input, Mat& output);

void AdjustFocus();
void SelectRoi();
void MakeTemplate();
void Testliquid();
