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
std::vector<cv::Point2f> detectCorners(cv::Mat &src, cv::Mat &dst) {
    cv::Mat grayImg;
    cv::cvtColor(src, grayImg, cv::COLOR_BGR2GRAY);
    dst = src.clone();
    std::vector<cv::Point2f> corner_set = {}; // this will be filled by the detected corners
    cv::Size patternsize(9, 6);
    bool patternfound = cv::findChessboardCorners(dst, patternsize, corner_set, CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE + CALIB_CB_FAST_CHECK);
    if (patternfound) {
        cv::cornerSubPix(grayImg, corner_set, cv::Size(9, 9), cv::Size(-1, -1), cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
        // std::cout << "found " << corner_set.size() << " corners, first corner at (" << corner_set[0].x << "," << corner_set[0].y << ")" << std::endl;
    } else {
        // std::cout << "found 0 corners" << std::endl;
    }
    cv::drawChessboardCorners(dst, patternsize, cv::Mat(corner_set), patternfound);
    return corner_set;
}

// specify calibration images
void saveData(
    Mat &src,
    vector<vector<Point2f>> corner_list,
    vector<vector<Point3f>> point_list,
    Mat &cameraMatrix,
    Mat &distCoeffs) {
    // save Image

    // save rotations and translations
    Size imageSize = src.size();

    // std::cout << "Camera matrix before:" << std::endl
    //           << cameraMatrix << std::endl;
    // std::cout << "Distortion coefficients before:" << std::endl
    //           << distCoeffs << std::endl
    //           << endl;

    // Calibrate camera
    cv::Mat rvec, tvec;
    double error = cv::calibrateCamera(point_list, corner_list, imageSize, cameraMatrix, distCoeffs, rvec, tvec, CALIB_FIX_ASPECT_RATIO);

    // Output the results
    // std::cout << "Camera matrix after:" << std::endl
    //           << cameraMatrix << std::endl;
    // std::cout << "Distortion coefficients after:" << std::endl
    //           << distCoeffs << std::endl;
    // std::cout << "Rotation vector:" << std::endl
    //           << rvec << std::endl;
    // std::cout << "Translation vector:" << std::endl
    //           << tvec << std::endl;

    cout << "error: " << error << endl
         //  << endl
         << endl;

    return;
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
