#include "kohone.h"
using namespace std;


vector<vector<float> > data;
vector<clasificacion> Y;


vector<float> separar(string cadena, char separador){
		vector<float> cont;
		cadena += ";";
		int tam = cadena.size();
		int atras = 0;
		for(int i=0; i<tam; i++){
			if(cadena[i]==separador){
				string cad = cadena.substr(atras,i-atras);
				cont.push_back(strtof((cad).c_str(),0));
				atras = i+1;
			}
			else if(cadena[i]==';'){
				string cad = cadena.substr(atras,i-atras);
				cont.push_back(strtof((cad).c_str(),0));
				atras = i+1;
			}
		}
		return cont;
	}


void readData(string nameData,char delimitador){
			ifstream read(nameData.c_str());
			while(!read.eof()){
				char C[200];
				read.getline(C,200);
				string caracteristicas(C);

				if(caracteristicas!=""){
					vector<float> A = separar(caracteristicas,delimitador);
					int clas = A[A.size()-1];
					A.pop_back();

					Y.push_back(clas);
					data.push_back(A);
				}
			}
			read.close();
			//generar aqui de forma aleatoria

}

int main(){
	kohone A(4,30);

	//A.imprimirCapa();
	//A.imprimirNeuronas();
	A.readData("training2.csv",',');
	//A.normalizar();//ver si es conveniente para los datos
	A.entrenarKohonen();
	A.imprimirClasificacion();

/*
	while(true){
		vector<float>Cons;
		float val;
		for(int i=0;i<4;i++){
			cin>>val;
			Cons.push_back(val);
		}
		A.consultar(Cons);	
	}
*/	
	readData("training2.csv",',');
	//
	//
	for(int i=0; i<data.size();i++){
		A.consultar(data[i]);
	}

}