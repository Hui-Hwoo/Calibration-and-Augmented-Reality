/*
    Helper functions to deal with CSV files
*/

#ifndef FILE_HELPER
#define FILE_HELPER

#include <vector>
#include <string>

using namespace std;


int addData(char *filaPath, cv::Mat &cameraMatrix, cv::Mat &distCoeffs, bool reset = false);

int readData(char *filePath, cv::Mat &cameraMatrix, cv::Mat &distCoeffs);

#endif