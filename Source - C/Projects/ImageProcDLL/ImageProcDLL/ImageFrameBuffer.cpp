#include "stdafx.h"
#include "ImageFrameBuffer.h"
#include "DebugT.h"
#include <vector>

ImageFrameBuffer::ImageFrameBuffer(): 
	ALARM_MOVEMENT_DETECTION_EVENT(FALSE)
	, FRAMES_PER_SECOND(30)
	, MAX_SIZE_OF_BUFFER_IN_SECONDS(3) {
}

ImageFrameBuffer::~ImageFrameBuffer() {
	clean();
}

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
	return static_cast<int> (ImageFrameBuffer::getFrameBuffer()->size());
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
	
	if (!isAlarmMovimentDetectionEvent()){
		if (bufferImage.size() >= (FRAMES_PER_SECOND * MAX_SIZE_OF_BUFFER_IN_SECONDS)) {
			cv::Mat m = bufferImage.at(0);
			bufferImage.erase(bufferImage.begin());
			m.release();
		}
	}
	
	bufferImage.push_back(image);

}

bool ImageFrameBuffer::shrink(){
	if (bufferImage.size() > maxSizeOfBuffer()){

		size_t elementsToRemove	= bufferImage.size() - maxSizeOfBuffer();
		//bufferImage.erase(bufferImage.begin(), bufferImage.begin() + (elementsToRemove -1));
		for (size_t i = 0 ; i < elementsToRemove; i++){
			cv::Mat m = bufferImage.at(0);//Remove sempre  o primeiro elemento
			bufferImage.erase(bufferImage.begin());
			m.release();
		}

		return true;
	}
	return false;
}


void ImageFrameBuffer::clean(){

	if (bufferImage.size() > 0){
		size_t elementsToRemove = bufferImage.size();
		for (size_t i = 0 ; i < elementsToRemove; i++){
			cv::Mat m = bufferImage.at(0);
			bufferImage.erase(bufferImage.begin());
			m.release();
		}
	}
	
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