#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include"getspfeatures.h"

class MainWidget:public QWidget{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
    GetSpFeatures *getSpFeatures;

};
#endif // MAINWIDGET_H
