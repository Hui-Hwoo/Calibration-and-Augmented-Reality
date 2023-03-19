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
std::vector<cv::Point2f> detectCorners(Mat &src, Mat &dst);

// save calibration image and corresponding rotations and translations
void saveData(Mat &src, vector<vector<Point2f>> corner_list, vector<vector<Point3f>> point_list,
              Mat &cameraMatrix,
              Mat &distCoeffs);

// Calibrate the Camera
void calibrateCamera();

// Calculate Current Position of the Camera
void calculatePos();

// Project Outside Corners or 3D Axes
void projectCorner();

// Create a Virtual Object
void createVirtualObj();

// Detect Robust Features
void detectRobustFeatures();

#endif
