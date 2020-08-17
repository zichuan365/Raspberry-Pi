#pragma once
#include <iostream>
#include <algorithm>
#include <omp.h>
#include <opencv/cv.h>
#include <unordered_map>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <fstream>
#include <assert.h>
#include <vector>
#include <math.h>
#include <sstream>
#include <iomanip>

using namespace std;
using namespace cv;


//Otsu大图分割
float var(int hist[], int level, float val, int pix_num);

Mat otsu(Mat& input);

//KmeansRGB图像分割
struct Center {
    double blueValue;
    double greenValue;
    double redValue;
};
void createClustersInfo(Mat imgInput, int clusters_number, vector<Scalar> & clustersCenters, vector<vector<Point>> & ptInClusters);
double computeColorDistance(Scalar pixel, Scalar clusterPixel);
void findAssociatedCluster(Mat imgInput, int clusters_number, vector<Scalar> clustersCenters, vector<vector<Point>> & ptInClusters);
double adjustClusterCenters(Mat imgInput, int clusters_number, vector<Scalar> & clustersCenters, vector<vector<Point>> ptInClusters, double & oldCenter, double newCenter);
Mat applyFinalClusterToImage(Mat & imgOutput, int clusters_number, vector<vector<Point>> ptInClusters);

Mat Kmeans(Mat input,int& begin,int& end);

int maxValue(unordered_map<int,int>num);

