/**
    Matching Algorithm for Content-based Image Retrieval (CBIR)
    Created by Hui Hu for CS 5330 Computer Vision Spring 2023

    Functions to extract feature vector and calculate the distance
*/

// basic
#include <cmath>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
// opencv
#include <opencv2/core/types_c.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
// custom function

using namespace std;
using namespace cv;

// detect a target and extracting target corners
void detectCorners(Mat &src, Mat &dst) {
    dst = src.clone();
    vector<Point2f> corners; // this will be filled by the detected corners
    Size patternsize(8, 6);
    bool patternfound = findChessboardCorners(src, patternsize, corners, CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE + CALIB_CB_FAST_CHECK);
    if (patternfound) {
        cornerSubPix(src, corners, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
    }
    drawChessboardCorners(dst, patternsize, Mat(corners), patternfound);
}

// specify calibration images
void selectCalibrationImg() {
    std::vector<cv::Vec3f> point_set;
    std::vector<std::vector<cv::Vec3f>> point_list;
    std::vector<std::vector<cv::Point2f>> corner_list;

    
}

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
