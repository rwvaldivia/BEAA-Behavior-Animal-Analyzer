#include "stdafx.h"
#include "ImageProc.h"
#include "DebugT.h"

//Construtores
ImageProc::ImageProc(int i, Rect b, Point p1, Point p2): 
	indexAreaSelection(i)
	, boxArea(b)
	, imageBox1(p1)
	, imageBox2(p2)
	, movementDetectionInProgress(false)
	, movementEventOcurrencies(5)
	, eventMovimentCounter(0)
	, initVideoForSave(true)
	, motionCounter(0)
	, blockFrameCounter(0)
	, saveBuffer(false)
	, MIN_EVENT_OCORRENCIES_ALLOWED_TO_SAVE(5)//20
	, SENSITIVITY_VALUE(15)//40
	, BLUR_SIZE (10){
	
	//Codigo do construtor
}

ImageProc::ImageProc(): 
	movementDetectionInProgress(false)
	, movementEventOcurrencies(5)
	, eventMovimentCounter(0)
	, initVideoForSave(true)
	, motionCounter(0)
	, blockFrameCounter(0)
	, saveBuffer(false)
	, MIN_EVENT_OCORRENCIES_ALLOWED_TO_SAVE(20)
	, SENSITIVITY_VALUE(20)//40
	, BLUR_SIZE (10){
}

ImageProc::~ImageProc(void){}

void trackbar_callbackImageProc( int, void* ){}


ImageFrameBuffer * ImageProc::getImageFrameBuffer(){
	return &buffer;
}

Rect ImageProc::getBoxArea(){
	return boxArea;
}

void ImageProc::setMotionCounter(int c){
	motionCounter = c;
}

void ImageProc::setSaveBuffer(bool b){
	saveBuffer = b;
}
int ImageProc::isToSaveBuffer(){
	return saveBuffer;
}


int ImageProc::getMotionCounter(){
	return motionCounter;
}

void ImageProc::setBlockFrameCounter(int b){
	blockFrameCounter = b;
}

int ImageProc::getBlockFrameCounter(){
	return blockFrameCounter;
}


int ImageProc::getMovementEventOcurrencies(){
	return movementEventOcurrencies;
}

void ImageProc::setMovementEventOcurrencies(int m){
	movementEventOcurrencies = m;
}

bool ImageProc::isMovementDetectionInProgress(){
	return movementDetectionInProgress;
}

void ImageProc::setMovementDetectionInProgress(bool m){
	movementDetectionInProgress = m;
}


void ImageProc::setSensivityValue(int s){
	SENSITIVITY_VALUE = s;
}

int ImageProc::getSensivityValue(){
	return SENSITIVITY_VALUE;
}

void ImageProc::setBlurSize(int s){
	BLUR_SIZE = s;
}

int ImageProc::getBlurSize(){
	return BLUR_SIZE;
}


void ImageProc::setBoxArea(Rect box){
	boxArea = box;
}

bool ImageProc::initBuffer(cv::Mat image){
	buffer.addFrame(image.clone());
	return !buffer.isBufferFull();
}

//############################
//Metodo principal do programa
void ImageProc::analiseMoment(cv::Mat image) {
	cv::Mat imageToProcess = image.clone();
	bool hasMotion = processSingleImage(imageToProcess);

	if (hasMotion){
		//############################
		//Prepara para chamadas posteriores e permite preecher
		//o buffer de imagens
		setMovementDetectionInProgress(true);
	}

	stringstream ss;
	ss << (indexAreaSelection);
	string imageBoxNumber = ss.str();

	int textPosX = 0;
	if (indexAreaSelection > 9){
		textPosX = imageBox2.x - 23;
	}  else {
		textPosX = imageBox2.x - 15;
	}
	
	if (isMovementDetectionInProgress()){
		rectangle(image, imageBox1, imageBox2, Scalar(0,0,255));
		cv::putText(image, imageBoxNumber,cv::Point(textPosX, imageBox2.y - 10), FONT_HERSHEY_TRIPLEX, 0.5, cv::Scalar(0,0,255),1,8,false);

	} else {
		rectangle(image, imageBox1, imageBox2, Scalar(0,255,0));
		cv::putText(image, imageBoxNumber,cv::Point(textPosX, imageBox2.y - 10), FONT_HERSHEY_TRIPLEX, 0.5, cv::Scalar(0,255,0),1,8,false);
	}

	if (hasMotion || isMovementDetectionInProgress()) {
		//#########################################
		//Houve um evento. Necessário manter buffer
		//Controle para manter o buffer carregado
		buffer.setAlarmMovimentDetectionEvent(true);

		if (hasMotion){
			setMotionCounter(getMotionCounter() + 1);
		}

		setBlockFrameCounter(getBlockFrameCounter() + 1);
		

		//printf("Contagem de blocos: [%d] - contagem de movimento: [%d]", getBlockFrameCounter(), getMotionCounter());

		//#########################################
		//caso passe 2 segundo e meio de video
		//preparar a gravacao do video
		if (getBlockFrameCounter() >= 60) {
			
			if (getMotionCounter() >= MIN_EVENT_OCORRENCIES_ALLOWED_TO_SAVE){
				setSaveBuffer(true);
				printf("Alcancou o minimo de imagens detectadas\n");
			} else {
				
				if (isToSaveBuffer()){
					saveBlockOfVideo();
					setSaveBuffer(false);
				}
					//###############################################
					//Encolhe o vide e deixa ele do tamanho do buffer
					buffer.shrink();
					printf("shrink!\n");
			}

			//####################################################
			//reinicializo o bloco e analiso mais 1 segundo e meio
			setBlockFrameCounter(0);
			setMotionCounter(0);
			buffer.setAlarmMovimentDetectionEvent(false);
			setMovementDetectionInProgress(false);
		}

	}

	buffer.addFrame(imageToProcess.clone());

}

void ImageProc::saveBlockOfVideo(){

	if (buffer.size() <= 0){
		return;
	}

	printf("Salvando video - inicio\n");
	string videoFileName = getFileName() + "_.avi" ;
					
	if (initVideoForSave){
		videoWriter  = VideoWriter(videoFileName, CV_FOURCC('D', 'I', 'V', '3'), 30, buffer.getFrameSize(), true);
		initVideoForSave = false;
	}

	stringstream ss;
	ss << (++eventMovimentCounter);

	string strTextEvent = "EVENTO " + ss.str();
	//###################################
	//Grava 3 segundos de video separador
	cv::Mat * mTemp = new cv::Mat( cv::Mat::zeros(buffer.getFrameHeight(), buffer.getFrameWidth(), buffer.getImageAt(0).type()) );
	cv::putText(*mTemp, strTextEvent,cv::Point(5,(buffer.getFrameHeight() / 2)), CV_FONT_HERSHEY_DUPLEX, 2, cv::Scalar(255),1,8,false);

	for (int i = 0; i < 60; i++){
		videoWriter.write(mTemp->clone());
	}
	delete mTemp;
	
	cv::Mat im;
	for (int i = 0; i < buffer.size(); i++){
		im = buffer.getImageAt(i).clone();
		rectangle(im, imageBox1, imageBox2, Scalar(0,0,255));
		videoWriter.write(im);
	}

}

//############################
//Analise se uma imagem possui diferença entre com relacao
//a anterior.
bool ImageProc::processSingleImage(const cv::Mat image) {
	
	DebugT d;

	Rect box = getBoxArea();

	vector<cv::Mat> * vtemp = buffer.getFrameBuffer();
	cv::Mat lastImageBuffer	= vtemp->at(vtemp->size() - 1 );

	cv::Mat graylastImageBufferCrop;
	cv::Mat grayCurrentImageCrop;
	
	cv::Mat differenceImage;
	cv::Mat thresholdImage;


	double alpha = 2.0;
	double beta = 15;//15
	
	
	
	//################################################################
	//Recorta as imagens na selecao do usuario
	cv::Mat lastImageBufferCrop(lastImageBuffer.clone(), box);
	cv::Mat currentImageCrop(image, box);


	//Mat new_image;// = Mat::zeros( image.size(), image.type() );
	//new_image = alpha * image.clone() + beta;
	//Mat new_LastImageBuffer;
	//new_LastImageBuffer = alpha + lastImageBuffer.clone() + beta;
	//cv::Mat lastImageBufferCrop(new_LastImageBuffer, box);
	//cv::Mat currentImageCrop(new_image, box);
	//d.debugImage(new_image, "1");
	//d.debugImage(new_LastImageBuffer, "2");


	
	d.debugImage(lastImageBufferCrop, "L");
	d.debugImage(currentImageCrop, "A");


	//################################################################
	//Converte as imagens para grayscale
	cv::cvtColor(lastImageBufferCrop  ,graylastImageBufferCrop ,COLOR_BGR2GRAY);
	cv::cvtColor(currentImageCrop     ,grayCurrentImageCrop    ,COLOR_BGR2GRAY);
	

	//TENTAR COM ERODE E DILATE
	//equalizeHist( graylastImageBufferCrop, graylastImageBufferCrop );
	//equalizeHist( grayCurrentImageCrop, grayCurrentImageCrop );

	d.debugImage(graylastImageBufferCrop, "L");
	d.debugImage(grayCurrentImageCrop, "G");


	//################################################################
	//Aplica a diferenca entre as duas imagens e gera o threshold

	int s = ImageProc::getSensivityValue();
	cv::absdiff(graylastImageBufferCrop, grayCurrentImageCrop, differenceImage);
	cv::threshold(differenceImage, thresholdImage, ImageProc::getSensivityValue(), 255, THRESH_BINARY);

	int b = ImageProc::getBlurSize();
	cv::blur(thresholdImage,thresholdImage,cv::Size(ImageProc::getBlurSize(),ImageProc::getBlurSize()));
	cv::threshold(thresholdImage,thresholdImage,ImageProc::getSensivityValue(),255,THRESH_BINARY);

	d.debugImage(thresholdImage, "T");
	createTrackbar( "Sensibilidade", "T", &SENSITIVITY_VALUE, 30, trackbar_callbackImageProc );
	createTrackbar( "Blur", "T", &BLUR_SIZE, 60, trackbar_callbackImageProc );

	//d.debugPrintI("SENSITIVITY_VALUE",SENSITIVITY_VALUE); 
	//d.debugPrintI("BLUR_SIZE",BLUR_SIZE);

	
	//################################################################
	//REFATORAR ESSE TRECHO DO CODIGO
	Mat temp;
	thresholdImage.copyTo(temp);

	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;

	//################################################################
	//Obtem a lista de contornos
	findContours(temp,contours,hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE );

	if(contours.size() > 0) {
		return true;
	} else {
		return false;
	}

}

void ImageProc::drawNumberedBox(cv::Mat framefeed, int indexRectangleArea, Point p1a, Point p2a, Scalar regionColor){

	Point p1 = p1a;
	Point p2 = p2a;
	Point ptemp;

	stringstream ss;
	ss << (indexRectangleArea);
	string imageBoxNumber = ss.str();

	if (p1a.y > p2a.y){
		ptemp = p1;
		p1 = p2;
		p2 = ptemp;
	}

	int textPosX = 0;
	if (indexRectangleArea > 9){
		textPosX = p2.x - 23;
	}  else {
		textPosX = p2.x - 15;
	}

	rectangle(framefeed, p1, p2, regionColor);
	cv::putText(framefeed, imageBoxNumber,cv::Point(textPosX, p2.y - 10), FONT_HERSHEY_TRIPLEX, 0.5, regionColor ,1,8,false);

}

void ImageProc::setFileName(string name){
	fileName = name;
}

string ImageProc::getFileName(){
	return fileName;
}
