# Use One Travel Day

# Calibration-and-Augmented-Reality

This project involves learning how to calibrate a camera and using the calibration to generate virtual objects in a scene. The project uses a checkerboard pattern as a target, and the tasks involved include detecting and extracting the checkerboard corners, selecting calibration images, calibrating the camera, and generating virtual objects in a scene relative to the checkerboard. 

If encounter any problems, please feel free to contact us via email hu.hui1@northeastern.edu

## Environment

MacOS Intel chip

IDE: VS Code

Build with Makefile

## Links

- link to report: https://cerulean-novel-54b.notion.site/Report-4-Calibration-and-Augmented-Reality-cc6a47d456514cd5abf308d67a232bf2

- link to github: https://github.com/Hui-Hwoo/Calibration-and-Augmented-Reality

# How to run the code

## Calibration part

In the treminal, enter command `make run_calibrate && ./run_calibrate` to compile and run cpp files

- Press `q` or `esc` to exit

- Press `s` to save image

## Project 3D objects part

In the treminal, enter command `make run_overlay && ./run_overlay` to compile and run cpp files

- Press `q` or `esc` to exit

- Press `s` to save image

## Feature part

In the treminal, enter command `make run_feature && ./run_feature` to compile and run cpp files

- Press `q` or `esc` to exit

- Press `s` to save image


# Tips

- Use `make clean` commmand to remove temporary files created when compiling

- Saved images will be renamed by the timestamps in `img` directory, therefore, the newely saved image will be the last one

- Program may have some delays, thank you for your patience :)


