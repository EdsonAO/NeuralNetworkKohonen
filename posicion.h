#include <iostream>
using namespace std;

class posicion
{

public:
	int x;
	int y;
	int z;

	posicion(int x,int y,int z){
		this->x = x;
		this->y = y;
		this->z = z;
	}

	posicion(int x, int y){
		this->x = x;
		this->y = y;
		this->z = 0;
	}

	void printPoints(){
		cout<<"X="<<this->x<<" Y="<<this->y<<" Z="<<this->z<<endl;
	}
	//~posicion();
	
};