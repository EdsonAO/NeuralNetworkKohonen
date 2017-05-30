#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>

#include "CImg.h"
#include <omp.h>

using namespace std;
using namespace cimg_library;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_click_clicked()
{

    ifstream leer_dato("ima2");
    ofstream escribir_vector("vectores.data");
    escribir_vector<<"media"<<'\t'<<"mediana"<<'\t'<<"desviacion"<<'\t'<<"min"<<'\t'<<"max"<<endl;
    string imagen;
    int num_haar = 3,divisor=8;
    while (!leer_dato.eof()) {
        leer_dato>>imagen;
        cout<<"Procesando imagen: "<<imagen<<endl;
        CImg<float> img(imagen.c_str());
        img.resize(400,400);
        CImg<float>res=img.get_haar(false,num_haar);
        int x=res.width()/divisor,y=res.height()/divisor;
        CImg<float> salida(x,y);
        for(register int i=0;i<x;++i)
            for(register int j=0;j<y;++j)
                salida(i,j)=res(i,j);
        escribir_vector<<salida.mean()<<" "<<salida.median()<<" "<<sqrt(salida.variance())<<" "<<salida.min()<<" "<<salida.max()<<endl;
    }
    cout<<"Termino de la Operacion"<<endl;
    leer_dato.close();
    escribir_vector.close();
}
