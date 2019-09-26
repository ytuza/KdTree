#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QOpenGLWindow>
#include <QSurfaceFormat>
#include <QOpenGLFunctions>
#include <QtOpenGL>
#include <GL/glu.h>
#include <QMouseEvent>


#include "point.h"
#include "kdtree.h"

class MainWindow : public QOpenGLWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    KdTree *kdTree ;

    virtual void paintGL();


    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);

    virtual void initializeGL();
    virtual void resizeGL(int w, int h);

    void wheelEvent(QWheelEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);


    vector<Point> pointsW;
    vector<Point> verR;

    Point cCircle;
    float angle;
    bool pressLB;
    float zoomScale;

    void printKT(KdTree **quad);
private:

    QOpenGLContext *context;
    QOpenGLFunctions *openGLFunctions;
};

#endif // MAINWINDOW_H
