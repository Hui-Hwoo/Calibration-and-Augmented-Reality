/**
    Calibration and Augmented Reality
    Created by Hui Hu for CS 5330 Computer Vision Spring 2023

    Detect Robust Feature - Harris corners
*/

#include <iostream>
#include <stdio.h>

#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

#include "calibration.h"

using namespace cv;
using namespace std;

int main(int argc, char *argv[]) {
    // Open the video device
    VideoCapture *capdev = new VideoCapture(0);
    if (!capdev->isOpened()) {
        printf("Unable to open video device\n");
        return (-1);
    }

    namedWindow("Video", 1); // identifies a window

    // Create or/and initialize basic variables
    vector<vector<Point2f>> corner_list;
    vector<vector<Point3f>> point_list;
    Mat distCoeffs = Mat::zeros(5, 1, CV_64F);

    // Each detected chessboard has the shape
    vector<Point3f> point_set;
    for (int y = 0; y < 6; y++) {
        for (int x = 0; x < 9; x++) {
            point_set.push_back(Point3f(x, -y, 0));
        }
    }

    // Get the size of frame to initialize cameraMatrix
    Mat cameraMatrix;
    Mat frame;
    *capdev >> frame;
    if (!frame.empty()) {
        double data[3][3] = {{1, 0, double(frame.cols / 2)}, {0, 1, double(frame.rows / 2)}, {0, 0, 1}};
        cameraMatrix = Mat(Size(3, 3), CV_64FC1, data);
    } else {
        double data[3][3] = {{1, 0, double(1920 / 2)}, {0, 1, double(1080 / 2)}, {0, 0, 1}};
        cameraMatrix = Mat(Size(3, 3), CV_64FC1, data);
    }

    for (;;) {
        // get a new frame from the camera, treat as a stream
        Mat frame;
        *capdev >> frame;
        if (frame.empty()) {
            printf("frame is empty\n");
            continue;
        }

        Mat dst;
        vector<Point2f> corner_set = detectCorners(frame, dst);
        // detectRobustFeatures(frame, dst);
        // for(int i = 0; i < corner_set.size(); i++){
        //     cout << corner_set[i] << " ";
        // }
        // cout << endl;
        imshow("Video", dst);

        switch (pollKey()) {
        case 'q':
        case 27:
            return 0;
        case 's':
            // imwrite("./img/task1.png", dst.clone());
            if (corner_set.size() == 54) {
                corner_list.push_back(corner_set);
                point_list.push_back(point_set);
                saveData(frame, corner_list, point_list, cameraMatrix, distCoeffs);
                // if (corner_list.size() > 5) {
                    
                //     // goldenGateBridge(frame, corner_list, point_list, cameraMatrix, distCoeffs);
                // }
            }
            break;
        default:
            break;
        }
    }

    delete capdev;
    return 0;
}
