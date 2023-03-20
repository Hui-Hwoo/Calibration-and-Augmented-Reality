/*
    Created by Hui Hu for CS 5330 Computer Vision Spring 2023
*/

#include <cstring>
#include <dirent.h>
#include <iostream>
#include <stdio.h> /* printf, fgets */
#include <vector>

// openCV
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// Add the camera_matrix and the distortion_ceofficients data to CSV file
int addData(char *filePath, Mat &cameraMatrix, Mat &distCoeffs, bool reset) {
    // Variables for CSV file
    char mode[8];
    FILE *fp;

    // Open CSV file
    if (reset) {
        // If file exists, erase all the contents; if file doesn't exist, create new file
        strcpy(mode, "w");
    } else {
        // If file exists, start writing from the end; if file doesn't exist, create new file
        strcpy(mode, "a");
    }

    fp = fopen(filePath, mode);

    if (!fp) {
        printf("Unable to open output file %s\n", filePath);
        exit(-1);
    }

    // Write cameraMatrix data to CSV file
    char tmp[256];
    Mat_<double> cameraVec = cameraMatrix;
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            // Write formatted data to string
            sprintf(tmp, "%.8f,", (float)cameraVec(r, c));
            fwrite(tmp, sizeof(char), strlen(tmp), fp);
        }
    }

    // Write distortion_ceofficients data to CSV file
    Mat_<double> distortionVec = distCoeffs;
    for (int c = 0; c < 5; c++) {
        // Write formatted data to string
        sprintf(tmp, "%.8f,", (float)distortionVec(c));
        fwrite(tmp, sizeof(char), strlen(tmp), fp);
    }

    fwrite("\n", sizeof(char), 1, fp); // EOL

    // Close CSV file
    fclose(fp);

    return 0;
}

// Read one float value from a CSV file
int getfloat(FILE *fp, float *v) {
    char s[256];
    int p = 0;
    int eol = 0;

    for (;;) {
        char ch = fgetc(fp);
        if (ch == ',') {
            break;
        } else if (ch == '\n' || ch == EOF) {
            eol = 1;
            break;
        }

        s[p] = ch;
        p++;
    }
    s[p] = '\0';  // terminator
    *v = atof(s); // convert string to float

    return (eol); // return true if eol
}

// Read the camera_matrix and the distortion_ceofficients data from CSV file
int readData(char *filePath, Mat &cameraMatrix, Mat &distCoeffs) {
    FILE *fp;
    float value;
    cameraMatrix = Mat::zeros(Size(3, 3), CV_64FC1);
    distCoeffs = Mat::zeros(Size(5, 1), CV_64FC1);

    Mat_<double> cameraVec = cameraMatrix;
    Mat_<double> distortionVec = distCoeffs;

    // Open CSV file
    fp = fopen(filePath, "r");
    if (!fp) {
        printf("Unable to open feature file\n");
        return 1;
    }

    // Read file
    int count = 0;
    for (;;) {
        float eol = getfloat(fp, &value);
        if (count < 9) {
            cameraVec(count / 3, count % 3) = double(value);
            count++;
        } else if (count < 15) {
            distortionVec(count - 9) = double(value);
            count++;
        } else {
            break;
        }

        if (eol) {
            break;
        }
    }

    // Close CSV file
    fclose(fp);
    printf("Finished reading CSV file\n");

    return 0;
}
