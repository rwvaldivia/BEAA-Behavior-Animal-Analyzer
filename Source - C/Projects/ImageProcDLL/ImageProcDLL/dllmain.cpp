//############################################################################################################
//CODIGO PARA DLL INICIO
//############################################################################################################
// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "br_com_protowork_engine_ImageProcessThread.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <fstream>

#include "ImageFrameBuffer.h"
#include "ConfigProcessVideo.h"
#include "ImageProc.h"
#include "DebugT.h"

using namespace std;
using namespace cv;


//############################################################################################################
//CODIGO PARA DLL INICIO
//############################################################################################################

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

//############################################################################################################
//CODIGO PARA DLL FIM
//############################################################################################################

#define _FRAMES_PER_SECOND_IMAGE_PROC_ 30
#define _MAX_IM_PROC_SIZE_PARAM_ 10
#define _DIRETORIO_PADRAO_PROJETOS_ "C:/imageproc/Projetos/"
#define _DIRETORIO_PADRAO_TREINAMENTO_DB_ "C:/imageproc/database/"
bool global_llock_critical_session = false;
bool global_showImageWindow = false;

void trackbar_callback( int, void* ){}


bool dectecClickOnRegion(Point p1, Point p2, int pcx, int pcy){
	
	//#########################################
	//Cuidado
	if (p1.x == pcx && p1.y == pcy){
		return false;
	}

	if (pcx >= p1.x && pcx <= p2.x && pcy >= p1.y && pcy <= p2.y){
		return true;
	}

	return false;
}


string convertIntToStringHelper(int numero){
	std::ostringstream s_stream;
	s_stream << numero;
	return s_stream.str();

}

static void mouse_callback(int evt, int x, int y, int a, void * stConfig){
	
	DebugT d;
	ConfigProcessVideo * config = ( ConfigProcessVideo *)stConfig;
	
	if (evt == EVENT_LBUTTONDOWN){
		config->leftButtonMouseDown = true;
		config->corner1.x = x;
		config->corner1.y = y;

		//cout << "Corner 1 recored at " << corner1 << endl;
	}

	if (evt == EVENT_LBUTTONUP){ 

		if (abs(x - config->corner1.x) > 10 && abs(y - config->corner1.y) > 10 && 
			config->typeBoxSelection != ConfigProcessVideo::TYPE_BOX_SELECTION_TOP ) {

			config->leftButtonMouseUp = true;
			config->corner2.x = x;
			config->corner2.y = y;
			//cout << "Corner 2 record at " << corner2 << endl;

		} else {
			//cout << "Please select a big region" << endl;
			
			if (config->typeConfig == ConfigProcessVideo::TYPE_CONFIG_SELECT_REGIONS) {

				config->verifyBoxColision = true;
				

				if (config->typeBoxSelection == ConfigProcessVideo::TYPE_BOX_SELECTION_TOP ) {

					config->leftButtonMouseDown		= true;
					config->leftButtonMouseUp		= true;

					//config->corner2.x = config->corner1.x + 47;
					//config->corner2.y = config->corner1.y + 24;

					config->corner2.x = config->corner1.x + config->sizeWidthPresetSelection;
					config->corner2.y = config->corner1.y + config->sizeHeightPresetSelection;

					config->colisionPosX = config->corner1.x;
					config->colisionposY = config->corner1.y;

				} else {

					config->leftButtonMouseDown = false;
					config->colisionPosX = x;
					config->colisionposY = y;

				}
			}

		}
	}

	if (config->leftButtonMouseDown == true && config->leftButtonMouseUp == false){
		Point pt_local;
		pt_local.x = x;
		pt_local.y = y;

		Mat local_img = config->frameFeed.clone();
		rectangle(local_img, config->corner1, pt_local, Scalar(0,0,255));
		imshow(config->mainWindowName, local_img);

	}

	if (config->leftButtonMouseDown == true && config->leftButtonMouseUp == true){
		
		//printf("x1:[%d] - y1:[%d] - x2:[%d] - y2:[%d]\n", config->corner1.x, config->corner1.y, config->corner2.x, config->corner2.y);


		if (config->corner1.x < 0){
			config->corner1.x = 2;
		}

		if (config->corner1.y < 0){
			config->corner1.y = 2;
		}

		if (config->corner2.x > config->videoFrameWidth){
			config->corner2.x = config->videoFrameWidth - 2;
		}

		if (config->corner2.y > config->videoFrameHeight){
			config->corner2.y = config->videoFrameHeight - 2;
		}

		config->box.width	= abs(config->corner1.x - config->corner2.x);
		config->box.height	= abs(config->corner1.y - config->corner2.y);

		config->box.x		= min(config->corner1.x, config->corner2.x);
		config->box.y		= min(config->corner1.y, config->corner2.y);
		
		config->leftButtonMouseDown		= false;
		config->leftButtonMouseUp		= false;

		if (config->typeConfig == ConfigProcessVideo::TYPE_CONFIG_PROCESS_VIDEO){
			config->startImageProc			= true;
		} else if (config->typeConfig == ConfigProcessVideo::TYPE_CONFIG_SELECT_REGIONS){
			config->isRegionSelected		= true;
		}

		
	}

}

void look_critical_session(){
	while(global_llock_critical_session){
		//wait for unlook;
	}
	global_llock_critical_session = true;
}

void unlook_critical_session(){
	global_llock_critical_session = false;
}

int convertBufferToInt(char * value, int size){
	char buffTemp[11];
	memset(buffTemp,'\0', sizeof(buffTemp));
	memcpy(buffTemp, value, size);
	return atoi(buffTemp);
}

int processVideo(char * videoFileName, char * projectNameIn, vector<SelectedRegion> * v_region, char * configPar) {
	
	//const clock_t begin_time = std::clock();
	//time_t start_t, end_t;
    //double diff_t;
    //time(&start_t);

	int delayShowImageInFrames = 15;
	int countDelay = 0;
	int const NAME_WINDOW_SIZE = 100;
	static char terminateThreadWindow[NAME_WINDOW_SIZE];

	//#############################################
	//Formato nome do arquivo: Projeto01_001_hhmmss
	char file_name_video_project[1024];
	memset(file_name_video_project, '\0', 1024);

	time_t now	= time(0);
	tm * ltm	= localtime(&now);
	int hour	= ltm->tm_hour;
	int min		= ltm->tm_min;
	int sec		= ltm->tm_sec;

	SelectedRegion sreg = v_region->at(0);
	int indexRegion = sreg.indexRegion;

	sprintf_s(file_name_video_project, "%s%s/%s_%03d_%02d%02d%02d", _DIRETORIO_PADRAO_PROJETOS_, projectNameIn, projectNameIn, indexRegion, hour, min, sec);

	//#############################################
	//Inicializa parametros
	structConfigProcessVideo * configParameters = (structConfigProcessVideo *)configPar;

	char buffTemp[11];
	memset(buffTemp,'\0', sizeof(buffTemp));
	memcpy(buffTemp, configParameters->threadGroupSession, sizeof(configParameters->threadGroupSession));
	int threadGroupSession = atoi(buffTemp);

	Mat videoFrameFeed;

	char mainWindowName[NAME_WINDOW_SIZE];
	memset(mainWindowName, '\0', sizeof(mainWindowName));
	sprintf_s(mainWindowName, "Processamento[Thread Session ID: %00d]", threadGroupSession);

	bool initParameters		= true;
	bool terminateApp		= false;
	
	ConfigProcessVideo config;
	config.blurLevel			= 10;
	config.sensibilityLevel		= 40;
	config.threadGroupSession	= threadGroupSession;
	config.leftButtonMouseDown	= false;
	config.leftButtonMouseUp	= false;
	config.startImageProc		= false;
	config.mainWindowName		= mainWindowName;
	config.typeConfig			= ConfigProcessVideo::TYPE_CONFIG_PROCESS_VIDEO;
	config.typeBoxSelection		= ConfigProcessVideo::TYPE_BOX_SELECTION_NORMAL;

	//setMouseCallback(mainWindowName, mouse_callback, &config);
	
	//##############################################
	//Se habilitar, abre uma janela para cada thread
	//namedWindow(mainWindowName);
	
	VideoCapture capture;
	capture.open(videoFileName);
	
	if(!capture.isOpened()){
		return -1;
	}
	
	double dWidth	= capture.get(CV_CAP_PROP_FRAME_WIDTH);
	double dHeight	= capture.get(CV_CAP_PROP_FRAME_HEIGHT);

	ImageProc * imageproc	= NULL;
	Rect box;
	box.width	= abs(sreg.p1.x - sreg.p2.x);
	box.height	= abs(sreg.p1.y - sreg.p2.y);


	box.x = (sreg.p1.x > sreg.p2.x) ? sreg.p2.x: sreg.p1.x;
	box.y = (sreg.p1.y > sreg.p2.y) ? sreg.p2.y: sreg.p1.y;
	
	//box.x = min(sreg.p1.x, sreg.p2.x);
	//box.y = min(sreg.p1.y, sreg.p2.y);

	imageproc = new ImageProc(sreg.indexRegion, box, sreg.p1, sreg.p2, &config);
	
	bool initparam = false;

	while(capture.read(videoFrameFeed) && !terminateApp){

		config.frameFeed = videoFrameFeed.clone();
		
		//if (config.startImageProc) {
		//	
		//	if (!imageproc) {
		//		d.debugPrint("CRIANDO NOVO IMAGEPROC");
		//		int securityArea = 10;
		//		Point p1;
		//		Point p2;

		//		p1.x = config.corner1.x;
		//		p1.y = config.corner1.y;

		//		p2.x = config.corner2.x;
		//		p2.y = config.corner2.y;

		//		printf("p1x: [%d]\n", p1.x);
		//		printf("p1y: [%d]\n", p1.y);
		//		printf("p2x: [%d]\n", p2.x);
		//		printf("p2y: [%d]\n", p2.y);

		//		printf("Box.x: [%d]\n", config.box.x);
		//		printf("Box.y: [%d]\n", config.box.y);

		//		printf("Box.height: [%d]\n", config.box.height);
		//		printf("Box.width: [%d]\n", config.box.width);

		//		imageproc = new ImageProc(1, config.box, p1, p2);

		//	}

		//	if (initParameters){
		//	
		//		if(imageproc->initBuffer(config.frameFeed)){
		//			continue;
		//		}

		//		ImageFrameBuffer * ifb = imageproc->getImageFrameBuffer();
		//		ifb->setFrameSize(dWidth, dHeight);
		//		initParameters = false;

		//	} else {

		//		imageproc->setFileName(file_name_video_project);
		//		imageproc->analiseMoment(config.frameFeed);

		//	}
		//	

		//}

		if (!initparam){
		
			if(imageproc->initBuffer(config.frameFeed)){
				continue;
			}
			
			ImageFrameBuffer * ifb1 = imageproc->getImageFrameBuffer();
			ifb1->setFrameSize(dWidth, dHeight);
			imageproc->setFileName(file_name_video_project);
			initparam = true;

		}
		
		imageproc->analiseMoment(config.frameFeed);
		
		if (global_showImageWindow) {

			if (countDelay % delayShowImageInFrames == 0){
				look_critical_session();
				imshow(mainWindowName, config.frameFeed);
				unlook_critical_session();
			}

			++countDelay;

			//##############################################
			//Previne overflow - apenas preciosismo
			if (countDelay >= delayShowImageInFrames * 30){
				countDelay = 0;
			} 

		} else {
			destroyAllWindows();
		}
		
		//#####################################################
		//Notifica todas as Threads que devem terminar
		if (memcmp(terminateThreadWindow, mainWindowName, sizeof(mainWindowName)) == 0 ){
			terminateApp = true;
		}
		
		switch(waitKey(30)){
			case 'q':
			case 'Q':
			case 27://Tecla esc
				terminateApp = true;
				memset(terminateThreadWindow, '\0', sizeof(terminateThreadWindow));
				sprintf_s(terminateThreadWindow, "%s", mainWindowName);
				break;
			default:
				break;

		}

	}

	if (imageproc){

		if (imageproc->isToSaveBuffer()) {
			imageproc->saveBlockOfVideo();
		}

		imageproc->closeVideWriter();
		imageproc->getImageFrameBuffer()->clean();

	}

	destroyAllWindows();
	capture.release();

   //time(&end_t);
   //diff_t = difftime(end_t, start_t);
   //const clock_t end_time = std::clock();
	
   //printf("inicio: [%d] - fim: [%d] - Time Elapsed:[%d] s TG: [%d] IDReg: [%d] \n", begin_time, end_time, ((end_time - begin_time) /  CLOCKS_PER_SEC), config.threadGroupSession, sreg.indexRegion); 
   //printf("Time Elapsed:[%f] s TG: [%d] IDReg: [%d] \n", diff_t, config.threadGroupSession, sreg.indexRegion); 

	return 0;

}

int previewVideo(char * videoFileName, vector<SelectedRegion> * v_region) {

	SelectedRegion sreg;

	Mat videoFrameFeed;

	char mainWindowName[30];
	memset(mainWindowName, '\0', sizeof(mainWindowName));
	sprintf_s(mainWindowName, "Preview");

	VideoCapture capture;
	capture.open(videoFileName);
	
	if(!capture.isOpened()){
		return -1;
	}
	

	ImageProc imageproc;
	bool terminateApp = false;

	while(capture.read(videoFrameFeed) && !terminateApp){
		
		for (int i = 0; i < v_region->size(); i ++){
			sreg = v_region->at(i);
			imageproc.drawNumberedBox(videoFrameFeed, sreg.indexRegion, sreg.p1, sreg.p2, Scalar(255,0,0));
		}

		imshow(mainWindowName, videoFrameFeed);
		videoFrameFeed.release();	
		switch(waitKey(30)){
			case 'q':
			case 'Q':
			case 27://Tecla esc
				terminateApp = true;
				break;
			default:
				break;

		}

	}

	capture.release();
	destroyAllWindows();
	capture.release();

	return 0;

}

void convertMessageParamInToVector(vector<SelectedRegion> * vec, char * t_char_array, int size_t_char_array){

	int s1 = sizeof(t_char_array) - 1;
	int s2 = (sizeof(structSelectedRegion));
	int sizeArray = (size_t_char_array) / (sizeof(structSelectedRegion));
	char * element;
	int mem_pos = 0;
	
	structSelectedRegion * stTemp;
	char buffTemp[30];
	memset(buffTemp,'\0', sizeof(buffTemp));

	for (int i = 0; i < sizeArray; i++) {
		SelectedRegion sr;
		//############################
		//Posiciona o ponteiro
		element = &t_char_array[(i * (sizeof(structSelectedRegion)))];
		stTemp = (structSelectedRegion *) element;

		//############################
		//Indice da Regiao
		memset(buffTemp,'\0', sizeof(buffTemp));//Nao esta sobrando
		memcpy(buffTemp, stTemp->index, sizeof(stTemp->index));
		sr.indexRegion = atoi(buffTemp);
		memset(buffTemp,'\0', sizeof(buffTemp));

		//############################
		//P1
		memcpy(buffTemp, stTemp->p1x, sizeof(stTemp->p1x));
		sr.p1.x = atoi(buffTemp);
		memset(buffTemp,'\0', sizeof(buffTemp));

		memcpy(buffTemp, stTemp->p1y, sizeof(stTemp->p1y));
		sr.p1.y = atoi(buffTemp);
		memset(buffTemp,'\0', sizeof(buffTemp));

		//############################
		//P2
		memcpy(buffTemp, stTemp->p2x, sizeof(stTemp->p2x));
		sr.p2.x = atoi(buffTemp);
		memset(buffTemp,'\0', sizeof(buffTemp));

		memcpy(buffTemp, stTemp->p2y, sizeof(stTemp->p2y));
		sr.p2.y = atoi(buffTemp);
		memset(buffTemp,'\0', sizeof(buffTemp));

		vec->push_back(sr);

	}

}

void createRegions(const char * filename, vector <SelectedRegion> * v_regiao, char * configPar) {

	char * mainWindowName	= "Criando regioes";
	bool terminateApp		= false;
	int indexAreaSelection	= 0;
	ImageProc imageProc;
	Scalar regionColor = cv::Scalar(0, 255,0);

	Mat videoFrameFeed;
	
	structConfigProcessVideo * configParameters = (structConfigProcessVideo *)configPar;
	

	ConfigProcessVideo config;
	config.blurLevel			= 0;
	config.sensibilityLevel		= 0;
	config.isRegionSelected		= false;
	config.leftButtonMouseDown	= false;
	config.leftButtonMouseUp	= false;
	config.startImageProc		= false;
	config.mainWindowName		= mainWindowName;
	config.typeConfig			= ConfigProcessVideo::TYPE_CONFIG_SELECT_REGIONS;
	
	config.verifyBoxColision	= false;
	config.colisionPosX			= 0;
	config.colisionposY			= 0;

	char buffTemp[11];
	memset(buffTemp,'\0', sizeof(buffTemp));
	memcpy(buffTemp, configParameters->sizeHeightPresetSelection, sizeof(configParameters->sizeHeightPresetSelection));
	int intTemp = atoi(buffTemp);
	config.sizeHeightPresetSelection	= intTemp;

	memset(buffTemp,'\0', sizeof(buffTemp));
	memcpy(buffTemp, configParameters->sizeWidthPresetSelection, sizeof(configParameters->sizeWidthPresetSelection));
	intTemp = atoi(buffTemp);
	config.sizeWidthPresetSelection		= intTemp;

	config.typeBoxSelection				= configParameters->typeBoxSelection[0];

	namedWindow(mainWindowName);
	setMouseCallback(mainWindowName, mouse_callback, &config);

	VideoCapture capture;
	capture.open(filename);
	

	if(!capture.isOpened()){
		//############################
		//vector irá retornar vazio
		return;
	}

	config.videoFrameWidth  = capture.get(CV_CAP_PROP_FRAME_WIDTH);
	config.videoFrameHeight = capture.get(CV_CAP_PROP_FRAME_HEIGHT);

	while( capture.read(videoFrameFeed)&& !terminateApp ){
		
		DebugT d;
		SelectedRegion regionTemp;

		config.frameFeed = videoFrameFeed.clone();

		if (config.isRegionSelected){
			
			
			regionTemp.p1.x = config.corner1.x;
			regionTemp.p1.y = config.corner1.y;
			
			regionTemp.p2.x = config.corner2.x;
			regionTemp.p2.y = config.corner2.y;

			v_regiao->push_back(regionTemp);
			config.isRegionSelected = false;
		}
		
		if (v_regiao->size() > 0) {

			if (config.verifyBoxColision) {
				bool isDectecClickOnRegion = false;

				for (int i = 0; i < v_regiao->size(); i++) {
					isDectecClickOnRegion = dectecClickOnRegion(v_regiao->at(i).p1, v_regiao->at(i).p2, config.colisionPosX, config.colisionposY);
					if (isDectecClickOnRegion){
						v_regiao->erase(v_regiao->begin() + i);
						break;
					}
					
				}


				if ( config.typeBoxSelection == ConfigProcessVideo::TYPE_BOX_SELECTION_TOP ) {
				    //##################################################
					//Remove o ultimo elemento inserido pois trata-se de
					//do click em uma região e nao pode inserir outra regiao
					if (isDectecClickOnRegion){
						v_regiao->erase(v_regiao->begin() + v_regiao->size() -1);
					}
				}

				config.colisionPosX			= 0;
				config.colisionposY			= 0;
				config.verifyBoxColision	= false;
			}

			for (int i = 0; i < v_regiao->size(); i++) {
				v_regiao->at(i).indexRegion = i + 1;
				imageProc.drawNumberedBox(videoFrameFeed, v_regiao->at(i).indexRegion, v_regiao->at(i).p1, v_regiao->at(i).p2, regionColor);
			}

		}

		imshow(mainWindowName, videoFrameFeed);
		
		switch(waitKey(30)){
			case 'q':
			case 'Q':
			case 27://Tecla esc
				terminateApp = true;
				break;
			default:
				break;

		}

	}
	
	destroyAllWindows();
	capture.release();

}

long createDataBaseTrainning(const char * filename, SelectedRegion * sectedRegion, const char * filenameDatabase, char * configPar) {
	DebugT d;

	char * mainWindowName	= "Matrix all";
	char * imageCropWindow	= "Matrix";
	bool terminateApp		= false;
	bool debugMode			= false;
	ImageProc imageProc;

	structConfigProcessVideo * configParameters = (structConfigProcessVideo *)configPar;

	char bufferTemp[sizeof(configParameters->databaseTrainningProcessTime) + 1];
	memset(bufferTemp, '\0', sizeof(bufferTemp));
	memcpy(bufferTemp, configParameters->databaseTrainningProcessTime, sizeof(configParameters->databaseTrainningProcessTime));
	int trainningProcessTime = atoi(bufferTemp);

	int totalFramesToProcess = _FRAMES_PER_SECOND_IMAGE_PROC_ * trainningProcessTime;
	int totalFramesToProcessControl = 1;

	//bufferTemp[sizeof(configParameters->databaseTrainningProcessStep) + 1];
	memset(bufferTemp, '\0', sizeof(bufferTemp));
	memcpy(bufferTemp, configParameters->databaseTrainningProcessStep, sizeof(configParameters->databaseTrainningProcessStep));
	int trainningProcessStep = atoi(bufferTemp);
	int trainningProcessStepControl = 0;

	Rect box;
	box.width	= abs(sectedRegion->p1.x - sectedRegion->p2.x);
	box.height	= abs(sectedRegion->p1.y - sectedRegion->p2.y);

	box.x = (sectedRegion->p1.x > sectedRegion->p2.x) ? sectedRegion->p2.x: sectedRegion->p1.x;
	box.y = (sectedRegion->p1.y > sectedRegion->p2.y) ? sectedRegion->p2.y: sectedRegion->p1.y;

	Mat videoFrameFeed;
	
	//namedWindow(mainWindowName);

	VideoCapture capture;
	capture.open(filename);
	
	if(!capture.isOpened()){
		//############################
		//vector irá retornar vazio
		return 0;
	}

	long videoFrameWidth  = capture.get(CV_CAP_PROP_FRAME_WIDTH);
	long videoFrameHeight = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
	long totalframecounter = 0;

	ofstream myfile;
	string s;
	std::stringstream strTemp;
	strTemp << _DIRETORIO_PADRAO_TREINAMENTO_DB_ << filenameDatabase;
	//myfile.open (strTemp.str(), ios::app); //permite append
	myfile.open (strTemp.str());

	while( capture.read(videoFrameFeed)&& !terminateApp ){
		++totalframecounter;


		cv::Mat grayImageCrop;

		//################################################################
		//Recorta as imagens na selecao do usuario
		cv::Mat colorImageCroped(videoFrameFeed.clone(), box);

		//################################################################
		//Converte as imagens para grayscale
		cv::cvtColor(colorImageCroped, grayImageCrop, COLOR_BGR2GRAY);
		
		if (debugMode) {
			imshow(mainWindowName, videoFrameFeed);
			imshow(imageCropWindow, grayImageCrop);
		}

		unsigned int intensityValue;

		long size = grayImageCrop.rows * grayImageCrop.cols;
		for(int j = 0 ; j < grayImageCrop.rows; j++) {

			for (int i = 0; i < grayImageCrop.cols; i++) {

				intensityValue = grayImageCrop.at<uchar>(j,i);
				//printf("%d", intensityValue);
				myfile << intensityValue;
				
				if (i < grayImageCrop.cols -1){
					myfile << ", ";
				} else {
					myfile << " ROTULO: 1\n";
				}

			}
		}
		myfile << " \n---------------------------------------------------\n\n";
		
		if (totalFramesToProcessControl >= totalFramesToProcess){
			terminateApp = true;
		}

		++totalFramesToProcessControl;

		switch(waitKey(30)){
			case 'q':
			case 'Q':
			case 'd':
			case 'D':
				debugMode = !debugMode;
				break;
			case 27://Tecla esc
				terminateApp = true;
				break;
			default:
				break;

		}

	}

	myfile.close();
	destroyAllWindows();
	capture.release();

	return totalframecounter;

}

string paddingZeros(int vl){

	char buffer[(_MAX_IM_PROC_SIZE_PARAM_ + 1)];
	std::stringstream ss;
	sprintf_s(buffer, "%010d", vl);
	ss << buffer;
	return ss.str();

}

string convertMessageOutParam(vector<SelectedRegion> * v_region ) {

	std::stringstream ss;
	string s_temp;
	
	for (int i = 0; i < v_region->size(); i++){

		SelectedRegion sr = v_region->at(i);

		ss	<< paddingZeros( sr.indexRegion)
			<< paddingZeros(sr.p1.x)
			<< paddingZeros(sr.p1.y)
			<< paddingZeros(sr.p2.x)
			<< paddingZeros(sr.p2.y);

	}

	return ss.str();
}

//############################################################################################################
//CODIGO PARA DLL INICIO
//############################################################################################################

void convertJstringToCharArrayHelper(JNIEnv * env, jstring strToConvert, char * buffer, const int sizebuffer){

	const char * nativeString = env->GetStringUTFChars(strToConvert, 0);
	memset(buffer, '\0', sizebuffer);
	memcpy(buffer, nativeString, strlen(nativeString));
	env->ReleaseStringUTFChars(strToConvert, nativeString);

}


JNIEXPORT jlong JNICALL Java_br_com_protowork_engine_ImageProcessThread_databaseTrainning
	(JNIEnv * env, jobject thisObject, jstring filename_in, jstring regionParametesIn, jstring databaseTrainingFilenameIn, jstring configParametersIn){
	
	//############################################################
	//rwv
	DebugT d;
	const int maxSizeBuffer = 2048;
	const char * local_filename = env->GetStringUTFChars(filename_in, 0);
	const char * local_filename_trainningDatabase = env->GetStringUTFChars(databaseTrainingFilenameIn, 0);

	vector<SelectedRegion> v_region;

	//####################################################
	//Tratamento para regiao selecionada
	char regionParameters[maxSizeBuffer];
	convertJstringToCharArrayHelper(env, regionParametesIn, regionParameters, maxSizeBuffer);
	int sizeRegion = strlen(regionParameters);
	convertMessageParamInToVector(&v_region, regionParameters, sizeRegion);
	SelectedRegion selectedRegion = v_region.at(0);

	//####################################################
	//Tratamento para regiao selecionada
	char configParameters[sizeof(structConfigProcessVideo)];
	convertJstringToCharArrayHelper(env, configParametersIn, configParameters, sizeof(structConfigProcessVideo));


	return createDataBaseTrainning(local_filename, &selectedRegion, local_filename_trainningDatabase, configParameters);


}

JNIEXPORT jint JNICALL Java_br_com_protowork_engine_ImageProcessThread_processImage
  (JNIEnv * env, jobject, jstring videoFileNameIn, jstring projectNameIn, jstring regionParametesIn, jstring configParametersIn){

		//##################################
		//formato parametros
		//1|30,40|110,500*
		
		
		const int maxSizeBuffer = 2048;
		const int minSizeBuffer	= 100;

		char videoFileName[maxSizeBuffer];
		convertJstringToCharArrayHelper(env, videoFileNameIn, videoFileName, maxSizeBuffer);
		
		char projectName[maxSizeBuffer];
		convertJstringToCharArrayHelper(env, projectNameIn, projectName, maxSizeBuffer);

		char regionParameters[maxSizeBuffer];


		convertJstringToCharArrayHelper(env, regionParametesIn, regionParameters, maxSizeBuffer);

		char configParameters[sizeof(structConfigProcessVideo)];
		convertJstringToCharArrayHelper(env, configParametersIn, configParameters, sizeof(structConfigProcessVideo));

		vector<SelectedRegion> v_region;
		int sizeRegion = strlen(regionParameters);
		convertMessageParamInToVector(&v_region, regionParameters, sizeRegion);
		
		return processVideo(videoFileName, projectName, &v_region, configParameters);
		
}

JNIEXPORT jint JNICALL Java_br_com_protowork_engine_ImageProcessThread_previewRegions
	(JNIEnv * env, jobject obj, jstring videoFileNameIn, jstring regionParametesIn){

		const int maxSizeBuffer = 2048;
		const int minSizeBuffer	= 100;

		char videoFileName[maxSizeBuffer];
		convertJstringToCharArrayHelper(env, videoFileNameIn, videoFileName, maxSizeBuffer);
		
		char regionParameters[maxSizeBuffer];
		convertJstringToCharArrayHelper(env, regionParametesIn, regionParameters, maxSizeBuffer);

		vector<SelectedRegion> v_region;
		int sizeRegion = strlen(regionParameters);
		convertMessageParamInToVector(&v_region, regionParameters, sizeRegion);
		
		return previewVideo(videoFileName, &v_region);

}





JNIEXPORT jstring JNICALL Java_br_com_protowork_engine_ImageProcessThread_createRegions
	(JNIEnv * env , jobject thisObject, jstring filename_in, jstring configParametersIn){

	
	//###########################################
	//Transforma a variavel de entrada em char *
	//Inicia o processamento principal
	vector <SelectedRegion> vec;
	const char * local_filename = env->GetStringUTFChars(filename_in, 0);

	char configParameters[sizeof(structConfigProcessVideo)];
	convertJstringToCharArrayHelper(env, configParametersIn, configParameters, sizeof(structConfigProcessVideo));



	createRegions(local_filename, &vec, configParameters);
	string s_to_return = convertMessageOutParam(&vec);

	env->ReleaseStringUTFChars(filename_in, NULL);
	jstring result = env->NewStringUTF(s_to_return.c_str());

	return result;
}

JNIEXPORT jint JNICALL Java_br_com_protowork_engine_ImageProcessThread_showWindowProcessImage
	(JNIEnv *, jobject){
		global_showImageWindow = !global_showImageWindow;
	return 1;
}


JNIEXPORT jint JNICALL JNI_OnUnLoad(JavaVM *vm, void *reserved){
	
	return JNI_VERSION_1_2;
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
	return JNI_VERSION_1_2;
}




//############################################################################################################
//CODIGO PARA DLL FIM
//############################################################################################################

