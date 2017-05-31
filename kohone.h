#include <iostream>
#include <vector>
#include <math.h>
#include "posicion.h"
#include <stdlib.h>
#include <string>
#include <fstream>
#define e 2.718281828459045
using namespace std;

typedef int clasificacion;

class kohone
{
private:
	int entradas;
	int salidas;
	vector<posicion> posiciones;
	vector<vector<float> > Neuronas;
	vector<clasificacion> Clasificacion;

	vector<vector<float> > data;
	vector<int>dataSeleted;
	vector<clasificacion> Y;

	float radioInicial;
	float lambda;
	float alphaInicial;
	int numIteraciones;

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

	float euclidea(int posData, int posNeurona){
		float distancia=0.0;

		for(int i=0; i<entradas;i++){
			distancia += pow(data[posData][i] - Neuronas[posNeurona][i],2);
		}

		return sqrt(distancia);
	}

	int buscarBMU(int posData){
		int tam = salidas*salidas;//Es en 2D
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
		//return alphaInicial*pow(e,-(float)tiempo/lambda); //ver cod cambiarlo por numero de iterciones 
		return alphaInicial*pow(e,-(float)tiempo/numIteraciones);//que esta disminuyendo
	}	

	float getTheta(float vecinoDistancia,float radio){
		return pow(e,-pow(vecinoDistancia,2)/(2*pow(radio,2)));
	}


	int elegirDataRandom(){
		//int valsel = rand()%dataSeleted.size();//
		//return valsel;

		int valsel = rand()%dataSeleted.size();
		int valor =dataSeleted[valsel];

		dataSeleted.erase(dataSeleted.begin()+valsel);
		
		return valor;
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
				Clasificacion.push_back(0);
			}
		}

		for(int i=0; i<salidas*salidas;i++){
			for(int j=0; j<entradas;j++){
				Neuronas[i][j] = (float)(rand()%100)/100;
			}
		}
	}

	void readData(string nameData,char delimitador){
			int i=0;
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
					dataSeleted.push_back(i);
					i++;
				}
			}
			read.close();
			//generar aqui de forma aleatoria

			numIteraciones = data.size();///////////////////////////
			lambda = numIteraciones/log(radioInicial);/////////////////////////////

	}

	void normalizar(){
		for(int i = 0;i<entradas;i++){
			int min = data[0][i];
			int max = data[0][i];

			for(int j=1;j<data.size();j++){
				if(data[j][i]<min)min = data[j][i];
				if(data[j][i]>max)max = data[j][i];
			}

			//normalizar...
			for(int k=0;k<data.size();k++){
				data[k][i] = (data[k][i] - min)/(max - min);
			}
		}

	}

	void entrenarKohonen(){
		//int tam = data.size();
		int timei=1;
		//for(int i=0; i<tam; i++){// cantidad de datos de entrenamiento
		while(numIteraciones>0){
			//elegir la data aleatoria
			int i = elegirDataRandom();
			//
			int posBMU = buscarBMU(i);//buscas la neurona correcta (in: posicion del vector de entrenamiento) bn
			float radio = hallarRadio(timei);//radio de vecindad del BMU

			int longNeuronas = Neuronas.size();
			for(int j=0;j<longNeuronas;j++){//recorre por la cuadricula de neuronas
				float  vecinoDistancia = distanciaVecino(posBMU,j);//calcula la distancia entre el BMU y la neurona

				if(vecinoDistancia < radio){//verifica si la neurona esta dentro del radio establecido
					float tasaAprendizaje = getTasaAprendizaje(timei);//(L)--revisar
					float theta = getTheta(vecinoDistancia,radio);//revisar

					for(int k=0;k<entradas;k++){
						Neuronas[j][k] += theta*tasaAprendizaje*(data[i][k] - Neuronas[j][k]);	
					}

					Clasificacion[j] = Y[i];
				}	

			}
			timei++;	
			numIteraciones--;
			cout<<i<<endl;
		}
			//cout<<posBMU<<" "<<radio<<endl;
		//}
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

	void imprimirClasificacion(){
		for(int i=0 ; i<salidas ; i++){
			for(int j=0 ; j<salidas ; j++){
				cout<<Clasificacion[i*salidas+j]<<" ";
			}
			cout<<endl;
		}
	}

	void consultar(vector<float>&Enter){

		int tam = salidas*salidas;//Es en 2D
		int neu = 0;

		float minimo = distanciaConsulta(Enter,0);
		for(int i=1;i<tam;i++){	
			float resDistancia = distanciaConsulta(Enter,i);
			if(resDistancia<minimo){
				minimo = resDistancia;
				neu = i;
			}
		}

		cout<<Clasificacion[neu]<<endl;
	}

	float distanciaConsulta(vector<float>&Enter,int posNeurona){
		float dis=0.0;
		for(int i=0;i<entradas;i++){
			dis+=pow(Enter[i]-Neuronas[posNeurona][i],2);
		}
		return sqrt(dis);
	}
	//~kohone();
	
};