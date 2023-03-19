//
//  main.cpp
//  Computer Vision - Project 1
//
//  Created by Hui Hu on 1/10/23.
//

#include "calibration.h"
#include <fstream>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

int main() {
    // read the image
    Mat src = imread("./img/chessboard.png");
    Mat dst;
    detectCorners(src, dst);

    // Enter a loop
    while (true) {
        imshow("Image", dst);
        char key = (char)cv::waitKey(0); // explicit cast

        switch (key) {
            // Modify Contrast
        default:
            break;
        }

        // exit if 'q' key or 'esc' key was pressed.
        if (key == 'q' || key == 27)
            break;
    }
    return 0;
}
