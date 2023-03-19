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
    // 
    std::vector<std::vector<cv::Point2f>> corner_list;
    std::vector<std::vector<cv::Vec3f>> point_list;

    // read the image
    Mat src = imread("./img/chessboard.png");
    Mat dst;

    // each chessboard has the shape
    int x, y;
    int z = 0;
    std::vector<cv::Vec3f> point_set;
    for(x = 0; x < 9; x++){
        for(y = 0; y > -6; y--){
            point_set.push_back(Vec3f(x, y, z));
        }
    }

    detectCorners(src, dst);

    // Enter a loop
    while (true) {
        imshow("Image", dst);
        char key = (char)cv::waitKey(0); // explicit cast

        switch (key) {
            // Modify Contrast
        case 's':
            

        default:
            break;
        }

        // exit if 'q' key or 'esc' key was pressed.
        if (key == 'q' || key == 27)
            break;
    }
    return 0;
}
