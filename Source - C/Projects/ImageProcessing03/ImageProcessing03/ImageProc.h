
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <iostream>
#include <stdio.h>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include "ImageFrameBuffer.h"



#ifndef __ImageProc__
#define __ImageProc__

class ImageProc {
	
private:
	//Controle de ocorrencias em um determinado momento
	//Indica o numero de ocorrencias de um momento
	const int MIN_EVENT_OCORRENCIES_ALLOWED_TO_SAVE;
	int SENSITIVITY_VALUE;
	int BLUR_SIZE;
	int movementEventOcurrencies;
	bool movementDetectionInProgress;
	bool initVideoForSave;
	bool saveBuffer;
	int eventMovimentCounter;
	int motionCounter;
	int blockFrameCounter;
	int indexAreaSelection;
	string fileName;
	ImageFrameBuffer buffer;
	Rect boxArea;
	VideoWriter videoWriter;
	Point imageBox1;
	Point imageBox2;

public:
	ImageProc(int, Rect, Point, Point);
	ImageProc(void);
	~ImageProc();

	bool processSingleImage(cv::Mat image);
	void drawNumberedBox(cv::Mat framefeed, int indexRectangleArea, Point p1, Point p2, Scalar cor);
	void setFileName(string name);
	string getFileName();
	void analiseMoment(cv::Mat image);
	
	bool initBuffer(cv::Mat);

	Rect getBoxArea();
	void setBoxArea(Rect);

	int getSensivityValue();
	void setSensivityValue(int);
	
	int getBlurSize();
	void setBlurSize(int);

	
	int getMovementEventOcurrencies();
	void setMovementEventOcurrencies(int);

	bool isMovementDetectionInProgress();
	void setMovementDetectionInProgress(bool);

	void setMotionCounter(int);
	int getMotionCounter();

	void setBlockFrameCounter(int);
	int getBlockFrameCounter();

	void setSaveBuffer(bool);
	int isToSaveBuffer();
	ImageFrameBuffer * getImageFrameBuffer();

	void saveBlockOfVideo();

};


#endif