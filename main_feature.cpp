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

    namedWindow("Feature", 1); // identifies a window

    for (;;) {
        // get a new frame from the camera, treat as a stream
        Mat frame;
        *capdev >> frame;
        if (frame.empty()) {
            printf("frame is empty\n");
            continue;
        }

        Mat dst;
        detectRobustFeatures(frame, dst);
        imshow("Feature", dst);

        switch (pollKey()) {
        case 'q':
        case 27:
            return 0;
        case 's':
            saveImage(dst, "png");
            break;
        default:
            break;
        }
    }

    delete capdev;
    return 0;
}
