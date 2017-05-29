#include "kohone.h"
using namespace std;

int main(){
	kohone A(4,5);

	//A.imprimirCapa();
	//A.imprimirNeuronas();
	A.readData("training2.csv",',');
	A.entrenarKohonen();
}