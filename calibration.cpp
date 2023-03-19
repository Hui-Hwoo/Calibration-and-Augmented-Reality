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
    std::vector<cv::Mat> rvec, tvec;
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

    // cout << "error: " << error << endl
    //      //  << endl
    //      << endl;

    vector<Point2f> imagePoints;
    vector<Point3f> shape = {
        Point3f(-1, 1, 0),
        Point3f(9, 1, 0),
        Point3f(9, -6, 0),
        Point3f(-1, -6, 0)};
    // vector<Point3f> shape = {
    //     Point3f(0.5, 0.5, 0),
    //     Point3f(0, 1.5, 0)};

    projectPoints(shape, rvec[0], tvec[0], cameraMatrix, distCoeffs, imagePoints);

    Mat projected = src.clone();
    vector<vector<Point>> contourAry = {};
    vector<Point> contour = {};
    for (int i = 0; i < imagePoints.size(); i++) {
        contour.push_back(Point(imagePoints[i].x, imagePoints[i].y));
    }
    contourAry.push_back(contour);
    if (contour.size() > 0) {
        drawContours(projected, contourAry, -1, Scalar(0, 0, 255), 2);
        imshow("test", projected);
    }
    // Scalar color(0, 0, 255); // red color
    // int thickness = 2;
    // int line_type = cv::LINE_AA; // anti-aliased line
    // int shift = 0;
    // arrowedLine(projected, contour[0], contour[1], color, thickness, line_type, shift);
    imshow("test", projected);

    return;
}

// Calibrate the Camera
void calibrateCamera();

// Calculate Current Position of the Camera
void calculatePos();

// Project Outside Corners or 3D Axes
void projectCorner();

// Create a Virtual Object
void goldenGateBridge(Mat &src,
                      vector<vector<Point2f>> corner_list,
                      vector<vector<Point3f>> point_list,
                      Mat &cameraMatrix,
                      Mat &distCoeffs) {

    Size imageSize = src.size();
    std::vector<cv::Mat> rvec, tvec;
    double error = cv::calibrateCamera(point_list, corner_list, imageSize, cameraMatrix, distCoeffs, rvec, tvec, CALIB_FIX_ASPECT_RATIO);

    float unit = float(7) / (14 * 20);

    // left side and right side
    vector<Point3f> leftFrame = {};
    vector<Point3f> rightFrame = {};
    for (float i = 1.0; i < 2; i += unit) {
        float z = 1 + 2 * (i - 1) * (i - 1);
        leftFrame.push_back(Point3f(i, -2.5, z));
        rightFrame.push_back(Point3f(i, -3.5, z));
    }
    for (float i = 2.0; i < 7; i += unit) {
        float z = 1 + 2 * (i - 4.5) * (i - 4.5) / 6.25;
        leftFrame.push_back(Point3f(i, -2.5, z));
        rightFrame.push_back(Point3f(i, -3.5, z));
    }
    for (float i = 7.0; i < 8; i += unit) {
        float z = 1 + 2 * (i - 8) * (i - 8);
        leftFrame.push_back(Point3f(i, -2.5, z));
        rightFrame.push_back(Point3f(i, -3.5, z));
    }
    leftFrame.push_back(Point3f(8, -2.5, 1));
    rightFrame.push_back(Point3f(8, -3.5, 1));

    vector<Point2f> leftPoints;
    projectPoints(leftFrame, rvec[0], tvec[0], cameraMatrix, distCoeffs, leftPoints);
    vector<Point> leftContour = {};
    for (int i = 0; i < leftPoints.size(); i++) {
        leftContour.push_back(Point(leftPoints[i].x, leftPoints[i].y));
    }

    vector<Point2f> rightPoints;
    projectPoints(rightFrame, rvec[0], tvec[0], cameraMatrix, distCoeffs, rightPoints);
    vector<Point> rightContour = {};
    for (int i = 0; i < rightPoints.size(); i++) {
        rightContour.push_back(Point(rightPoints[i].x, rightPoints[i].y));
    }

    vector<vector<Point>> contourAry = {};
    contourAry.push_back(rightContour);
    contourAry.push_back(leftContour);
    Mat projected = src.clone();
    drawContours(projected, contourAry, -1, Scalar(0, 0, 255), 2);
    // imwrite("./img/test.png", projected.clone());
    imshow("test", projected);
};

// Detect Robust Features
void detectRobustFeatures();
