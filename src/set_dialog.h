/******************************************************************************
Copyright (c) 2016. All Rights Reserved.

FileName: set_dialog.h
Version: 1.0
Date: 2017.4.25

History:
shengkaishan     2017.4.25   1.0     Create
******************************************************************************/

#ifndef __SET_DIALOG_H__
#define __SET_DIALOG_H__

#include <QtWidgets/QDialog>
#include <QtWidgets/QtWidgets>
//#include "ui_future_platform.h"
namespace future
{

    class set_dialog : public QDialog
    {
        Q_OBJECT

    public:
        set_dialog(int type, QWidget *parent = 0);
        ~set_dialog();

    private slots:
        void slot_close();
        void slot_save();

        void mousePressEvent(QMouseEvent *event)
        {
            this->windowPos = this->pos();           // 获得部件当前位置
            this->mousePos = event->globalPos();     // 获得鼠标位置
            this->dPos = mousePos - windowPos;       // 移动后部件所在的位置
        }

        void mouseMoveEvent(QMouseEvent *event)
        {
            this->move(event->globalPos() - this->dPos);
        }
    public:
        QWidget* top_wgt_;
        QLabel* title_label_;
        QToolButton* close_btn_;

        QWidget* body_wgt_;
        //1 line
        QLabel* server_label_;
        QLineEdit* server_line_edit_;

        //2 line
        QLabel* user_label_;
        QLineEdit* user_line_edit_;

        //3 line
        QLabel* passwd_label_;
        QLineEdit* passwd_line_edit_;

        QWidget* bottom_wgt_;
        QPushButton* save_btn_;
        QPushButton* cancel_btn_;

        QPoint windowPos;
        QPoint mousePos;
        QPoint dPos;

    private:
        void init();

    private:
        int m_dialog_type_; //0:md 1:trader
    };
}
#endif // FUTURE_PLATFORM_H
