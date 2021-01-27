#include <iostream>
#include <fstream>

#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include "ImageFrameConfig.h"


using namespace std;
using namespace cv;

bool leftButtonMouseDown = false;
bool leftButtonMouseUp = false;
bool captureROI = false;
bool morphImage = false;

Mat img;
Point corner1, corner2;
Rect box;

int H_MIN = 0;
int H_MAX = 256;
int S_MIN = 65;//0;
int S_MAX = 139;//256;
int V_MIN = 73;//0;
int V_MAX = 111;//256;
const int FRAME_WIDTH = 720;
const int FRAME_HEIGHT = 480;
const int MAX_NUM_OBJECTS=50;
const int MIN_OBJECT_AREA = 25*25;
const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH/1.5;



const string trackbarWindowName = "Trackbars";
const string windowName1 = "HSV Image";
const string windowName2 = "Thresholded Image";

void on_trackbar( int, void* ){}

void createTrackbars(){
	
    namedWindow(trackbarWindowName,0);
	char TrackbarName[50];
	sprintf( TrackbarName, "H_MIN", H_MIN);
	sprintf( TrackbarName, "H_MAX", H_MAX);
	sprintf( TrackbarName, "S_MIN", S_MIN);
	sprintf( TrackbarName, "S_MAX", S_MAX);
	sprintf( TrackbarName, "V_MIN", V_MIN);
	sprintf( TrackbarName, "V_MAX", V_MAX);
   
    createTrackbar( "H_MIN", trackbarWindowName, &H_MIN, H_MAX, on_trackbar );
    createTrackbar( "H_MAX", trackbarWindowName, &H_MAX, H_MAX, on_trackbar );
    createTrackbar( "S_MIN", trackbarWindowName, &S_MIN, S_MAX, on_trackbar );
    createTrackbar( "S_MAX", trackbarWindowName, &S_MAX, S_MAX, on_trackbar );
    createTrackbar( "V_MIN", trackbarWindowName, &V_MIN, V_MAX, on_trackbar );
    createTrackbar( "V_MAX", trackbarWindowName, &V_MAX, V_MAX, on_trackbar );
}

static void mouse_callback(int event, int x, int y, int, void *){
	
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

		Mat local_img = img.clone();
		rectangle(local_img, corner1, pt, Scalar(0,0,255));
		imshow("Cropping app", local_img);

	}

	if (leftButtonMouseDown == true && leftButtonMouseUp == true){
		
		box.width = abs(corner1.x - corner2.x);
		box.height = abs(corner1.y - corner2.y);

		box.x = min(corner1.x, corner2.x);
		box.y = min(corner1.y, corner2.y);
		
		leftButtonMouseDown = false;
		leftButtonMouseUp = false;
		captureROI = true;
		
	}

}

bool writeOnce = false;
void writeHistBGR_ToFile(float b, float g, float r){
	
	ofstream outputfile;
    outputfile.open("d:\\data.csv", ios::out | ios::app);
	
	outputfile << b << ";" << g << ";" << r << endl;
	
	outputfile.close();
}

int calcHisOpenCV(Mat imgToCalc) {
  
  Mat src;
  Mat dst;

  src = imgToCalc.clone();

  if( !src.data ) { 
	  return -1; 
  }

  /// Establish the number of bins
  int bin_size = 256;

  /// Set the ranges ( for B,G,R) )
  float range[] = { 0, 256 } ;
  const float * histRange = { range };

  bool uniform		= true; 
  bool accumulate	= false;

  Mat b_hist;
  Mat g_hist;
  Mat r_hist;

  //separa em 3 planos a imagem croped
  vector<Mat> bgr_planes;
  split( src, bgr_planes );
  
  /// Compute the histograms:
  calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &bin_size, &histRange, uniform, accumulate );
  calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &bin_size, &histRange, uniform, accumulate );
  calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &bin_size, &histRange, uniform, accumulate );

  if (writeOnce){
	  for( int i = 0; i < bin_size; i++ ) {
			writeHistBGR_ToFile(
				b_hist.at<float>(i), 
				g_hist.at<float>(i), 
				r_hist.at<float>(i)
				);
		
	  }
	  
	  writeOnce = false;
  }
  //putText(imageToPrintMessage, "Gravando Histograma",cv::Point(10,10), CV_FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255),1,8,false);
  // Draw the histograms for B, G and R
  int hist_w = 512; 
  int hist_h = 400;
  int bin_w = cvRound( (double) hist_w/bin_size );

  Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

  /// Normalize the result to [ 0, histImage.rows ]
  normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
  normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
  normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
  
  /// Draw for each channel
  for( int i = 1; i < bin_size; i++ ) {
	  
      //cout << "size: [" << histSize << "] X: " <<  << " Y: " << (b_hist.at<float>(i));
      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                       Scalar( 255, 0, 0), 2, 8, 0  );

      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                       Scalar( 0, 255, 0), 2, 8, 0  );

      line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
                       Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                       Scalar( 0, 0, 255), 2, 8, 0  );
  }

  /// Display
  namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE );
  imshow("calcHist Demo", histImage );

}

vector<ImageFrameConfig> listImageFrameConfig;

int prepareTrainningDatabase(){
	
	VideoCapture capture;
	
	capture.open("D:\\_Richard\\A 01 - Videos Ratos\\teste000001.mp4");
	int step = 15;
	int totalFrames = capture.get(CV_CAP_PROP_FRAME_COUNT);
	int captureControl = 0;

	//#############################//#############################
	//Parametros para o histograma
	
	int h_bins = 50;
	int s_bins = 60;
	int histSize[] = {h_bins, s_bins};
	
	//HUE variando de 0  a 256
	float h_ranges[] = {0, 256};
	float s_ranges[] = {0, 180};

	const float * ranges[] = {h_ranges, s_ranges};
	
	int channels[] = {0 , 1};
	//#############################//#############################

	double tempDouble = 0.0;
	Mat epilectImage;

	while (capture.read(epilectImage)){
	
		if (captureControl % step == 0){

			ImageFrameConfig imageconfigTemp;
			Mat hist_base;
			
			Mat hsv_epiletic;
			
			//###########################################################################################
			//Realiza o procedimento para calculo de histograma
			cvtColor(epilectImage,hsv_epiletic,COLOR_BGR2HSV);
			calcHist(&hsv_epiletic, 1, channels, Mat(), hist_base, 2, histSize, ranges, true, false);
			normalize(hist_base, hist_base, 0, 1, NORM_MINMAX, -1, Mat());
			
			imageconfigTemp.setMatrixND(hist_base);
			listImageFrameConfig.push_back(imageconfigTemp);
			printf("Total de imagens capturadas [%i] de um total: [%i]\n", listImageFrameConfig.size(), totalFrames);

		}

		captureControl++;
	}

	capture.release();
	return 1;

}


int compareHistMethod(Mat imgsrc){

	//#############################//#############################
	//Parametros para o histograma
	Mat hist_base;
	int h_bins = 50;
	int s_bins = 60;
	int histSize[] = {h_bins, s_bins};
	
	//HUE variando de 0  a 256
	float h_ranges[] = {0, 256};
	float s_ranges[] = {0, 180};

	const float * ranges[] = {h_ranges, s_ranges};
	
	int channels[] = {0 , 1};
	//#############################//#############################
	Mat img_candidate_nd;
	Mat hsv_img_candidate;
	ImageFrameConfig imageFrameConfigAtual;
	

	cvtColor(imgsrc, hsv_img_candidate, COLOR_BGR2HSV);
	calcHist(&hsv_img_candidate, 1, channels, Mat(), img_candidate_nd, 2, histSize, ranges, true, false);
	normalize(img_candidate_nd, img_candidate_nd, 0, 1, NORM_MINMAX, -1, Mat());

	double calc_hist_base = 0;
	double calc_hist_test = 0 ;

	double correlation		= 0;
	double chi_sqt			= 0;
	double intersect		= 0;
	double bhattacharyya	= 0;

	
	for (int i = 0; i < listImageFrameConfig.size(); i++) {

		imageFrameConfigAtual = listImageFrameConfig.at(i);
		Mat tempND = imageFrameConfigAtual.getMatrixND();

		
		correlation		= compareHist(tempND.clone(), img_candidate_nd, CV_COMP_CORREL);
		chi_sqt			= compareHist(tempND.clone(), img_candidate_nd, CV_COMP_CHISQR);
		intersect		= compareHist(tempND.clone(), img_candidate_nd, CV_COMP_INTERSECT);
		bhattacharyya	= compareHist(tempND.clone(), img_candidate_nd, CV_COMP_BHATTACHARYYA);

		printf("T [%f]: Cor: [%f]\t", (i + 1), correlation);
		printf("Chi: [%f]\t", chi_sqt);
		printf("Int: [%f]\t", intersect);
		printf("Bha: [%f]\n", bhattacharyya);

		//##################################################################################
		//Analise da saida
		
		//if (correlation >= 0.7 && chi_sqt <= 0.5 && intersect && bhattacharyya <= 5.0) {

			ofstream outputfile;
			outputfile.open("d:\\analiticx.csv", ios::out | ios::app);
			outputfile << correlation << ";" << chi_sqt << ";" << intersect << ";" << bhattacharyya << endl;
			outputfile.close();

		//}
		
		//##################################################################################
		
	}

	return 1;

}

int main(char *argv[], int argc){


	prepareTrainningDatabase();
	
	namedWindow("Cropping app");
	setMouseCallback("Cropping app", mouse_callback);
	
	VideoCapture capture;
	//capture.open("D:\\_Richard\\A 01 - Videos Ratos\\19_01_R_112013090000_cutted.mp4");
	//capture.open("D:\\_Richard\\19_01_R_112013090000.avi");
	capture.open("D:\\_Richard\\A 01 - Videos Ratos\\teste000001.mp4");
	//capture.open(0);

	capture.set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);

	//namedWindow("Cropping app");
	namedWindow("Crop");
	namedWindow("Thresholded Image");
	namedWindow("HSV Image");
	//namedWindow("Histogram Equalized", CV_WINDOW_AUTOSIZE);
	
	Mat imageHSV;
	Mat threshold;
	createTrackbars();
	double framesRemain = 0;
	double nFrames = 0;

	//Mat img_hist_equalized;

	while (char(waitKey(1)) != 'q' && capture.read(img)){
		
		
		imshow("Cropping app", img);

		nFrames = capture.get(CV_CAP_PROP_FRAME_COUNT);
		framesRemain = nFrames - capture.get(CV_CAP_PROP_POS_FRAMES);
		if (captureROI){

			//#############################################################
			//cout << " FPS/Total: [" << capture.get(CV_CAP_PROP_FPS) << "/" << nFrames << "] - Curr.Frame: [" << capture.get(CV_CAP_PROP_POS_FRAMES) << "] - Remain Frames:  [" << framesRemain  << "]" <<  endl;

			Mat crop(img, box);
			imshow("Crop", crop);
			compareHistMethod(crop);

			//#############################################################
			//equalizeHist(crop, img_hist_equalized);
			//imshow("Histogram Equalized", img_hist_equalized);
			//calcHisOpenCV(crop);

			cvtColor(crop,imageHSV,COLOR_BGR2HSV);
			inRange(imageHSV,Scalar(H_MIN,S_MIN,V_MIN),Scalar(H_MAX,S_MAX,V_MAX),threshold);

			calcHisOpenCV(imageHSV);

			if (morphImage){
				

				Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
				Mat dilateElement = getStructuringElement( MORPH_RECT,Size(8,8));

				erode(threshold,threshold,erodeElement);
				erode(threshold,threshold,erodeElement);

				dilate(threshold,threshold,dilateElement);
				dilate(threshold,threshold,dilateElement);

			}

			imshow("Thresholded Image",threshold);
			imshow("HSV Image",imageHSV);
		}

		switch (char(waitKey(1))){
			case 'm':
			case 'M':
				morphImage = !morphImage;
				break;
			case 'W':
			case 'w':
			    writeOnce = !writeOnce;
				break;

		}

	}

	capture.release();

	return 0;

}