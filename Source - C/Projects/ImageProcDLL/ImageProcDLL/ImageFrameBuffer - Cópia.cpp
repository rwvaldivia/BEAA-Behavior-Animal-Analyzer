#include "stdafx.h"
#include "ImageFrameBuffer.h"
#include "DebugT.h"
#include <vector>

ImageFrameBuffer::ImageFrameBuffer(): 
	ALARM_MOVEMENT_DETECTION_EVENT(FALSE)
	, FRAMES_PER_SECOND(30)
	, MAX_SIZE_OF_BUFFER_IN_SECONDS(2) {
	
	//Codigo do construtor
}

ImageFrameBuffer::~ImageFrameBuffer() {}

cv::Mat * ImageFrameBuffer::getCurrentImage(){
	return & bufferImage.at(bufferImage.size() - 1);
}

void ImageFrameBuffer::setAlarmMovimentDetectionEvent(bool evt){
	ALARM_MOVEMENT_DETECTION_EVENT = evt;
}

bool ImageFrameBuffer::isAlarmMovimentDetectionEvent(){
	return ALARM_MOVEMENT_DETECTION_EVENT;
}

int ImageFrameBuffer::size(){
	return ImageFrameBuffer::getFrameBuffer()->size();
}

std::vector<cv::Mat> * ImageFrameBuffer::getFrameBuffer(){
	return &bufferImage;
}

bool ImageFrameBuffer::isBufferFull() {
	if (bufferImage.size() >= maxSizeOfBuffer()) {
		return true;
	}
	return false;
}

int ImageFrameBuffer::maxSizeOfBuffer(){
	return ImageFrameBuffer::FRAMES_PER_SECOND * ImageFrameBuffer::MAX_SIZE_OF_BUFFER_IN_SECONDS;
}

void ImageFrameBuffer::addFrame(cv::Mat image){
	
	static int x = 0;
	++x;
	DebugT d;

	/*
	if (bufferImage.size() == 0 ){
		lastImage		= image;
		currentImage	= image;
	} else {
		lastImage		= currentImage;
		currentImage	= image;
	}
	*/
	if (!isAlarmMovimentDetectionEvent()){
		if (bufferImage.size() >= (FRAMES_PER_SECOND * MAX_SIZE_OF_BUFFER_IN_SECONDS)){
			bufferImage.erase(bufferImage.begin());
		}
	}
	
	bufferImage.push_back(image);

}

bool ImageFrameBuffer::shrink(){
	if (bufferImage.size() > maxSizeOfBuffer()){
		int elementsToRemove	= bufferImage.size() - maxSizeOfBuffer();
		bufferImage.erase(bufferImage.begin(), bufferImage.begin() + (elementsToRemove -1));
		return true;
	}
	return false;
}

void ImageFrameBuffer::setFrameSize(double width, double height){

		frameWidth	= width;
		frameHeight = height;
		 
		Size f(static_cast<int>(width), static_cast<int>(height));
		frameSize = f;
}

Size ImageFrameBuffer::getFrameSize(){
	return frameSize;
}

cv::Mat ImageFrameBuffer::getImageAt(int index){
	return bufferImage.at(index);
}

double ImageFrameBuffer::getFrameWidth(){
	return frameWidth;
}
double ImageFrameBuffer::getFrameHeight(){
	return frameHeight;
}