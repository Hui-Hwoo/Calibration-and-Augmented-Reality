//
//  vidDisplay.cpp
//  Computer Vision - Project 1
//
//  Created by Hui Hu on 1/24/23.
//

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
    vector<vector<Point2f>> corner_list;
    vector<vector<Vec3f>> point_list;

    // each chessboard has the shape
    int x, y;
    int z = 0;
    vector<Vec3f> point_set;
    for (x = 0; x < 9; x++) {
        for (y = 0; y > -6; y--) {
            point_set.push_back(Vec3f(x, y, z));
        }
    }

    VideoCapture *capdev;

    // open the video device
    capdev = new VideoCapture(0);
    if (!capdev->isOpened()) {
        printf("Unable to open video device\n");
        return (-1);
    }

    namedWindow("Video", 1); // identifies a window
    Mat frame;
    Mat dst;

    for (;;) {
        // get a new frame from the camera, treat as a stream
        *capdev >> frame;
        if (frame.empty()) {
            printf("frame is empty\n");
            continue;
        }

        Mat dst;
        vector<Point2f> corner_set = detectCorners(frame, dst);

        switch (pollKey()) {
        case 'q':
        case 27:
            return 0;
        case 's':
            if (corner_set.size() == 54) {
                corner_list.push_back(corner_set);
                point_list.push_back(point_set);
                saveData(frame, corner_list, point_list);
                cout << "there" << endl;
            }
            break;
        default:
            break;
        }

        imshow("Video", dst);
    }

    delete capdev;
    return (0);
}
