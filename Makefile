CC = g++
CFLAGS = -g -Wall -std=c++17
CAM_PLIST_FLAG = -sectcreate __TEXT __info_plist Info.plist
PRODUCTS = run_calibrate run_overlay run_feature

OPENCV = `pkg-config opencv4 --cflags --libs`
LIBS = $(OPENCV)

run_calibrate: main_calibrate.cpp calibration.cpp fileHelper.cpp
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

run_overlay : main_overlay.cpp calibration.cpp fileHelper.cpp
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

run_feature: main_feature.cpp calibration.cpp fileHelper.cpp
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

.PHONY: clean

clean : 
	rm -r -f *.o *~ $(PRODUCTS) *.dSYM
