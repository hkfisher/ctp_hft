/******************************************************************************
Copyright (c) 2016. All Rights Reserved.

FileName: set_dialog.cpp
Version: 1.0
Date: 2017.4.25

History:
shengkaishan     2017.4.25   1.0     Create
******************************************************************************/

#include "set_dialog.h"
#include "common.h"
namespace future
{
    set_dialog::set_dialog(int type, QWidget *parent)
        : m_dialog_type_(type),
        QDialog(parent)
    {
        top_wgt_ = new QWidget(this);
        top_wgt_->setStyleSheet(
            "background-color:#000080;color:#FFFFFF");
        top_wgt_->setGeometry(QRect(0, 0, 300, 30));
       
        title_label_ = new QLabel("账户设置");
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        title_label_->setFont(font);

        close_btn_ = new QToolButton();
        close_btn_->setIcon(QIcon(
            close_picture.c_str()));
        close_btn_->setAutoRaise(true);
        connect(close_btn_, SIGNAL(clicked()),
            this, SLOT(slot_close()));

        QHBoxLayout* top_layout = new QHBoxLayout(top_wgt_);
        top_layout->setContentsMargins(0, 0, 0, 0);
        top_layout->addWidget(title_label_, 0, Qt::AlignCenter);
        top_layout->addWidget(close_btn_);

        body_wgt_ = new QWidget(this);
        body_wgt_->setStyleSheet(
            "background-color:#FFFFFF; color:#000000");
        body_wgt_->setGeometry(QRect(0, 30, 300, 160));
        
        server_label_ = new QLabel("服务器:");
        server_label_->setFixedHeight(30);
        server_line_edit_ = new QLineEdit();
        server_line_edit_->setFixedHeight(30);

        user_label_ = new QLabel("用  户:");
        user_label_->setFixedHeight(30);
        user_line_edit_ = new QLineEdit();
        user_line_edit_->setFixedHeight(30);

        passwd_label_ = new QLabel("密  码:");
        passwd_label_->setFixedHeight(30);
        passwd_line_edit_ = new QLineEdit();
        passwd_line_edit_->setFixedHeight(30);
        passwd_line_edit_->setEchoMode(QLineEdit::Password);

        QGridLayout* body_layout = new QGridLayout(body_wgt_);
        body_layout->setContentsMargins(30, 30, 30, 30);
        body_layout->addWidget(server_label_, 0, 0, 1, 1);
        body_layout->addWidget(server_line_edit_, 0, 1, 1, 3);

        body_layout->addWidget(user_label_, 1, 0, 1, 1);
        body_layout->addWidget(user_line_edit_, 1, 1, 1, 3);

        body_layout->addWidget(passwd_label_, 2, 0, 1, 1);
        body_layout->addWidget(passwd_line_edit_, 2, 1, 1, 3);

        bottom_wgt_ = new QWidget(this);
        bottom_wgt_->setStyleSheet(
            "background-color:#FFFFFF;color:#000000");
        bottom_wgt_->setGeometry(QRect(0, 190, 300, 60));

        save_btn_ = new QPushButton("保存");
        save_btn_->setFixedHeight(30);
        save_btn_->setStyleSheet("background-color: #CFC9C9;border: 1px solid black;");
        cancel_btn_ = new QPushButton("取消");
        cancel_btn_->setFixedHeight(30);
        cancel_btn_->setStyleSheet("background-color: #CFC9C9;border: 1px solid black;");
        connect(save_btn_, SIGNAL(clicked()),
            this, SLOT(slot_save()));
        connect(cancel_btn_, SIGNAL(clicked()),
            this, SLOT(slot_close()));

        QHBoxLayout* bottom_layout = new QHBoxLayout(bottom_wgt_);
        bottom_layout->setSpacing(60);
        bottom_layout->setContentsMargins(30, 0, 30, 30);
        bottom_layout->addWidget(save_btn_);
        bottom_layout->addWidget(cancel_btn_);

        //QWidget *whole_widget = new QWidget(this);
        //whole_widget->setContentsMargins(QMargins(0, 0, 0, 0));
        //whole_widget->setStyleSheet("background-color:white; border: 1px solid #CCCCCC;");
        //
        

        init();

        setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
        setFixedSize(300, 250);
    }

    set_dialog::~set_dialog()
    {

    }

    void set_dialog::slot_close()
    {
        this->close();
    }

    void set_dialog::slot_save()
    {
        string key_ip;
        string key_user;
        string key_pwd;
        if (m_dialog_type_ == MD_TYPE) {
            key_ip = "md_info/ip";
            key_user = "md_info/userid";
            key_pwd = "md_info/passwd";
        } else {
            key_ip = "trader_info/ip";
            key_user = "trader_info/userid";
            key_pwd = "trader_info/passwd";
        }

        string ip = server_line_edit_->text().toStdString();
        string user = user_line_edit_->text().toStdString();
        string pwd = common::getXorEncryptDecrypt(passwd_line_edit_->text()).
            toStdString();
        common::set_config_value(key_ip, ip);
        common::set_config_value(key_user, user);
        common::set_config_value(key_pwd, pwd);

        this->close();
    }

    void set_dialog::init()
    {
        string key_ip;
        string key_user;
        string key_pwd;
        if (m_dialog_type_ == MD_TYPE) {
            key_ip = "md_info/ip";
            key_user = "md_info/userid";
            key_pwd = "md_info/passwd";
        } 
        else {
            key_ip = "trader_info/ip";
            key_user = "trader_info/userid";
            key_pwd = "trader_info/passwd";
        }

        QString ip = common::get_config_value(key_ip).toString();
        QString userid = common::get_config_value(key_user).toString();
        QString passwd = common::get_config_value(key_pwd).toString();
        server_line_edit_->setText(ip);
        user_line_edit_->setText(userid);
        passwd_line_edit_->setText(passwd);
    }
}