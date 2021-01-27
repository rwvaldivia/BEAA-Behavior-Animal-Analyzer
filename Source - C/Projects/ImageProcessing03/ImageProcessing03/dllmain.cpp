

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <time.h>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "ImageFrameBuffer.h"
#include "ConfigProcessVideo.h"
#include "ImageProc.h"
#include "DebugT.h"

using namespace std;
using namespace cv;

#define _MAX_IM_PROC_SIZE_PARAM_ 10


void trackbar_callback( int, void* ){}

string convertIntToStringHelper(int numero){
	std::ostringstream s_stream;
	s_stream << numero;
	return s_stream.str();

}

static void mouse_callback(int event, int x, int y, int a, void * stConfig){
	
	ConfigProcessVideo * config = ( ConfigProcessVideo *)stConfig;
	
	if (event == EVENT_LBUTTONDOWN){
		config->leftButtonMouseDown = true;
		config->corner1.x = x;
		config->corner1.y = y;

		//cout << "Corner 1 recored at " << corner1 << endl;
	}

	if (event == EVENT_LBUTTONUP){

		if (abs(x - config->corner1.x) > 10 && abs(y - config->corner1.y) > 10){
			config->leftButtonMouseUp = true;
			config->corner2.x = x;
			config->corner2.y = y;
			//cout << "Corner 2 record at " << corner2 << endl;
		} else {
			//cout << "Please select a big region" << endl;
			config->leftButtonMouseDown = false;
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
		
		config->box.width = abs(config->corner1.x - config->corner2.x);
		config->box.height = abs(config->corner1.y - config->corner2.y);

		config->box.x = min(config->corner1.x, config->corner2.x);
		config->box.y = min(config->corner1.y, config->corner2.y);
		
		config->leftButtonMouseDown		= false;
		config->leftButtonMouseUp		= false;

		if (config->typeConfig == ConfigProcessVideo::TYPE_CONFIG_PROCESS_VIDEO){
			config->startImageProc			= true;
		} else if (config->typeConfig == ConfigProcessVideo::TYPE_CONFIG_SELECT_REGIONS){
			config->isRegionSelected		= true;
		}

		
	}

}

int processVideo(char * videoFileName, char * projectNameIn, vector<SelectedRegion> * v_region, char * configPar) {

	char * file_name_video_project[1024];
	memset(file_name_video_project, '\0', 1024);
	stProcConfigParameters * configParameters = (stProcConfigParameters *)configPar;
	Mat videoFrameFeed;
	ImageProc * imageproc	= NULL;
	char * mainWindowName	= "Processamento de Video";
	bool initParameters		= true;
	bool terminateApp		= false;
	
	ConfigProcessVideo config;
	config.BLUR_SIZE			= 10;
	config.SENSITIVITY_VALUE	= 40;
	config.leftButtonMouseDown	= false;
	config.leftButtonMouseUp	= false;
	config.startImageProc		= false;
	config.mainWindowName		= mainWindowName;
	config.typeConfig			= ConfigProcessVideo::TYPE_CONFIG_PROCESS_VIDEO;

	//namedWindow("Threshold Image", 0);
	namedWindow(mainWindowName);
	setMouseCallback(mainWindowName, mouse_callback, &config);

	VideoCapture capture;
	capture.open(videoFileName);
	//capture.open("d:/_richard/19_01_R_112013090000.avi");
	//capture.open("d:/_richard/M2U00543.MPG");
	//capture.open("d:/_richard/Caixa21-43min.avi.mp4");
	//capture.open("d:/_richard/video_teste.avi");
	//capture.open("d:/_richard/video_teste2.avi");
	
	double dWidth	= capture.get(CV_CAP_PROP_FRAME_WIDTH);
	double dHeight	= capture.get(CV_CAP_PROP_FRAME_HEIGHT);

	//cout << "Frame Size = " << dWidth << "x" << dHeight << endl;

	Size frameSize(static_cast<int>(dWidth), static_cast<int>(dHeight));

	if(!capture.isOpened()){
		return -1;
	}


	for (int i = 0; i <  v_region->size(); i++){
		printf("P1X: [%d] - P1Y: [%d] - P2X[%d] - P2Y[%d] \n", v_region->at(i).p1.x, v_region->at(i).p1.y, v_region->at(i).p2.x, v_region->at(i).p2.y);
	}

	if (v_region->size() == 0){
		printf("Size> 0\n");
	}

	while(capture.read(videoFrameFeed) && !terminateApp){
		DebugT d;
		config.frameFeed = videoFrameFeed.clone();
		

		if (config.startImageProc) {
			
			if (!imageproc) {
				d.debugPrint("CRIANDO NOVO IMAGEPROC");
				int securityArea = 10;
				Point p1;
				Point p2;

				p1.x = config.corner1.x;
				p1.y = config.corner1.y;

				p2.x = config.corner2.x;
				p2.y = config.corner2.y;

				imageproc = new ImageProc(1, config.box, p1, p2);

			}

			if (initParameters){
			
				if(imageproc->initBuffer(config.frameFeed)){
					continue;
				}

				ImageFrameBuffer * ifb = imageproc->getImageFrameBuffer();
				ifb->setFrameSize(dWidth, dHeight);
				initParameters = false;

			} else {
				
				imageproc->setFileName("d:/arquivoTeste");
				imageproc->analiseMoment(config.frameFeed);

			}
			

		}

		imshow(mainWindowName, config.frameFeed);
		
		switch(waitKey(30)){
			case 'q':
			case 'Q':
				terminateApp = true;
				break;
			default:
				break;

		}

	}

	capture.release();

	if (imageproc){

		if (imageproc->isToSaveBuffer()) {
			imageproc->saveBlockOfVideo();
		}

	}

	destroyAllWindows();

	capture.release();

	return 0;

}

void convertMessageParamInToVector(vector<SelectedRegion> * vec, char * t_char_array, int size_t_char_array){
	//formato parametros
	//index - p1 - p2
	//110,500#2|30,50|111,50
	//0000000003 id
	//0000000040 p1x
	//0000000110 p1y
	//0000000040 p2x
	//0000000110 p2y
	//Terminator
	//char temp[] = "000000000100000000020000000003000000000400000000050000000000T060000000007000000000800000000090000000001T";
	int s1 = sizeof(t_char_array) - 1;
	int s2 = (sizeof(stParamJNI));
	int sizeArray = (size_t_char_array - 1) / (sizeof(stParamJNI));
	char * element;
	int mem_pos = 0;
	
	stParamJNI * stTemp;
	char buffTemp[30];
	memset(buffTemp,'\0', sizeof(buffTemp));

	for (int i = 0; i < sizeArray; i++) {
		SelectedRegion sr;
		//############################
		//Posiciona o ponteiro
		element = &t_char_array[(i * (sizeof(stParamJNI)))];
		stTemp = (stParamJNI *) element;

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

void createRegions(const char * filename, vector <SelectedRegion> * v_regiao) {

	char * mainWindowName	= "Criando regioes";
	bool terminateApp		= false;
	int indexAreaSelection	= 0;
	ImageProc imageProc;
	Scalar regionColor = cv::Scalar(0, 255,0);

	Mat videoFrameFeed;

	ConfigProcessVideo config;
	config.BLUR_SIZE			= 0;
	config.SENSITIVITY_VALUE	= 0;
	config.isRegionSelected		= false;
	config.leftButtonMouseDown	= false;
	config.leftButtonMouseUp	= false;
	config.startImageProc		= false;
	config.mainWindowName		= mainWindowName;
	config.typeConfig			= ConfigProcessVideo::TYPE_CONFIG_SELECT_REGIONS;

	namedWindow(mainWindowName);
	setMouseCallback(mainWindowName, mouse_callback, &config);

	VideoCapture capture;
	capture.open(filename);
	

	if(!capture.isOpened()){
		//############################
		//vector irá retornar vazio
		return;
	}

	while( capture.read(videoFrameFeed)&& !terminateApp ){
		
		DebugT d;
		config.frameFeed = videoFrameFeed.clone();

		if (config.isRegionSelected){
			SelectedRegion regionTemp;
			
			regionTemp.p1.x = config.corner1.x;
			regionTemp.p1.y = config.corner1.y;
			
			regionTemp.p2.x = config.corner2.x;
			regionTemp.p2.y = config.corner2.y;

			v_regiao->push_back(regionTemp);
			config.isRegionSelected = false;
		}
		
		if (v_regiao->size() > 0) {

			for (int i = 0; i < v_regiao->size(); i++) {
				v_regiao->at(i).indexRegion = i + 1;
				imageProc.drawNumberedBox(videoFrameFeed, v_regiao->at(i).indexRegion, v_regiao->at(i).p1, v_regiao->at(i).p2, regionColor);
			}

		}

		imshow(mainWindowName, videoFrameFeed);
		
		switch(waitKey(30)){
			case 'q':
				terminateApp = true;
				break;
			default:
				break;

		}

	}
	
	destroyAllWindows();
	capture.release();

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
//CODIGO PARA EXECUTAVEL INICIO
//############################################################################################################

void createRegionsHelper(){

	//convertIntToStringHelper();

	//filename = "d:/_richard/19_01_R_112013090000.avi";
	//filename = "d:/_richard/M2U00543.MPG";
	//filename = "d:/_richard/Caixa21-43min.avi.mp4";
	//filename = "d:/_richard/video_teste.avi";
	//filename = "d:/_richard/video_teste2.avi";

	//filename = "d:/_richard/19_01_R_112013090000.avi";
	//filename = "d:/_richard/19_01_R_112013090000.avi";
	//filename = "d:/_richard/M2U00543.MPG";
	//filename = "d:/_richard/Caixa21-43min.avi.mp4";
	//filename = "d:/_richard/video_teste.avi";
	//filename = "d:/_richard/video_teste2.avi";
	
	vector <SelectedRegion> vec;
	char * filename = "d:/_richard/M2U00543.MPG";

	 createRegions(filename, &vec);
	 string s_to_return = convertMessageOutParam(&vec);
	 
	 int k = 0;
	 k++;

	 //vec.clear();
	 
}

int processVideoHelper(){

	const int bufferSize = 2048;

	vector<SelectedRegion> v_region;
	char configParameters[sizeof(stProcConfigParameters)];
	memset(configParameters, '\0', sizeof(stProcConfigParameters));
	

	char regioes [] = "0000000001000000033200000001580000000495000000029300000000020000000155000000016600000003460000000273";

	int size = sizeof(regioes);

	char * param   = "000000000000000000000000000000";

	convertMessageParamInToVector(&v_region, regioes, sizeof(regioes));

	//int processVideo(char * videoFileName, char * projectNameIn, vector<SelectedRegion> * v_region, char * configPar) {
	return processVideo("d:/_richard/M2U00543.MPG", "Projeto 01", &v_region, configParameters);
}

int main(){
	processVideoHelper();
	//createRegionsHelper();
	
}
//############################################################################################################
//CODIGO PARA DLL FIM
//##################################################################star##########################################