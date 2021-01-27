#pragma once

#include <opencv\cv.h>
#include <vector>
#include <string>

using namespace std;
using namespace cv;

#ifndef __ImageFrameBufferObj__
#define __ImageFrameBufferObj__

//##########################################
//Ao inicilializar a classe e necessario adicionar o tamanho do frame

class ImageFrameBuffer {

private:
		//################################
		//Configuracoes iniciais do buffer
		const int FRAMES_PER_SECOND;
		const int MAX_SIZE_OF_BUFFER_IN_SECONDS;
		
		//################################
		//Variaveis de instancia
		bool ALARM_MOVEMENT_DETECTION_EVENT;
		std::vector<cv::Mat> bufferImage;
		double frameWidth;
		double frameHeight;
		Size frameSize;

public:
		ImageFrameBuffer();
		~ImageFrameBuffer();

		void addFrame(cv::Mat image);
		bool isBufferFull();
		int maxSizeOfBuffer();
		int size();

		std::vector<cv::Mat> * getFrameBuffer();
		cv::Mat * getCurrentImage();

		void setAlarmMovimentDetectionEvent(bool);
		bool isAlarmMovimentDetectionEvent();
		bool shrink();
		void setFrameSize(double width, double height);
		Size getFrameSize();
		cv::Mat getImageAt(int);
		double getFrameWidth();
		double getFrameHeight();
		void clean();

		
};

#endif