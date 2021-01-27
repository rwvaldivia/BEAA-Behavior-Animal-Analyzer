#pragma once
#include<vector>
class Box {
private:
	
public:
	std::vector <int> vetor;
	Box(void);
	~Box(void);
	std::vector <int> * getVector();
	void printVector();

};

