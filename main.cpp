#include "mainwindow.h"
#include <QApplication>
#include <QString>
#include <QTextStream>
#include "point.h"
#include <chrono>

void Read(QString Filename,QVector<float> &vector){
    QFile mFile(Filename);

    if(!mFile.open(QFile::ReadOnly | QFile:: Text)){
        qDebug() << "no pude";
        return;
    }

    QTextStream in(&mFile);
    QString mText = in.readAll();
    mText.remove('v');


    QStringList strList = mText.split(QRegExp("[ ]"),QString::SkipEmptyParts);
    float aum=0.1;

    for(int i=0; i<strList.length(); i++)
    {
        vector.push_back(strList[i].toFloat()*aum);
    }

    mFile.close();

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;


    QVector<float> dat;

    QString mFilename = "C:/Users/Acer/Documents/puntos3d/airboat.txt";


    //1 conejo coord2
    //0.4
    Read(mFilename,dat);
    qDebug() << dat.size()/3;

    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i<dat.size()-3; i=i+3)
    {
        w.kdTree->insert(new Point(dat[i],dat[i+1],dat[i+2]));
        w.pointsW.push_back(Point(dat[i],dat[i+1],dat[i+2]));
    }


    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end-start;

    qDebug() << diff.count() << " s\n";


    w.paintGL();



    w.setTitle("OpenGL QT");
    w.resize(680,680);
    w.show();

    return a.exec();
}
