#ifndef KDTREE_H
#define KDTREE_H

#include "point.h"
#include <vector>
#include <QtDebug>
#include <algorithm>
using namespace std;

class KdTree{

public:
    KdTree(){

        dimMin = Point(-1,-1,-1);
        dimMax = Point(1,1,1);
        leaf = true;
        ndim = 0;
        midP = nullptr;
    }
    KdTree(Point min, Point max, int d){
        dimMax = max;
        dimMin = min;
        leaf = true;
        ndim = d;
        midP = nullptr;
    }

    ~KdTree(){
        for(int i = 0;i<data.size();i++)
            delete data[i];
        if(!leaf){
            delete kdA;
            delete kdB;
            delete midP;
        }
    }

    int nextD(void){
        if(ndim == 2){
            return 0;
        }
        return ndim+1;
    }

    KdTree *kdA;
    KdTree *kdB;

    bool leaf;
    Point dimMax;
    Point dimMin;
    vector<Point*> data;
    int ndim;
    Point* midP;

    bool find(Point *pt, KdTree **pQT ){
        if(leaf){
            *pQT = this;

            for(int i=0;i<data.size();i++){

                if(data[i]->val[0] == pt->val[0] && data[i]->val[1] == pt->val[1] && data[i]->val[2] == pt->val[2]){
                    return true;
                }
            }
            return false;
        }
        if(midP->val[0] == pt->val[0] && midP->val[1] == pt->val[1] && midP->val[2] == pt->val[2]){
            return true;
        }
        return ubic(pt)->find(pt,pQT);
    }

    KdTree* ubic(Point *p){

        if(p->val[ndim] <= midP->val[ndim]){
            return kdA;
        }else{
            return kdB;
        }
    }

    bool myfunction (Point* i,Point* j) {

        return (i->val[ndim]<j->val[ndim]);
    }

    void sortData (void) {
        if(leaf) return;
        sort(data.begin(),data.end(), [this] (Point* a, Point* b) {
            return myfunction(a, b); });
    }


    bool insert(Point *pt)
    {
        //qDebug() << "comenzo" ;

        KdTree *pQT = nullptr;
        //qDebug() << "1" << endl;
        if(find(pt,&pQT)){
            //qDebug() << "ya esta " ;
            return false;
        }

        //qDebug() << "2" << endl;
        if(pQT==nullptr) return false;

        if( pQT->stopCond() ){

            pQT->leaf = false;


            pQT->sortData();

            int mitad = pQT->data.size()/2;

            pQT->midP = pQT->data[mitad];

            if(pQT->ndim==0){
                pQT->kdA = new KdTree(pQT->dimMin,Point(pQT->midP->val[0],pQT->dimMax.val[1],pQT->dimMax.val[2]),pQT->nextD());
                pQT->kdB = new KdTree(Point(pQT->midP->val[0],pQT->dimMin.val[1],pQT->dimMin.val[2]),pQT->dimMax,pQT->nextD());
            }else if(pQT->ndim == 1){
                pQT->kdA = new KdTree(pQT->dimMin,Point(pQT->dimMax.val[0],pQT->midP->val[1],pQT->dimMax.val[2]),pQT->nextD());
                pQT->kdB = new KdTree(Point(pQT->dimMin.val[0],pQT->midP->val[1],pQT->dimMin.val[2]),pQT->dimMax,pQT->nextD());
            }else{
                pQT->kdA = new KdTree(pQT->dimMin,Point(pQT->dimMax.val[0],pQT->dimMax.val[1],pQT->midP->val[2]),pQT->nextD());
                pQT->kdB = new KdTree(Point(pQT->dimMin.val[0],pQT->dimMin.val[1],pQT->midP->val[2]),pQT->dimMax,pQT->nextD());

            }



            for(int i = 0;i<int(pQT->data.size());i++){
                //if(i!=mitad)
                    (pQT->ubic(pQT->data[i]))->insert(pQT->data[i]);
            }
            (pQT->ubic(pt))->insert(pt);



            (pQT->data).clear();

        }else{
            (pQT->data).push_back(pt);
        }


        return true;
    }


private:

    bool stopCond(){
        if(data.size() >=1159){
            return true;
        }
        return false;

    }

};
#endif // KDTREE_H
