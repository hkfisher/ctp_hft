/********************************************************************************
** Form generated from reading UI file 'future_platform.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FUTURE_PLATFORM_H
#define UI_FUTURE_PLATFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_future_platformClass
{
public:

    void setupUi(QDialog *future_platformClass)
    {
        if (future_platformClass->objectName().isEmpty())
            future_platformClass->setObjectName(QStringLiteral("future_platformClass"));
        future_platformClass->resize(600, 400);

        retranslateUi(future_platformClass);

        QMetaObject::connectSlotsByName(future_platformClass);
    } // setupUi

    void retranslateUi(QDialog *future_platformClass)
    {
        future_platformClass->setWindowTitle(QApplication::translate("future_platformClass", "future_platform", 0));
    } // retranslateUi

};

namespace Ui {
    class future_platformClass: public Ui_future_platformClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FUTURE_PLATFORM_H
