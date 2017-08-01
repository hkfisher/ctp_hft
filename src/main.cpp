/******************************************************************************
Copyright (c) 2016. All Rights Reserved.

FileName: main.cpp
Version: 1.0
Date: 2017.4.25

History:
shengkaishan     2017.4.25   1.0     Create
******************************************************************************/

#include "future_platform.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    future::future_platform w;
    w.start_server();
    w.show();
    return a.exec();
}
