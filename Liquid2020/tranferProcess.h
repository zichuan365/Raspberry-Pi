#ifndef TRANFERPROCESS_H
#define TRANFERPROCESS_H
//将result和socket相结合，传输结果

#include <raspicam/raspicam_cv.h>
#include <iostream>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <modbus.h>

#include "unit-test.h"
#include <coreAlgorithm.h>

class tranferProcess
{
public:
    tranferProcess();
    ~tranferProcess();

    int createServer(int Port);
private:
    raspicam::RaspiCam_Cv cam;
    Mat frame;
    Mat temp,input;

    Mat templateImg;
    int roiX = 0;
    int roiY = 0;
    int roiW = 0;
    int roiH = 0;

    int begin = 0;
    int end = 0;

    int tubeWidth = 0;
};

#endif // TRANFERPROCESS_H

