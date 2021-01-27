#include "stdafx.h"
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <time.h>
#include <iostream>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

#include "ImageFrameBuffer.h"
#include "ImageProc.h"
#include "DebugT.h"

using namespace std;
using namespace cv;

bool leftButtonMouseDown	= false;
bool leftButtonMouseUp		= false;
bool startImageProc			= false;

Mat frameFeed;
Point corner1, corner2;
Rect box;

Point boxCoord01;
Point boxCoord02;

int SENSITIVITY_VALUE = 40;
int BLUR_SIZE = 10;

void trackbar_callback( int, void* ){}


static void mouse_callback(int event, int x, int y, int a, void * b){
	
	if (event == EVENT_LBUTTONDOWN){
		leftButtonMouseDown = true;
		corner1.x = x;
		corner1.y = y;

		//cout << "Corner 1 recored at " << corner1 << endl;
	}

	if (event == EVENT_LBUTTONUP){

		if (abs(x - corner1.x) > 10 && abs(y - corner1.y) > 10){
			leftButtonMouseUp = true;
			corner2.x = x;
			corner2.y = y;
			//cout << "Corner 2 record at " << corner2 << endl;
		} else {
			//cout << "Please select a big region" << endl;
			leftButtonMouseDown = false;
		}
	}

	if (leftButtonMouseDown == true && leftButtonMouseUp == false){
		Point pt_local;
		pt_local.x = x;
		pt_local.y = y;

		boxCoord01.x = corner1.x;
		boxCoord01.y = corner1.y;

		boxCoord02.x = pt_local.x;
		boxCoord02.y = pt_local.y;

		Mat local_img = frameFeed.clone();
		rectangle(local_img, corner1, pt_local, Scalar(0,0,255));
		imshow("Processamento de Video", local_img);

	}

	if (leftButtonMouseDown == true && leftButtonMouseUp == true){
		
		box.width = abs(corner1.x - corner2.x);
		box.height = abs(corner1.y - corner2.y);

		box.x = min(corner1.x, corner2.x);
		box.y = min(corner1.y, corner2.y);
		
		leftButtonMouseDown = false;
		leftButtonMouseUp = false;
		startImageProc = true;
		
	}

}

string intToString(int number){

	std::stringstream ss;
	ss << number;
	return ss.str();
}

int processVideo(char * buffer, char * fileName) {

	//namedWindow("Threshold Image", 0);
	namedWindow("Processamento de Video");
	setMouseCallback("Processamento de Video", mouse_callback);

	int inc					= 0;
	bool terminateApp		= false;
	bool recording			= false;
	bool startNewRecording	= false;
	bool firstRun			= true;
	bool motionDetected		= false;
	bool pause				= false;


	Mat frame2;
	Mat grayImage1,grayImage2;
	Mat differenceImage;
	Mat thresholdImage;
	VideoCapture capture;

	//capture.open("d:/_richard/19_01_R_112013090000.avi");
	//capture.open("d:/_richard/19_01_R_112013090000.avi");
	//capture.open("d:/_richard/M2U00543.MPG");
	capture.open("d:/_richard/Caixa21-43min.avi.mp4");
	//capture.open("d:/_richard/video_teste.avi");
	//capture.open("d:/_richard/video_teste2.avi");
	
	
	
	VideoWriter oVideoWriter;
	double dWidth = capture.get(CV_CAP_PROP_FRAME_WIDTH);
	double dHeight = capture.get(CV_CAP_PROP_FRAME_HEIGHT);

	//cout << "Frame Size = " << dWidth << "x" << dHeight << endl;

	Size frameSize(static_cast<int>(dWidth), static_cast<int>(dHeight));

	if(!capture.isOpened()){
		cout<<"Erro ao inicializar video\n";
		getchar();
		return -1;
	}

	
	bool initParameters = true;
	ImageProc * imageproc = NULL;

	while(capture.read(frameFeed) && !terminateApp){

		if (startImageProc) {
		
			if (!imageproc) {
				int securityArea = 10;
				Point p1;
				Point p2;

				p1.x = corner1.x;
				p1.y = corner1.y;

				p2.x = corner2.x;
				p2.y = corner2.y;

				imageproc = new ImageProc(9, box, p1, p2);
			}

			if (initParameters){
			
				if(imageproc->initBuffer(frameFeed)){
					continue;
				}

				ImageFrameBuffer * ifb = imageproc->getImageFrameBuffer();
				ifb->setFrameSize(dWidth, dHeight);
				initParameters = false;

			} else {
				
				imageproc->setFileName("d:/arquivoTeste");
				imageproc->analiseMoment(frameFeed);
			}
			

		}

		imshow("Processamento de Video",frameFeed);
		
		switch(waitKey(30)){
			case 'q':
				terminateApp = true;
				break;
			default:
				break;

		}

	}

	capture.release();

	if (imageproc->isToSaveBuffer()) {
		imageproc->saveBlockOfVideo();
	}

	capture.release();

	return 0;

}

int main() {
	return processVideo(NULL, NULL);
}