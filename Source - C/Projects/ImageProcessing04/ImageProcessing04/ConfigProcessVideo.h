#pragma once

#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <time.h>
#include <iostream>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

class ConfigProcessVideo {
public:

	const static char TYPE_CONFIG_SELECT_REGIONS	= 'A';
	const static char TYPE_CONFIG_PROCESS_VIDEO		= 'B';

	const static char TYPE_BOX_SELECTION_NORMAL		= 'A';
	const static char TYPE_BOX_SELECTION_TOP		= 'B';

	cv::Mat frameFeed;
	cv::Rect box;

	cv::Point corner1;
	cv::Point corner2;
	
	int idProject;

	//######################################
	//Configurações de analise da imagem
	int sensibilityLevel;
	int blurLevel;
	int brightnessLevel;
	int threadGroupSession;

	//######################################
	//Controle mouse callback
	bool leftButtonMouseDown;
	bool leftButtonMouseUp;
	bool startImageProc;

	//############################################
	//Usado para verificar se deve remover um box
	bool verifyBoxColision;
	int colisionPosX;
	int colisionposY;

	char * mainWindowName;
	char typeConfig;

	//##############################################
	//Usado para verificar o tipo de selecao do box
	char typeBoxSelection;
	int sizeWidthPresetSelection;
	int sizeHeightPresetSelection;
	
	//#####################################
	//Usado para selecionar varias regioes
	bool isRegionSelected;

	//#####################################
	//Tamanho do frame de video
	long videoFrameWidth;
	long videoFrameHeight;

	//#####################################
	//Usado para controle da base de treinamento
	int databaseTrainningProcessTime;


};

struct structConfigProcessVideo {
	char threadGroupSession[10];
	char idProject[10];
	char brightnessLevel[10];
    char sensibilityLevel[10];
    char blurLevel[10];
	char sizeWidthPresetSelection[10];
	char sizeHeightPresetSelection[10];
	char typeBoxSelection[1];
	char databaseTrainningProcessTime[10];
	char terminator;
};

class SelectedRegion {
public:
	int indexRegion;
	cv::Point p1;
	cv::Point p2;

	SelectedRegion::SelectedRegion(void){
		indexRegion = 0;
	}

	SelectedRegion SelectedRegion::clone(){

		SelectedRegion r;

		r.p1.x = p1.x;
		r.p1.y = p1.y;
		r.p2.x = p2.x;
		r.p2.y = p2.y;

		return r;

	}
};




struct structSelectedRegion{
	char index[10];
	char p1x[10];
	char p1y[10];
	char p2x[10];
	char p2y[10];
};