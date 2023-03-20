/**
    Calibration and Augmented Reality
    Created by Hui Hu for CS 5330 Computer Vision Spring 2023

    Functions to calibrate camera and generate virtual objects in a scene
*/

// basic
#include <cmath>
#include <filesystem>
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
#include "fileHelper.h"

using namespace std;
using namespace cv;

// detect a target and extracting target corners
vector<Point2f> detectCorners(Mat &src, Mat &dst) {
    // get grayscale image
    Mat grayImg;
    cvtColor(src, grayImg, COLOR_BGR2GRAY);

    dst = src.clone();
    vector<Point2f> corner_set = {}; // this will be filled by the detected corners
    Size patternsize(9, 6);

    // Find Chessboard Corners
    bool patternfound = findChessboardCorners(dst, patternsize, corner_set, CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE + CALIB_CB_FAST_CHECK);

    if (patternfound) {
        cornerSubPix(grayImg, corner_set, Size(9, 9), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
        // cout << "found " << corner_set.size() << " corners, first corner at (" << corner_set[0].x << "," << corner_set[0].y << ")" << endl;
    } else {
        // cout << "found 0 corners" << endl;
    }

    // Draw Chessboard Corners on Image
    drawChessboardCorners(dst, patternsize, Mat(corner_set), patternfound);

    return corner_set;
}

// rename image and save it to Resources folder
int saveImage(Mat &originalImage, string format) {
    // get seconds since epoch
    const auto current_time = chrono::system_clock::now();
    long seconds = chrono::duration_cast<chrono::seconds>(current_time.time_since_epoch()).count();

    // convert long seconds to string timeStamp
    stringstream stream;
    stream << seconds;
    string timeStamp = stream.str();

    // Open the directory or create it
    filesystem::create_directories("./img/");

    string imageName = "./img/" + timeStamp + "." + format;
    imwrite(imageName, originalImage.clone());
    return 0;
}

// specify calibration images
void saveData(Mat &src, vector<vector<Point2f>> corner_list, vector<vector<Point3f>> point_list, Mat &cameraMatrix, Mat &distCoeffs) {
    // save Image
    saveImage(src, "png");

    // save rotations and translations
    Size imageSize = src.size();
    char filePath[] = "./cameraConfig.csv";

    if (corner_list.size() > 5) {
        cout << "Camera matrix before:" << endl
             << cameraMatrix << endl;
        cout << "Distortion coefficients before:" << endl
             << distCoeffs << endl
             << endl;
    }

    // Calibrate camera
    vector<Mat> rvec, tvec;
    double error = calibrateCamera(point_list, corner_list, imageSize, cameraMatrix, distCoeffs, rvec, tvec, CALIB_FIX_ASPECT_RATIO);
    addData(filePath, cameraMatrix, distCoeffs, true);

    // Output the results
    if (corner_list.size() > 5) {
        cout << "Camera matrix after:" << endl
             << cameraMatrix << endl;
        cout << "Distortion coefficients after:" << endl
             << distCoeffs << endl;

        cout << "error: " << error << endl
             << endl
             << endl;
    }
}

// Project Outside Corners or 3D Axes
void projectCorner(Mat &src, Mat &dst, Mat &rvec, Mat &tvec, Mat &cameraMatrix, Mat &distCoeffs) {
    vector<Point2f> imagePoints;
    vector<Point3f> shape = {Point3f(-1, 1, 0), Point3f(9, 1, 0), Point3f(9, -6, 0), Point3f(-1, -6, 0), Point3f(0, 0, 0), Point3f(1, 0, 0), Point3f(0, 1, 0), Point3f(0, 0, 1)};

    projectPoints(shape, rvec, tvec, cameraMatrix, distCoeffs, imagePoints);

    dst = src.clone();
    vector<vector<Point>> contourAry = {};
    vector<Point> contour = {};
    vector<Point> arrow = {};
    for (int i = 0; i < 4; i++) {
        contour.push_back(Point(imagePoints[i].x, imagePoints[i].y));
    }
    for (int i = 4; i < 8; i++) {
        arrow.push_back(Point(imagePoints[i].x, imagePoints[i].y));
    }

    contourAry.push_back(contour);
    if (contour.size() > 0) {
        drawContours(dst, contourAry, -1, Scalar(0, 0, 255), 4);
    }

    Scalar color(0, 255, 0); // green color
    int thickness = 2;
    int line_type = LINE_AA; // anti-aliased line
    int shift = 0;
    arrowedLine(dst, arrow[0], arrow[1], color, thickness, line_type, shift); // x
    arrowedLine(dst, arrow[0], arrow[2], color, thickness, line_type, shift); // y
    arrowedLine(dst, arrow[0], arrow[3], color, thickness, line_type, shift); // z
}

// Create a Virtual Object
void goldenGateBridge(Mat &src, Mat &dst, Mat &rvec, Mat &tvec, Mat &cameraMatrix, Mat &distCoeffs) {
    float unit = float(7) / (14 * 4);

    // left side and right side
    vector<Point3f> leftFrame = {};
    vector<Point3f> rightFrame = {};
    for (float i = 1.0; i < 2; i += unit) {
        float z = 1.0 + 2 * (i - 1) * (i - 1);
        leftFrame.push_back(Point3f(i, -2.5, z));
        rightFrame.push_back(Point3f(i, -3.5, z));
    }
    for (float i = 2.0; i < 7; i += unit) {
        float z = 1.0 + 2 * (i - 4.5) * (i - 4.5) / 6.25;
        leftFrame.push_back(Point3f(i, -2.5, z));
        rightFrame.push_back(Point3f(i, -3.5, z));
    }
    for (float i = 7.0; i < 8; i += unit) {
        float z = 1.0 + 2 * (i - 8) * (i - 8);
        leftFrame.push_back(Point3f(i, -2.5, z));
        rightFrame.push_back(Point3f(i, -3.5, z));
    }
    leftFrame.push_back(Point3f(8, -2.5, 1));
    rightFrame.push_back(Point3f(8, -3.5, 1));

    // surface
    vector<Point3f> surfaceFrame = {};
    for (float i = 1.0; i <= 2; i += unit) {
        float z = 1.0 + 2 * (i - 1) * (i - 1);
        surfaceFrame.push_back(Point3f(i, -2.5, 1));
        surfaceFrame.push_back(Point3f(i, -2.5, z));
        surfaceFrame.push_back(Point3f(i, -2.5, 1));
        surfaceFrame.push_back(Point3f(i, -3.5, 1));
        surfaceFrame.push_back(Point3f(i, -3.5, z));
        surfaceFrame.push_back(Point3f(i, -3.5, 1));
        surfaceFrame.push_back(Point3f(i, -2.5, 1));
    }

    for (float i = 2.0; i <= 7; i += unit) {
        float z = 1.0 + 2 * (i - 4.5) * (i - 4.5) / 6.25;
        surfaceFrame.push_back(Point3f(i, -2.5, 1));
        surfaceFrame.push_back(Point3f(i, -2.5, z));
        surfaceFrame.push_back(Point3f(i, -2.5, 1));
        surfaceFrame.push_back(Point3f(i, -3.5, 1));
        surfaceFrame.push_back(Point3f(i, -3.5, z));
        surfaceFrame.push_back(Point3f(i, -3.5, 1));
        surfaceFrame.push_back(Point3f(i, -2.5, 1));
    }

    for (float i = 7.0; i <= 8; i += unit) {
        float z = 1.0 + 2 * (i - 8) * (i - 8);
        surfaceFrame.push_back(Point3f(i, -2.5, 1));
        surfaceFrame.push_back(Point3f(i, -2.5, z));
        surfaceFrame.push_back(Point3f(i, -2.5, 1));
        surfaceFrame.push_back(Point3f(i, -3.5, 1));
        surfaceFrame.push_back(Point3f(i, -3.5, z));
        surfaceFrame.push_back(Point3f(i, -3.5, 1));
        surfaceFrame.push_back(Point3f(i, -2.5, 1));
    }

    // base
    vector<Point3f> baseFrame = {};
    for (int i = 2; i < 8; i += 5) {
        baseFrame.push_back(Point3f(float(i), -2.5, 1.0));
        baseFrame.push_back(Point3f(float(i), -2.5, 0.0));
        baseFrame.push_back(Point3f(float(i), -2.5, 3.0));
        baseFrame.push_back(Point3f(float(i), -3.5, 3.0));
        baseFrame.push_back(Point3f(float(i), -3.5, 0.0));
        baseFrame.push_back(Point3f(float(i), -3.5, 1.0));
        baseFrame.push_back(Point3f(float(i), -2.5, 1.0));
    }

    vector<Point2f> leftPoints;
    projectPoints(leftFrame, rvec, tvec, cameraMatrix, distCoeffs, leftPoints);
    vector<Point> leftContour = {};
    for (int i = 0; i < leftPoints.size(); i++) {
        leftContour.push_back(Point(leftPoints[i].x, leftPoints[i].y));
    }

    vector<Point2f> rightPoints;
    projectPoints(rightFrame, rvec, tvec, cameraMatrix, distCoeffs, rightPoints);
    vector<Point> rightContour = {};
    for (int i = 0; i < rightPoints.size(); i++) {
        rightContour.push_back(Point(rightPoints[i].x, rightPoints[i].y));
    }

    vector<Point2f> surfacePoints;
    projectPoints(surfaceFrame, rvec, tvec, cameraMatrix, distCoeffs, surfacePoints);
    vector<Point> surfaceContour = {};
    for (int i = 0; i < surfacePoints.size(); i++) {
        surfaceContour.push_back(Point(surfacePoints[i].x, surfacePoints[i].y));
    }

    vector<Point2f> basePoints;
    projectPoints(baseFrame, rvec, tvec, cameraMatrix, distCoeffs, basePoints);
    vector<Point> baseContour = {};
    for (int i = 0; i < basePoints.size(); i++) {
        baseContour.push_back(Point(basePoints[i].x, basePoints[i].y));
    }

    vector<vector<Point>> contourAry = {};
    contourAry.push_back(rightContour);
    contourAry.push_back(leftContour);
    contourAry.push_back(surfaceContour);
    contourAry.push_back(baseContour);

    dst = src.clone();
    drawContours(dst, contourAry, -1, Scalar(0, 0, 255), 2);
};

void tower(Mat &src, Mat &dst, Mat &rvec, Mat &tvec, Mat &cameraMatrix, Mat &distCoeffs) {
    float unit = float(1) / 4;

    // layers
    vector<Point3f> layers = {};
    layers.push_back(Point3f(0, 0, 0));
    for (float z = 0.0; z < 6; z += unit) {
        float x = 0.5 + (z - 3) * (z - 3) / 18;
        layers.push_back(Point3f(x, x, z));
        layers.push_back(Point3f(x, -x, z));
        layers.push_back(Point3f(-x, -x, z));
        layers.push_back(Point3f(-x, x, z));
        layers.push_back(Point3f(x, x, z));
    }
    for (float z = 6.0; z < 8; z += unit) {
        float x = (8 - z) / 2;
        layers.push_back(Point3f(x, x, z));
        layers.push_back(Point3f(x, -x, z));
        layers.push_back(Point3f(-x, -x, z));
        layers.push_back(Point3f(-x, x, z));
        layers.push_back(Point3f(x, x, z));
    }
    layers.push_back(Point3f(0, 0, 8));


    // axis
    vector<Point3f> axis = {};
    for (float z = 0.0; z < 6; z += unit) {
        float x = 0.5 + (z - 3) * (z - 3) / 18;
        axis.push_back(Point3f(x, x, z));
    }
    for (float z = 6.0; z < 8; z += unit) {
        float x = (8 - z) / 2;
        axis.push_back(Point3f(x, x, z));
    }
    for (float z = 8.0; z > 6; z -= unit) {
        float x = (8 - z) / 2;
        axis.push_back(Point3f(x, -x, z));
    }
    for (float z = 6.0; z > 0; z -= unit) {
        float x = 0.5 + (z - 3) * (z - 3) / 18;
        axis.push_back(Point3f(x, -x, z));
    }
    for (float z = 0.0; z < 6; z += unit) {
        float x = 0.5 + (z - 3) * (z - 3) / 18;
        axis.push_back(Point3f(-x, -x, z));
    }
    for (float z = 6.0; z < 8; z += unit) {
        float x = (8 - z) / 2;
        axis.push_back(Point3f(-x, -x, z));
    }
    for (float z = 8.0; z > 6; z -= unit) {
        float x = (8 - z) / 2;
        axis.push_back(Point3f(-x, x, z));
    }
    for (float z = 6.0; z > 0; z -= unit) {
        float x = 0.5 + (z - 3) * (z - 3) / 18;
        axis.push_back(Point3f(-x, x, z));
    }

    vector<Point2f> layersPoints;
    projectPoints(layers, rvec, tvec, cameraMatrix, distCoeffs, layersPoints);
    vector<Point> layersContour = {};
    for (int i = 0; i < layersPoints.size(); i++) {
        layersContour.push_back(Point(layersPoints[i].x, layersPoints[i].y));
    }

    vector<Point2f> axisPoints;
    projectPoints(axis, rvec, tvec, cameraMatrix, distCoeffs, axisPoints);
    vector<Point> axisContour = {};
    for (int i = 0; i < axisPoints.size(); i++) {
        axisContour.push_back(Point(axisPoints[i].x, axisPoints[i].y));
    }

    vector<vector<Point>> contourAry = {};
    contourAry.push_back(layersContour);
    contourAry.push_back(axisContour);

    dst = src.clone();
    drawContours(dst, contourAry, -1, Scalar(255, 0, 0), 2);
}

// Detect Robust Features
void detectRobustFeatures(Mat &src, Mat &dst_circled) {
    int thresh = 200;
    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.04;

    Mat src_gray, detected, original;
    Mat dst = cv::Mat::zeros(src.size(), CV_32FC1);

    cvtColor(src, src_gray, COLOR_BGR2GRAY);
    cv::cornerHarris(src_gray, dst, blockSize, apertureSize, k);

    cv::Mat dst_norm;
    cv::normalize(dst, dst_norm, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat());
    // cv::convertScaleAbs(dst_norm, dst_norm_scaled);

    dst_circled = src.clone();

    for (int i = 0; i < dst_norm.rows; i++) {
        for (int j = 0; j < dst_norm.cols; j++) {
            if ((int)dst_norm.at<float>(i, j) > thresh) {
                cv::circle(dst_circled, cv::Point(j, i), 8, cv::Scalar(0, 0, 255), 2, 8, 0);
            }
        }
    }
}
