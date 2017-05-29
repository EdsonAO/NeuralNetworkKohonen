#include <iostream>
#include <vector>
#include <math.h>
#include "posicion.h"
#include <stdlib.h>
#include <string>
#include <fstream>
#define e 2.718281828459045
using namespace std;

class kohone
{
private:
	int entradas;
	int salidas;
	vector<posicion> posiciones;
	vector<vector<float> > Neuronas;
	vector<vector<double> > data;
	float radioInicial;
	float lambda;
	float alphaInicial;

	vector<double> separar(string cadena, char separador){
		vector<double> cont;
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

	float euclidea(int posData, int posNeurona){
		float distancia=0.0;

		for(int i=0; i<entradas;i++){
			distancia +=  pow(data[posData][i] - Neuronas[posNeurona][i],2);
		}

		return sqrt(distancia);
	}

	int buscarBMU(int posData){
		int tam = salidas*salidas;

		int neu = 0;
		float minimo = euclidea(posData,0);
		for(int i=1;i<tam;i++){	
			float resDistancia = euclidea(posData,i);
			if(resDistancia<minimo){
				minimo = resDistancia;
				neu = i;
			}
		}

		return neu;
	}

	float hallarRadio(int tiempo){
		return radioInicial*pow(e,-(float)tiempo/lambda);
	}

	float distanciaVecino(int posBMU, int posNeu){
		float distancia=0.0;
		distancia +=  pow(posiciones[posBMU].x - posiciones[posNeu].x,2);
		distancia +=  pow(posiciones[posBMU].y - posiciones[posNeu].y,2);
		distancia +=  pow(posiciones[posBMU].z - posiciones[posNeu].z,2);

		return sqrt(distancia);
	}

	float getTasaAprendizaje(int tiempo){
		return alphaInicial*pow(e,-(float)tiempo/lambda); //ver cod
	}

	float getTheta(float vecinoDistancia,float radio){
		return pow(e,-pow(vecinoDistancia,2)/(2*pow(radio,2)));
	}


public:
	kohone(int entradas,int salidas){//matriz de nodos de salidas x salidas
		this->entradas = entradas;
		this->salidas = salidas;

		radioInicial = salidas/2;
		alphaInicial = 0.1;
		
		for(int i=0; i<salidas; i++){
			for(int j=0;j<salidas;j++){
				Neuronas.push_back(vector<float>(entradas));
				// leer de un archivo las posiciones de las neuronas en 3d
				posiciones.push_back(posicion(i,j));
			}
		}

		for(int i=0; i<salidas*salidas;i++){
			for(int j=0; j<entradas;j++){
				Neuronas[i][j] = (float)(rand()%100)/100;
			}
		}
	}


	void readData(string nameData,char delimitador){
			ifstream read(nameData.c_str());
			while(!read.eof()){
				char C[200];
				read.getline(C,200);
				string caracteristicas(C);

				if(caracteristicas!=""){
					vector<double> A = separar(caracteristicas,delimitador);
					data.push_back(A);
				}
			}
			read.close();
			//generar aqui de forma aleatoria

			lambda = data.size()/log(radioInicial);/////////////////////////////

		}

	void entrenarKohonen(){
		int tam = data.size();

		for(int i=0; i<tam; i++){
			int posBMU = buscarBMU(i);//buscas la neurona correcta
			float radio = hallarRadio(i+1);//radio de vecindad			

			int longNeuronas = Neuronas.size();
			for(int j=0;j<longNeuronas;j++){
				float  vecinoDistancia = distanciaVecino(posBMU,j);

				if(vecinoDistancia < radio){
					float tasaAprendizaje = getTasaAprendizaje(i+1);
					float theta = getTheta(vecinoDistancia,radio);

					for(int k=0;k<entradas;k++){
						Neuronas[j][k] += theta*tasaAprendizaje*(data[i][k] - Neuronas[j][k]);	
						
					}
				}	

			}	
			cout<<posBMU<<" "<<radio<<endl;
		}
	}

	void imprimirCapa(){
		for(int i=0; i<salidas*salidas;i++)
			posiciones[i].printPoints();
	}

	void imprimirNeuronas(){
		for(int i=0; i<salidas*salidas;i++){
			for(int j=0; j<entradas;j++){
				cout<<Neuronas[i][j]<<" ";
			}
			cout<<endl;
		}
	}
	//~kohone();
	
};