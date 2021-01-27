#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <time.h>
#include <iostream>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

class ConfigProcessVideo {
public:

	const static char TYPE_CONFIG_SELECT_REGIONS   = 'A';
	const static char TYPE_CONFIG_PROCESS_VIDEO	= 'B';
	cv::Mat frameFeed;
	cv::Rect box;

	cv::Point corner1;
	cv::Point corner2;
	
	int SENSITIVITY_VALUE;
	int BLUR_SIZE;

	bool leftButtonMouseDown;
	bool leftButtonMouseUp;
	bool startImageProc;
	char * mainWindowName;
	char typeConfig;
	
	//#####################################
	//Usado para selecionar varias regioes
	bool isRegionSelected;

};

class SelectedRegion {
public:
	int indexRegion;
	cv::Point p1;
	cv::Point p2;

	SelectedRegion::SelectedRegion(void){
		indexRegion = 0;
	}

	SelectedRegion SelectedRegion::clone(){

		SelectedRegion r;

		r.p1.x = p1.x;
		r.p1.y = p1.y;
		r.p2.x = p2.x;
		r.p2.y = p2.y;

		return r;

	}
};

struct stProcConfigParameters {
	char brightness[10];
    char sensibilityValue[10];
    char blurValue[10];
};


struct stParamJNI{
	char index[10];
	char p1x[10];
	char p1y[10];
	char p2x[10];
	char p2y[10];
};