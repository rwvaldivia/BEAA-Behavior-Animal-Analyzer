#include <opencv\cv.h>
#include <opencv\highgui.h>

using namespace cv;

int main(){
	
	Mat image;
	VideoCapture cap;
	cap.open(0);
	namedWindow("window", 1);
	char key;
	bool terminate = false;
	while (!terminate) {

		cap>>image;
		imshow("window", image);
		key = waitKey(11);

		if (key == 'q'){
			terminate = true;
			cap.release();
		}
	}
}