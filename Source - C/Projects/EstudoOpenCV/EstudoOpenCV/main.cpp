

#include <iostream>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

#include <stdio.h>

using namespace std;
using namespace cv;

//Global
const int slider_max = 1;
int slider;
Mat img;

void on_trackbar(int pos, void *) {

	printf("on_trackbar called...\n");

	Mat img_converted;
	if (pos > 0){
		cvtColor(img, img_converted, CV_RGB2GRAY);
	} else {
		img_converted = img;
	}

	imshow("Trackbar app", img_converted);

}

int example01(char *argv[], int argc){

	img = imread("D:\\_Richard\\richard.jpg");
	namedWindow("Trackbar app");
	imshow("Trackbar app", img);
	slider = 0;

	createTrackbar("RGB <--> Grayscale", "Trackbar app", &slider, slider_max, on_trackbar);
	
	while (char(waitKey(1)) != 'q'){}

	return 0;
}

int main(char *argv[], int argc){
	
	return example01(argv, argc);

}


