#include <iostream>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

using namespace std;
using namespace cv;

bool ldown = false, lup = false;

Mat img;
Point corner1, corner2;
Rect box;

static void mouse_callback(int event, int x, int y, int, void *){
	
	if (event == EVENT_LBUTTONDOWN){
		ldown = true;
		corner1.x = x;
		corner2.y = y;

		cout << "Corner 1 recored at " << corner1 << endl;
	}

	if (event == EVENT_LBUTTONUP){

		if (abs(x - corner1.x) > 20 && abs(y - corner1.y) > 20){
			lup = true;
			corner1.x = x;
			corner1.y = y;
		
			cout << "Corner 2 record at " << corner2 << endl;
		} else {
			cout << "Please select a big region" << endl;
			ldown = false;
		}
	}

	if (ldown == true && lup == false){
		Point pt;
		pt.x = x;
		pt.y = y;

		Mat local_img = img.clone();
		rectangle(local_img, corner1, pt, Scalar(0,0,255));
		imshow("Cropping app", local_img);
	
	}

	if (ldown == true && lup == true){
		
		box.width = abs(corner1.x - corner2.x);
		box.height = abs(corner1.y - corner2.y);

		box.x = min(corner1.x, corner2.x);
		box.y = min(corner1.y, corner2.y);

		Mat crop(img, box);
		namedWindow("Crop");
		imshow("Crop", crop);
		ldown = false;
		lup = false;
		

		//##################################
		//Iterando entre as linhas e colunas
		Mat imgTest = crop.clone();
		namedWindow("ImageTest");
		int rows = imgTest.rows;
		int cols = imgTest.cols;
		int j = 0, i = 0;
		int color = -1;
	
		for (i = 0 ; i < rows; i++){
			for (j = 0; j < cols; j ++){
				if (imgTest.at<Vec3b>(i, j)[0] != 255 && imgTest.at<Vec3b>(i, j)[1] != 255 && imgTest.at<Vec3b>(i, j)[2] != 255){
					imgTest.at<Vec3b>(i, j)[0] = 0;
					imgTest.at<Vec3b>(i, j)[1] = 0;
					imgTest.at<Vec3b>(i, j)[2] = 0;
				}
			}
		}

		imshow("ImageTest", imgTest);



	
	}


}

int main(char *argv[], int argc){

	img = imread("D:\\_Richard\\richard.jpg");
	namedWindow("Cropping app");
	imshow("Cropping app", img);

	setMouseCallback("Cropping app", mouse_callback);

	while (char(waitKey(1)) != 'q'){}

	return 0;

}