#include "Box.h"


Box::Box(void)
{
	vetor.push_back(1);
	vetor.push_back(2);
	vetor.push_back(3);
	vetor.push_back(4);
	vetor.push_back(5);
	vetor.push_back(6);
}


Box::~Box(void)
{
}

std::vector <int>* Box::getVector(){
	return &vetor;
}
void Box::printVector(){
	for (int i = 0; i < vetor.size(); i++){
		printf ("element[%d]: \n", vetor.at(i)); 
	}
}