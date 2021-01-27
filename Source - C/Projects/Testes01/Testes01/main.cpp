#include <stdio.h>
#include <iostream>
#include "Box.h"
#include <vector>
using namespace std;

int main(){

	Box b1;
	vector <int> * v = b1.getVector();
	v->push_back(10);
	v->push_back(20);

	for (int i = 0; i < v->size() + 2; i++){
		printf ("element[%d]: \n", v->at(i)); 
	}

	printf ("size[%d]: \n", v->size()); 

	v->erase(v->begin());
	v->erase(v->begin());

	printf ("---------------------\n");

	b1.printVector();
	printf ("size[%d]: \n", v->size()); 

	char c;
	cin>>c;
	
	return 0;

}