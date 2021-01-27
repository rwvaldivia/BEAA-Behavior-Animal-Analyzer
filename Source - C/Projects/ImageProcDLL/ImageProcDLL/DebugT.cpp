#include "stdafx.h"
#include "DebugT.h"

DebugT::DebugT(void)
{
}


DebugT::~DebugT(void)
{
}

void DebugT::debugImage(cv::Mat m, char * name){
	//qcv::namedWindow(name);
	cv::imshow(name, m);
}

void DebugT::debugPrint(char * str) {
	printf("DEBUG [%s]\n", str);
}

void DebugT::debugPrintI(char * str, int i){
	printf("DEBUG [(%d) %s]\n", i, str);
}
