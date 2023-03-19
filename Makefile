CC = g++
CFLAGS = -g -Wall -std=c++17
CAM_PLIST_FLAG = -sectcreate __TEXT __info_plist Info.plist
PRODUCTS = main 


OPENCV = `pkg-config opencv4 --cflags --libs`
LIBS = $(OPENCV)


main : main.cpp calibration.cpp
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

.PHONY: clean

clean : 
	rm -r -f *.o *~ $(PRODUCTS) *.dSYM
