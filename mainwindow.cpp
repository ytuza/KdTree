#include "mainwindow.h"
#include "point.h"
#include <QtMath>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
{
    setSurfaceType(QWindow::OpenGLSurface);


    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    format.setVersion(2,1);

    setFormat(format);

    context = new QOpenGLContext;
    context->setFormat(format);
    context->create();

    context->makeCurrent(this);
    openGLFunctions = context->functions();

    Point min(-1.0,-1.0,-1.0);
    Point max(1.0,1.0,1.0);

    kdTree = nullptr;
    kdTree = new KdTree(min,max,0);

    cCircle.val[0] = 0.0;
    cCircle.val[1] = 0.0;
    angle= 0.2;

    zoomScale = 1.0;

    //pressLB = false;
}

void MainWindow::initializeGL()
{
    resizeGL(this->width(),this->height());

}


void MainWindow::resizeGL(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0,0,w,h);

   qreal aspectratio = qreal(w)/qreal(h);
   glOrtho(-1* aspectratio,1*aspectratio,-1,1,1,-1);


}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    resizeGL(this->width(),this->height());
    this->update();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    paintGL();

}

void MainWindow::mouseMoveEvent(QMouseEvent *ev)
{

      cCircle.val[0] = float(ev->x())/float(this->width())-0.5;
      cCircle.val[1] = float(ev->y())/float(this->height())-0.5;
      cCircle.val[0] *= 2;
      cCircle.val[1] *= -2;

      update();
}

void MainWindow::printKT(KdTree **quad)
{
    if(!((*quad)->leaf)){

        if((*quad)->ndim==0){

            verR.push_back(Point((*quad)->midP->val[0],(*quad)->dimMax.val[1],(*quad)->dimMax.val[2]));
            verR.push_back(Point((*quad)->midP->val[0],(*quad)->dimMax.val[1],(*quad)->dimMin.val[2]));

            verR.push_back(Point((*quad)->midP->val[0],(*quad)->dimMin.val[1],(*quad)->dimMin.val[2]));
            verR.push_back(Point((*quad)->midP->val[0],(*quad)->dimMin.val[1],(*quad)->dimMax.val[2]));

        }else if((*quad)->ndim == 1){

            verR.push_back(Point((*quad)->dimMax.val[0],(*quad)->midP->val[1],(*quad)->dimMax.val[2]));

            verR.push_back(Point((*quad)->dimMin.val[0],(*quad)->midP->val[1],(*quad)->dimMax.val[2]));

            verR.push_back(Point((*quad)->dimMin.val[0],(*quad)->midP->val[1],(*quad)->dimMin.val[2]));

            verR.push_back(Point((*quad)->dimMax.val[0],(*quad)->midP->val[1],(*quad)->dimMin.val[2]));
        }else{

            verR.push_back(Point((*quad)->dimMax.val[0],(*quad)->dimMax.val[1],(*quad)->midP->val[2]));
            verR.push_back(Point((*quad)->dimMax.val[0],(*quad)->dimMin.val[1],(*quad)->midP->val[2]));

            verR.push_back(Point((*quad)->dimMin.val[0],(*quad)->dimMin.val[1],(*quad)->midP->val[2]));
            verR.push_back(Point((*quad)->dimMin.val[0],(*quad)->dimMax.val[1],(*quad)->midP->val[2]));
        }

        printKT(&((*quad)->kdA));
        printKT(&((*quad)->kdB));
    }

}

void MainWindow::mousePressEvent(QMouseEvent *ev)
{

    float x_p = float(ev->x())/float(this->width())-0.5;
    float y_p = float(ev->y())/float(this->height())-0.5;




    pointsW.push_back(Point(x_p*2,y_p*-2));

    //qDebug() << "a" << endl;
    kdTree->insert(new Point(x_p*2,y_p*-2));
    //qDebug() << "b" << endl;


    verR.clear();

    //qDebug() << "a" << endl;
    printKT(&kdTree);
    //qDebug() << "b" << endl;
    update();

}

void MainWindow::paintGL()
{

    glClearColor(255, 200, 200, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();

    glScaled(zoomScale, zoomScale, zoomScale);
    //qDebug() << zoomScale ;
    glPointSize(3);


    gluPerspective(45.0,1.0,1.0,100.0);
    glTranslatef(0,0,-6);


    glRotatef(cCircle.val[0]*100,0,1,0);
    glRotatef(cCircle.val[1]*100,1,0,0);


    glBegin(GL_POINTS);
    glColor3d(0,0,255);
    for(int i=0;i<int(pointsW.size());i++){
        //qDebug() <<pointsW[i].first<< "paint " << pointsW[i].second<< "size" <<pointsW.size() ;
        glVertex3d(pointsW[i].val[0],pointsW[i].val[1],pointsW[i].val[2]);
    }
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(255,0,0);
          glVertex3f( 1.0f, 1.0f, -1.0f);
          glVertex3f(-1.0f, 1.0f, -1.0f);
          glVertex3f(-1.0f, 1.0f,  1.0f);
          glVertex3f( 1.0f, 1.0f,  1.0f);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3d(255,0,0);
          glVertex3f( 1.0f, -1.0f, -1.0f);
          glVertex3f(-1.0f, -1.0f, -1.0f);
          glVertex3f(-1.0f, -1.0f,  1.0f);
          glVertex3f( 1.0f, -1.0f,  1.0f);

    glEnd();

    glBegin(GL_LINES);
    glColor3d(255,0,0);

    glVertex3d(-1.0,-1.0,-1.0);
    glVertex3d(-1.0,1.0,-1.0);

    glVertex3d(-1.0,-1.0,1.0);
    glVertex3d(-1.0,1.0,1.0);

    glVertex3d(1.0,-1.0,-1.0);
    glVertex3d(1.0,1.0,-1.0);

    glVertex3d(1.0,-1.0,1.0);
    glVertex3d(1.0,1.0,1.0);

    glEnd();

   // qDebug()<< "1" << (int(verR.size())-4)<<endl;
    for(int i=0;i<=(int(verR.size())-4);i=i+4){
        //qDebug()<< "comenzo " << i << endl;
        glBegin(GL_LINE_LOOP);
        glColor3d(255,0,0);
              glVertex3f( verR[i].val[0], verR[i].val[1], verR[i].val[2]);
              glVertex3f( verR[i+1].val[0], verR[i+1].val[1], verR[i+1].val[2]);
              glVertex3f( verR[i+2].val[0], verR[i+2].val[1], verR[i+2].val[2]);
              glVertex3f( verR[i+3].val[0], verR[i+3].val[1], verR[i+3].val[2]);

        glEnd();

    }
    //qDebug()<< "fin 2" << endl;


    glFlush();


}

void MainWindow::wheelEvent(QWheelEvent *ev){

    QPoint numDegrees = ev->angleDelta();
    if (numDegrees.y() < 0)  zoomScale = zoomScale/1.1;
    if (numDegrees.y() > 0)  zoomScale = zoomScale*1.1;
    update(); // call paintGL()



}

MainWindow::~MainWindow()
{

}
