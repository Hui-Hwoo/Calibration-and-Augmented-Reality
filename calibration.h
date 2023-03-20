/**
    Matching Algorithm for Content-based Image Retrieval (CBIR)
    Created by Hui Hu for CS 5330 Computer Vision Spring 2023

    Functions to extract feature vector and calculate the distance
*/

#ifndef CALIBRATION
#define CALIBRATION

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

using namespace std;
using namespace cv;

// detect a target and extracting target corners
vector<Point2f> detectCorners(Mat &src, Mat &dst);

// rename image and save it to Resources folder
int saveImage(Mat &originalImage, string format = "png");

// save calibration image and corresponding rotations and translations
void saveData(Mat &src, vector<vector<Point2f>> corner_list, vector<vector<Point3f>> point_list, Mat &cameraMatrix, Mat &distCoeffs);

// Project Outside Corners or 3D Axes
void projectCorner(Mat &src, Mat &dst, Mat &rvec, Mat &tvec, Mat &cameraMatrix, Mat &distCoeffs);

// Create a Virtual Object
void goldenGateBridge(Mat &src, Mat &dst, Mat &rvec, Mat &tvec, Mat &cameraMatrix, Mat &distCoeffs);

void tower(Mat &src, Mat &dst, Mat &rvec, Mat &tvec, Mat &cameraMatrix, Mat &distCoeffs);

// Detect Robust Features
void detectRobustFeatures(Mat &src, Mat &dst);

#endif
