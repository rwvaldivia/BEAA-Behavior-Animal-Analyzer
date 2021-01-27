#include <opencv2\highgui\highgui.hpp>
#include "ImageFrameConfig.h"


ImageFrameConfig::ImageFrameConfig(){

	correlation		= 0;
	chi_sqt			= 0;
	intersect		= 0;
	bhattacharyya	= 0;

};

ImageFrameConfig::~ImageFrameConfig(){}

void ImageFrameConfig::setBhattacharyya(double d){
	bhattacharyya = d;
}

double ImageFrameConfig::getBhattacharyya(){
	return bhattacharyya;
}

void ImageFrameConfig::setIntersect(double d){
	intersect = d;
}

double ImageFrameConfig::getIntersect(){
	return intersect;
}

void ImageFrameConfig::setChiSqt(double d){
	chi_sqt = d;
}

double ImageFrameConfig::getChiSqt(){
	return chi_sqt;
}

double ImageFrameConfig::getCorrelation(){
	return correlation;
}

void ImageFrameConfig::setCorrelation(double d){
	correlation = d;
}

void ImageFrameConfig::setMatrixND(cv::MatND matrix){
	matrixND = matrix;
}

cv::MatND ImageFrameConfig::getMatrixND() {
	return matrixND;
}