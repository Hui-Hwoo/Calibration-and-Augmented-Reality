/**
    Calibration and Augmented Reality
    Created by Hui Hu for CS 5330 Computer Vision Spring 2023

    Project virtual objects
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
#include "fileHelper.h"

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

    // Initialize cameraMatrix and distortion_ceofficients with the value in CSV file
    Mat cameraMatrix, distCoeffs;
    char filePath[] = "./cameraConfig.csv";
    int result = readData(filePath, cameraMatrix, distCoeffs);
    if (result != 0) {
        cout << "Can not find CSV file, please try again!" << endl;
        return 1;
    }

    // Each detected chessboard has the shape
    vector<Point3f> point_set;
    for (int y = 0; y < 6; y++) {
        for (int x = 0; x < 9; x++) {
            point_set.push_back(Point3f(x, -y, 0));
        }
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

        Mat rvec, tvec;

        if (corner_set.size() == 54) {
            solvePnP(point_set, corner_set, cameraMatrix, distCoeffs, rvec, tvec);
            projectCorner(frame, dst, rvec, tvec, cameraMatrix, distCoeffs);
            goldenGateBridge(frame, dst, rvec, tvec, cameraMatrix, distCoeffs);
            cout
                << "Rotation: " << endl
                << rvec << endl
                << endl;
            cout << "Translation: " << endl
                 << tvec << endl
                 << endl;
        }

        imshow("Overlay", dst);

        Mat dstClone;
        switch (pollKey()) {
        case 's':
            dstClone = dst.clone();
            saveImage(dstClone);
            break;
        case 'q':
        case 27:
            return 0;
        default:
            break;
        }
    }

    delete capdev;
    return 0;
}
