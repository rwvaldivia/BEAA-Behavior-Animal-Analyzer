
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <time.h>
#include <Windows.h>﻿
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
bool captureROI				= false;
bool morphImage				= false;
bool startImageProc			= false;

Mat frame1;
Point corner1, corner2;
Rect box;

Point boxCoord01;
Point boxCoord02;

//our sensitivity value to be used in the absdiff() function
//for higher sensitivity, use a lower value
//const static int SENSITIVITY_VALUE = 40;
int SENSITIVITY_VALUE = 40;//size of blur used to smooth the intensity image output from absdiff() function
//const static int BLUR_SIZE = 10;
int BLUR_SIZE = 10;
//these two can be toggled by pressing 'd' or 't'
bool debugMode;
bool trackingEnabled;

void trackbar_callback( int, void* ){}


static void mouse_callback(int event, int x, int y, int a, void * b){
	
	if (event == EVENT_LBUTTONDOWN){
		captureROI = false;
		leftButtonMouseDown = true;
		corner1.x = x;
		corner1.y = y;

		cout << "Corner 1 recored at " << corner1 << endl;
	}

	if (event == EVENT_LBUTTONUP){

		if (abs(x - corner1.x) > 10 && abs(y - corner1.y) > 10){
			leftButtonMouseUp = true;
			corner2.x = x;
			corner2.y = y;
		
			cout << "Corner 2 record at " << corner2 << endl;
		} else {
			cout << "Please select a big region" << endl;
			leftButtonMouseDown = false;
		}
	}

	if (leftButtonMouseDown == true && leftButtonMouseUp == false){
		Point pt;
		pt.x = x;
		pt.y = y;

		boxCoord01.x = corner1.x;
		boxCoord01.y = corner1.y;

		boxCoord02.x = pt.x;
		boxCoord02.y = pt.y;

		Mat local_img = frame1.clone();
		rectangle(local_img, corner1, pt, Scalar(0,0,255));
		imshow("Frame1", local_img);

	}

	if (leftButtonMouseDown == true && leftButtonMouseUp == true){
		
		box.width = abs(corner1.x - corner2.x);
		box.height = abs(corner1.y - corner2.y);

		box.x = min(corner1.x, corner2.x);
		box.y = min(corner1.y, corner2.y);
		
		leftButtonMouseDown = false;
		leftButtonMouseUp = false;
		captureROI = true;

		startImageProc = true;
		
	}

}


string intToString(int number){

	std::stringstream ss;
	ss << number;
	return ss.str();
}

string getDateTime(){

	SYSTEMTIME theTime;
	GetLocalTime(&theTime);

	string dateTime;

	string year = intToString(theTime.wYear);


	std::stringstream m;
	m<<std::setfill('0')<<std::setw(2)<< theTime.wMonth;
	string month = m.str();
	
	std::stringstream d;
	d<<std::setfill('0')<<std::setw(2)<< theTime.wDay;
	string day = d.str();
	
	std::stringstream hr;
	hr<<setfill('0')<<std::setw(2)<<theTime.wHour;
	string hour = hr.str();
	
	std::stringstream min;
	min<<setfill('0')<<std::setw(2)<<theTime.wMinute;
	string minute = min.str();
	
	std::stringstream sec;
	sec<<setfill('0')<<std::setw(2)<<theTime.wSecond;
	string second = sec.str();

	dateTime = year + "-" + month + "-" + day + "  " + hour + ":" + minute + ":" + second;

	return dateTime;
}

string getDateTimeForFile(){

	
	SYSTEMTIME theTime;
	GetLocalTime(&theTime);
	string dateTime;

	string year = intToString(theTime.wYear);

	std::stringstream m;
	m<<std::setfill('0')<<std::setw(2)<< theTime.wMonth;
	string month = m.str();

	std::stringstream d;
	d<<std::setfill('0')<<std::setw(2)<< theTime.wDay;
	string day = d.str();

	std::stringstream hr;
	hr<<setfill('0')<<std::setw(2)<<theTime.wHour;
	string hour = hr.str();

	std::stringstream min;
	min<<setfill('0')<<std::setw(2)<<theTime.wMinute;
	string minute = min.str();

	std::stringstream sec;
	sec<<setfill('0')<<std::setw(2)<<theTime.wSecond;
	string second = sec.str();

	dateTime = year + "_" + month + "_" + day + "_" + hour + "h" + minute + "m" + second + "s";

	return dateTime;
}



bool detectMotion(Mat thresholdImage, Mat &cameraFeed){

	bool motionDetected = false;

	Mat temp;
	thresholdImage.copyTo(temp);

	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;

	findContours(temp,contours,hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE );// retrieves external contours

	if(contours.size()>0)motionDetected=true;
	else motionDetected = false;

	return motionDetected;

}
int main(){

	namedWindow("Threshold Image", 0);
	namedWindow("Frame1");
	setMouseCallback("Frame1", mouse_callback);

	int inc					= 0;
	bool terminateApp		= false;
	bool recording			= false;
	bool startNewRecording	= false;
	bool firstRun			= true;
	bool motionDetected		= false;
	bool pause				= false;

	debugMode				= false;
	trackingEnabled			= false;

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

	cout << "Frame Size = " << dWidth << "x" << dHeight << endl;

	Size frameSize(static_cast<int>(dWidth), static_cast<int>(dHeight));

	Point pt1, pt2;
	pt1.x = 0;
	pt1.y = 40;
	pt2.x = dWidth - 10;
	pt2.y = dHeight - 10;

	rectangle(frame1, pt1, pt2, Scalar(0,0,255));
	/*
	Rect box;
	box.width	= abs(pt1.x - pt2.x);
	box.height	= abs(pt1.y - pt2.y);
	box.x = min(pt1.x, pt2.x);
	box.y = min(pt1.y, pt2.y);
	*/
	int x1 = 600;
	int y1 = 1;
	int x2 = 750;
	int y2 = 480;
	

	Point p1(x1, y1);
	Point p2(x2, y2);


	if(!capture.isOpened()){
		cout<<"Erro ao inicializar video\n";
		getchar();
		return -1;
	}

	
	bool initParameters = true;
	ImageProc * imageproc = NULL;

	while(capture.read(frame1) && !terminateApp){

		if (startImageProc) {
		
			if (!imageproc) {
				int securityArea = 10;
				Point p1;
				Point p2;

				p1.x = boxCoord01.x;
				p1.y = boxCoord01.y;

				p2.x = boxCoord02.x;
				p2.y = boxCoord02.y;

				imageproc = new ImageProc(9, box, p1, p2);
			}

			if (initParameters){
			
				if(imageproc->initBuffer(frame1)){
					continue;
				}

				ImageFrameBuffer * ifb = imageproc->getImageFrameBuffer();
				ifb->setFrameSize(dWidth, dHeight);
				initParameters = false;

			} else {
				
				imageproc->setFileName("d:/arquivoTeste");
				imageproc->analiseMoment(frame1);
			}
			

		}

		imshow("Frame1",frame1);
		
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


	if (true){
		return 0;
	}
	
	//################################################################################
	//###############################################################################
	//################################################################################
	//###############################################################################




	while(capture.read(frame1) && !terminateApp){
		
		

		if(startNewRecording==true){

			oVideoWriter  = VideoWriter("D:/_Richard/MyVideo"+intToString(inc)+".avi", CV_FOURCC('D', 'I', 'V', '3'), 20, frameSize, true); 
			recording = true;
			startNewRecording = false;
			cout<<"New video file created D:/_Richard/MyVideo"+intToString(inc)+".avi "<<endl;

			if ( !oVideoWriter.isOpened() ) 
			{
				cout << "ERROR: Failed to initialize video writing" << endl;
				getchar();
				return -1;
			}

		}
		
		//imageToCropTemp = frame1;
		cv::rectangle(frame1, p1, p2, Scalar(255, 255, 255),CV_FILLED, 8, 0);


		if (captureROI){
			
			Mat crop1(frame1, box);
			imshow("Crop", crop1);
			cv::cvtColor(crop1,grayImage1,COLOR_BGR2GRAY);
			
			
			capture.read(frame2);
			cv::rectangle(frame2, p1, p2, Scalar(255, 255, 255),CV_FILLED, 8, 0);
			Mat crop2(frame2, box);

			cv::cvtColor(crop2,grayImage2,COLOR_BGR2GRAY);
			cv::absdiff(grayImage1,grayImage2,differenceImage);
			cv::threshold(differenceImage,thresholdImage,SENSITIVITY_VALUE,255,THRESH_BINARY);

			if(debugMode==true){
				cv::imshow("Difference Image",differenceImage);
				cv::imshow("Threshold Image", thresholdImage);
				//resizeWindow("Threshold Image",400,400);
				createTrackbar( "Sensibilidade", "Threshold Image", &SENSITIVITY_VALUE, 30, trackbar_callback );
				createTrackbar( "Blur", "Threshold Image", &BLUR_SIZE, 60, trackbar_callback );
			}else{
				cv::destroyWindow("Difference Image");
				cv::destroyWindow("Threshold Image");
			}

			cv::blur(thresholdImage,thresholdImage,cv::Size(BLUR_SIZE,BLUR_SIZE));
			cv::threshold(thresholdImage,thresholdImage,SENSITIVITY_VALUE,255,THRESH_BINARY);


		} else {

			//Mat cropImage1(frame1, box);
			//imshow("Cropping app", cropImage1);
			//convert frame1 to gray scale for frame differencing
			cv::cvtColor(frame1,grayImage1,COLOR_BGR2GRAY);
			//cv::cvtColor(cropImage1,grayImage1,COLOR_BGR2GRAY);
		
			capture.read(frame2);
			cv::rectangle(frame2, p1, p2, Scalar(255, 255, 255),CV_FILLED, 8, 0);
			//Mat cropImage2(frame2, box);

			cv::cvtColor(frame2,grayImage2,COLOR_BGR2GRAY);
			//cv::cvtColor(cropImage2,grayImage2,COLOR_BGR2GRAY);

			cv::absdiff(grayImage1,grayImage2,differenceImage);
			cv::threshold(differenceImage,thresholdImage,SENSITIVITY_VALUE,255,THRESH_BINARY);

			if(debugMode==true){
				cv::imshow("Difference Image",differenceImage);
				cv::imshow("Threshold Image", thresholdImage);
				createTrackbar( "Sensibilidade", "Threshold Image", &SENSITIVITY_VALUE, 30, trackbar_callback );
				createTrackbar( "Blur", "Threshold Image", &BLUR_SIZE, 60, trackbar_callback );
			}else{
				cv::destroyWindow("Difference Image");
				cv::destroyWindow("Threshold Image");
			}

			cv::blur(thresholdImage,thresholdImage,cv::Size(BLUR_SIZE,BLUR_SIZE));
			cv::threshold(thresholdImage,thresholdImage,SENSITIVITY_VALUE,255,THRESH_BINARY);
		}

		if(debugMode==true){
			imshow("Final Threshold Image",thresholdImage);
		} else {
			cv::destroyWindow("Final Threshold Image");
		}

		if(trackingEnabled){
			motionDetected = detectMotion(thresholdImage,frame1);
		}else{ 
			recording = false;
			motionDetected = false;
		}

		rectangle(frame1,Point(0,460),Point(200,480),Scalar(255,255,255),-1);
		putText(frame1,getDateTime(),Point(0,480),1,1,Scalar(0,0,0),2);

		if(recording){

			oVideoWriter.write(frame1);
			putText(frame1,"REC",Point(0,60),2,2,Scalar(0,0,255),2);
		}

		if(motionDetected){

			putText(frame1,"Deteccao de Movimento",cv::Point(0,420),2,2,cv::Scalar(0,255,0));
			recording = true;

			if(firstRun == true){

				string videoFileName = "D:/"+getDateTimeForFile()+".avi";
				cout << "Arquivo preparado para escrita: " << videoFileName<<endl;
				oVideoWriter  = VideoWriter(videoFileName, CV_FOURCC('D', 'I', 'V', '3'), 20, frameSize, true);

				if ( !oVideoWriter.isOpened() ) 
				{
					cout << "Erro ao gravar video em disco" << endl;
					getchar();
					return -1;
				}
				firstRun = false;
			}

		} else {
			recording = false;
		}

		imshow("Frame1",frame1);

		switch(waitKey(30)){

		case 27: //'esc' key has been pressed, exit program.
			return 0;
		case 116: //'t' has been pressed. this will toggle tracking
			trackingEnabled = !trackingEnabled;
			if(trackingEnabled == false) cout<<"Tracking disabled."<<endl;
			else cout<<"Tracking enabled."<<endl;
			break;
		case 100: //'d' has been pressed. this will debug mode
			debugMode = !debugMode;
			if(debugMode == false) cout<<"Debug mode disabled."<<endl;
			else cout<<"Debug mode enabled."<<endl;
			break;
		case 112: //'p' has been pressed. this will pause/resume the code.
			pause = !pause;
			if(pause == true){ cout<<"Code paused, press 'p' again to resume"<<endl;
				while (pause == true){
					//stay in this loop until 
					switch (waitKey()){
						//a switch statement inside a switch statement? Mind blown.
					case 112: 
						//change pause back to false
						pause = false;
						cout<<"Code Resumed"<<endl;
						break;
					}
				}
			}

		case 114:
			//'r' has been pressed.
			recording =!recording;

			if(firstRun == true){

				cout << "New Recording Started" << endl;
				oVideoWriter  = VideoWriter("D:/MyVideo0.avi", CV_FOURCC('D', 'I', 'V', '3'), 20, frameSize, true);

				if ( !oVideoWriter.isOpened() ) {
					cout << "ERROR: Failed to initialize video writing" << endl;
					getchar();
					return -1;
				}
				firstRun = false;
			}
			else {
				if (!recording) { 
					cout << "Recording Stopped" << endl;
				} else {
					cout << "Recording Started" << endl;
				}
			}
			break;

		case 110:
			//'n' has been pressed
			//start new video file
			startNewRecording = true;
			cout << "New Recording Started" << endl;
			//increment video file name
			inc+=1;
			break;

		case 'q':
			terminateApp = true;

		}

		

	}

	capture.release();

	return 0;

}