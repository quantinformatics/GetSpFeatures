#include "getspfeatures.h"
#include<QFile>
#include<QTextStream>
#include<QDebug>
#include<cmath>


GetSpFeatures::GetSpFeatures(QObject *parent):QObject{parent}{
    QString fileName("/home/michael/Desktop/Matlab/coeffs/kaiser_960_3.csv");
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    QString str;
    for(int i=0;i<960;i++){
        in >> str;
        tapWin[i] = str.toDouble();
    }
    file.close();

    QString fName("/home/michael/Desktop/Matlab/testData/outsig.csv");
    QFile file2(fName);
    file2.open(QIODevice::ReadOnly);
    QTextStream in2(&file2);
    for(int i=0;i<960;i++){
        in2 >> str;
        tsig[i] = str.toDouble();
    }
    file2.close();
    getFeatures();
}
void GetSpFeatures::getFeatures(){
    status = ippsMul_64f(tsig,tapWin,tsigK, 960);
    if(status != ippStsNoErr)
        qDebug() << ippGetStatusString(status);
    // tsigK and tsig Matlab agree
    // to 16 decimal places

    int sizeSpec, sizeBuf;

    status = ippsHilbertGetSize_64f64fc(960, ippAlgHintNone, &sizeSpec, &sizeBuf);
    if(status != ippStsNoErr)
        qDebug() << ippGetStatusString(status);
    pSpec = (IppsHilbertSpec*)ippMalloc(sizeSpec);
    pBuffer = (Ipp8u*)ippMalloc(sizeBuf);

    status = ippsHilbertInit_64f64fc(960,ippAlgHintNone,pSpec,pBuffer);
    if(status != ippStsNoErr)
        qDebug() << ippGetStatusString(status);
    status = ippsHilbert_64f64fc(tsigK,etsig, pSpec, pBuffer);
    if(status != ippStsNoErr)
        qDebug() << ippGetStatusString(status);

    //etsig and etsig Matlab agree here
    // to 16 decimal places

    //ippsMagnitude_64fc((Ipp32fc*)y, x, 5);

     ippFree(pSpec);
     ippFree(pBuffer);
     Ipp64fc sp[2048];

     int FFTOrder=11;
     IppsFFTSpec_C_64fc *pSpec=0;
     Ipp8u *pMemSpec=0;
     Ipp8u *pMemInit=0;
     Ipp8u *pMemBuffer=0;

     int sizeInit = 0;
     int sizeBuffer = 0;
     int flag = IPP_FFT_NODIV_BY_ANY;
     /// get sizes for required buffers
     status = ippsFFTGetSize_C_64fc(FFTOrder, flag, ippAlgHintNone, &sizeSpec, &sizeInit, &sizeBuffer);
     if(status!=ippStsNoErr)
         qDebug() << ippGetStatusString(status);

     /// allocate memory for required buffers
     pMemSpec = (Ipp8u*) ippMalloc(sizeSpec);
     pMemInit = (Ipp8u*) ippMalloc(sizeInit);
     pMemBuffer = (Ipp8u*) ippMalloc(sizeBuffer);

     /// initialize FFT specification structure
     status = ippsFFTInit_C_64fc(&pSpec, FFTOrder, flag, ippAlgHintNone, pMemSpec, pMemInit);
     if(status!=ippStsNoErr)
         qDebug() << ippGetStatusString(status);
     /// free initialization buffer
     if (sizeInit > 0){
         ippFree(pMemInit);
     }

     /// perform forward FFT
     status = ippsFFTFwd_CToC_64fc(etsig, sp, pSpec, pMemBuffer);
     if(status!=ippStsNoErr)
         qDebug() << ippGetStatusString(status);
     // sp and sp Matlab agree at this point
     // to 16 decimal places
     /// free buffers
     if (sizeBuffer > 0){
             ippFree(pMemBuffer);
     }
     ippFree(pMemSpec);

     double fq[1024];
     for(int i=0;i<1024;i++)
         fq[i] = (double)i/2048.0;
     // fq and fq Matlab agree here
     // to 15 decimal places

     double lfreqws = 0.55;
     double ufreqws = 12.0;
     double fs = 120;
     double flim[2]={2.0*lfreqws/fs, 2.0*ufreqws/fs};
     Ipp64fc spTmp[1024]={{0,0}};

     int fout =0;
     for(int i=0;i<1024;i++){
         if(fq[i]>=flim[0]/2.0 && fq[i] <=flim[1]/2.0){
            spTmp[i]=sp[i];
         }
     }
     Ipp64f spTempMag[1024];
     status = ippsMagnitude_64fc(spTmp,spTempMag,1024);
     if(status!=ippStsNoErr)
         qDebug() << ippGetStatusString(status);
     // sp = abs(sp(fout)).^2;
     Ipp64f norm;
     status=ippsNorm_L2_64fc64f(spTmp,1024,&norm);
     norm = norm*norm;
     double engr=norm/2048;
     qDebug("%.16f",engr);
     // engr and engr Matlabe agree here
     // to 15 decimal places

}
