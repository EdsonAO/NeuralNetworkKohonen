#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>

#include "CImg.h"

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
    pair<float,float> min_max[5];
    for(int i=0;i<5;++i)
        min_max[i]=make_pair(500.0,0.0);
    vector<vector<float> > Num;
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
        (salida.mean()<min_max[0].first)?min_max[0].first=salida.mean():(salida.mean()>min_max[0].second)?min_max[0].second=salida.mean():min_max[0].second;
        (salida.median()<min_max[1].first)?min_max[1].first=salida.median():(salida.median()>min_max[1].second)?min_max[1].second=salida.median():min_max[1].second;
        (sqrt(salida.variance())<min_max[2].first)?min_max[2].first=sqrt(salida.variance()):(sqrt(salida.variance())>min_max[2].second)?min_max[2].second=sqrt(salida.variance()):min_max[2].second;
        (salida.min()<min_max[3].first)?min_max[3].first=salida.min():(salida.min()>min_max[3].second)?min_max[3].second=salida.min():min_max[3].second;
        (salida.max()<min_max[4].first)?min_max[4].first=salida.max():(salida.max()>min_max[4].second)?min_max[4].second=salida.max():min_max[4].second;
        float vec[5] = {salida.mean(),salida.median(),sqrt(salida.variance()),salida.min(),salida.max()};
        Num.push_back(vector<float>(vec,vec+sizeof(vec)/sizeof(float)));
    }
    cout<<"Termino de la Operacion"<<endl;
    leer_dato.close();
    for(register size_t i=0;i<Num.size();++i){
        for(register size_t j = 0;j<Num[i].size();++j)
        escribir_vector<<(Num[i][j]-min_max[j].first)/(min_max[j].second-min_max[j].first)<<'\t';
        escribir_vector<<endl;
    }
    escribir_vector.close();
    for(int i=0;i<5;++i)
        cout<<min_max[i].first<<" "<<min_max[i].second<<endl;
}

void MainWindow::on_btn_foto_clicked()
{

}
