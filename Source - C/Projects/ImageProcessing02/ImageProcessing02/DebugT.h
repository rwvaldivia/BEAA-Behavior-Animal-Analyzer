#pragma once
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <iostream>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

class DebugT
{
public:
	DebugT(void);
	~DebugT(void);

	void debugImage(cv::Mat, char * name);
	void debugPrint(char * str);
	void debugPrintI(char * str, int i);
};

