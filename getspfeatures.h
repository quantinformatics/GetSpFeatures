#ifndef GETSPFEATURES_H
#define GETSPFEATURES_H

#include <QObject>
#include<ipp.h>
#include<ipps.h>
#include<ippvm.h>
#include<ippcore.h>

class GetSpFeatures:public QObject{
    Q_OBJECT
public:
    explicit GetSpFeatures(QObject *parent = nullptr);
    Ipp64f tapWin[960];
    Ipp64f tsig[960];
    Ipp64f tsigK[960];
    Ipp64fc etsig[960];
    IppStatus status;

    IppsHilbertSpec* pSpec;
    Ipp8u* pBuffer;

    void getFeatures();

signals:

};

#endif // GETSPFEATURES_H
