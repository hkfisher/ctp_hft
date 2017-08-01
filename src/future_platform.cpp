/******************************************************************************
Copyright (c) 2016. All Rights Reserved.

FileName: future_platform.cpp
Version: 1.0
Date: 2017.4.25

History:
shengkaishan     2017.4.25   1.0     Create
******************************************************************************/

#include "future_platform.h"
#include "md_server.h"
#include "trader_server.h"
#include "common.h"

namespace future
{

    future_platform::future_platform(QWidget *parent)
        : QDialog(parent),
        q_set_dialog_(MD_TYPE),
        t_set_dialog_(TRADER_TYPE)
    {
        //ui.setupUi(this);
        top_wgt_ = new QWidget(this);
        top_wgt_->setStyleSheet(
            "background-color:#000080;color:#FFFFFF");
        top_wgt_->setGeometry(QRect(0, 0, 400, 30));

        title_label_ = new QLabel("交易工具");
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        title_label_->setFont(font);

        close_btn_ = new QToolButton();
        close_btn_->setIcon(QIcon(
            close_picture.c_str()));
        close_btn_->setAutoRaise(true);

        QHBoxLayout* top_layout = new QHBoxLayout(top_wgt_);
        top_layout->setContentsMargins(0, 0, 0, 0);
        top_layout->addWidget(title_label_, 0, Qt::AlignCenter);
        top_layout->addWidget(close_btn_);

        body_wgt_ = new QWidget(this);
        body_wgt_->setStyleSheet(
            "background-color:rgb(255, 255, 255);color:rgb(0, 0, 0)");
        body_wgt_->setGeometry(QRect(0, 30, 400, 270));

        q_user_label_ = new QLabel("行情账号:");
        q_user_label_->setFixedHeight(30);
        q_user_line_edit_ = new QLineEdit();
        q_user_line_edit_->setFixedHeight(30);
        q_user_line_edit_->setEnabled(false);
        q_user_line_edit_->setFocusPolicy(Qt::NoFocus);
        q_set_btn_ = new QPushButton("设置");
        q_set_btn_->setFixedHeight(30);
        q_set_btn_->setStyleSheet("background-color: #CFC9C9;border: 1px solid black;");

        contract_label_ = new QLabel("    合约:");
        contract_label_->setFixedHeight(30);
        contract_line_edit_ = new QLineEdit();
        contract_line_edit_->setFixedHeight(30);
        operator_btn_ = new QPushButton("连接");
        operator_btn_->setFixedHeight(30);
        operator_btn_->setStyleSheet("background-color: #CFC9C9;border: 1px solid black;");

        quote_label_ = new QLabel("当前价格:");
        quote_label_->setFixedHeight(30);
        quote_line_edit_ = new QLineEdit();
        quote_line_edit_->setFixedHeight(30);
        quote_line_edit_->setEnabled(false);
        quote_line_edit_->setFocusPolicy(Qt::NoFocus);
        quote_diff_label_ = new QLabel("0");
        quote_diff_label_->setFixedHeight(30);

        t_user_label_ = new QLabel("交易账号:");
        t_user_label_->setFixedHeight(30);
        t_user_line_edit_ = new QLineEdit();
        t_user_line_edit_->setFixedHeight(30);
        t_user_line_edit_->setEnabled(false);
        t_user_line_edit_->setFocusPolicy(Qt::NoFocus);
        t_set_btn_ = new QPushButton("设置");
        t_set_btn_->setFixedHeight(30);
        t_set_btn_->setStyleSheet("background-color: #CFC9C9;border: 1px solid black;");

        dot_label_ = new QLabel("挂单点数:");
        dot_label_->setFixedHeight(30);
        dot_line_edit_ = new QLineEdit();
        dot_line_edit_->setFixedHeight(30);
        order_open_btn_ = new QPushButton("挂单");
        order_open_btn_->setFixedHeight(30);
        order_open_btn_->setStyleSheet("background-color: #CFC9C9;border: 1px solid black;");

        deal_price_label_ = new QLabel("成交单价:");
        deal_price_label_->setFixedHeight(30);
        deal_price_line_edit_ = new QLineEdit();
        deal_price_line_edit_->setFixedHeight(30);
        deal_price_line_edit_->setEnabled(false);
        deal_price_line_edit_->setFocusPolicy(Qt::NoFocus);
        order_close_btn_ = new QPushButton("平仓");
        order_close_btn_->setFixedHeight(30);
        order_close_btn_->setStyleSheet("background-color: #CFC9C9;border: 1px solid black;");

        QGridLayout* body_layout = new QGridLayout(body_wgt_);
        body_layout->setContentsMargins(30, 30, 30, 30);
        body_layout->addWidget(q_user_label_, 0, 0, 1, 1);
        body_layout->addWidget(q_user_line_edit_, 0, 1, 1, 2);
        body_layout->addWidget(q_set_btn_, 0, 3, 1, 1);

        body_layout->addWidget(contract_label_, 1, 0, 1, 1);
        body_layout->addWidget(contract_line_edit_, 1, 1, 1, 2);
        body_layout->addWidget(operator_btn_, 1, 3, 1, 1);

        body_layout->addWidget(quote_label_, 2, 0, 1, 1);
        body_layout->addWidget(quote_line_edit_, 2, 1, 1, 2);
        body_layout->addWidget(quote_diff_label_, 2, 3, 1, 1);

        body_layout->addWidget(t_user_label_, 3, 0, 1, 1);
        body_layout->addWidget(t_user_line_edit_, 3, 1, 1, 2);
        body_layout->addWidget(t_set_btn_, 3, 3, 1, 1);

        body_layout->addWidget(dot_label_, 4, 0, 1, 1);
        body_layout->addWidget(dot_line_edit_, 4, 1, 1, 2);
        body_layout->addWidget(order_open_btn_, 4, 3, 1, 1);

        body_layout->addWidget(deal_price_label_, 5, 0, 1, 1);
        body_layout->addWidget(deal_price_line_edit_, 5, 1, 1, 2);
        body_layout->addWidget(order_close_btn_, 5, 3, 1, 1);

        bottom_wgt_ = new QWidget(this);
        bottom_wgt_->setStyleSheet(
            "background-color:rgb(255, 255, 255);color:rgb(0, 0, 0)");
        bottom_wgt_->setGeometry(QRect(0, 300, 400, 300));

        log_label_ = new QLabel("操作日志");
        log_text_edit_ = new QTextBrowser();
        //log_text_edit_->setEnabled(false);
        //log_text_edit_->setFocusPolicy(Qt::NoFocus);
        QVBoxLayout* bottom_layout = new QVBoxLayout(bottom_wgt_);
        bottom_layout->setContentsMargins(30, 0, 30, 30);
        bottom_layout->addWidget(log_label_);
        bottom_layout->addWidget(log_text_edit_);

        setWindowIcon(QIcon(log_picture.c_str()));
        setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
        setFixedSize(400, 600);
    }

    future_platform::~future_platform()
    {
        slot_close();
    }

    void future_platform::start_server()
    {
        md_server::create_instance();
        trader_server::create_instance();

        init();

        //md_server::get_instance()->set_future_platform(this);
        md_server::get_instance()->start_server();
        //trader_server::get_instance()->set_future_platform(this);
        trader_server::get_instance()->start_server();
    }

    void future_platform::slot_close()
    {
        md_server::destory_instance();
        trader_server::destory_instance();
        this->close();
    }

    void future_platform::slot_open_q_dialog()
    {
        q_set_dialog_.exec();
    }

    void future_platform::slot_open_t_dialog()
    {
        t_set_dialog_.exec();
    }

    void future_platform::slot_connnet_dialog()
    {
        QString contract = contract_line_edit_->text();
        if (contract.isEmpty()) return;
        if (operator_btn_->text() == "连接") {
            md_server::get_instance()->get_md_spi()->
                req_sub_market_data(contract.toStdString());
            string key = "md_info/contractid";
            common::set_config_value(key, contract.toStdString());
            operator_btn_->setText("停止");
            contract_line_edit_->setEnabled(false);
        } else {
            md_server::get_instance()->get_md_spi()->
                req_unsub_market_data(contract.toStdString());
            operator_btn_->setText("连接");
            contract_line_edit_->setEnabled(true);
            quote_line_edit_->setText(QString::number(0, 10, 0));
            quote_diff_label_->setText(QString::number(0, 10, 0));
        }
    }
    
    void future_platform::slot_order_open()
    {
        if (order_open_btn_->text() == "挂单") {
            QString qaccount = (t_user_line_edit_->text().split("/"))[1];
            string account = qaccount.toStdString();
            string contract = contract_line_edit_->text().toStdString();
            double price = dot_line_edit_->text().toDouble() * 0.01 +
                quote_line_edit_->text().toDouble();

            trader_server::get_instance()->get_trader_spi()->order_open(
                account, contract, price);
        } 
        else {
            trader_server::get_instance()->get_trader_spi()->order_withdraw();
        }
    }

    void future_platform::slot_order_close()
    {
        QString qaccount = (t_user_line_edit_->text().split("/"))[1];
        string account = qaccount.toStdString();
        string contract = contract_line_edit_->text().toStdString();
        trader_server::get_instance()->get_trader_spi()->order_close(
            account, contract);
    }

    void future_platform::slot_write_log(QString str)
    {
        QDateTime now = QDateTime::currentDateTime();
        QString time_str = now.toString("hh:mm:ss");
        QString log = QObject::tr("%1  %2").arg(time_str).arg(str);
        log_text_edit_->append(log);
    }

    void future_platform::slot_quote_changed(QString last_price)
    {
        quote_line_edit_->setText(last_price);

        QString log = QObject::tr("%1 %2").arg("当前价格").arg(last_price);
        slot_write_log(log);
    }

    void future_platform::slot_state_changed(char state, char effect, QString deal_price)
    {
        switch (state) {
        case TAPI_ORDER_STATE_QUEUED:
        {
            deal_price_line_edit_->setText(deal_price);
            order_open_btn_->setText("弃单");
            break;
        }
        case TAPI_ORDER_STATE_FINISHED:
        {
            order_open_btn_->setText("挂单");
            if (effect == TAPI_SIDE_SELL) {
                order_open_btn_->setEnabled(false);
                order_close_btn_->setHidden(false);
                deal_price_line_edit_->setText(deal_price);

                double last_price = quote_line_edit_->text().toDouble();
                double diff_price = last_price - deal_price.toDouble();
                quote_diff_label_->setText(
                    QString::number(diff_price, 10, 2));
            } 
            else if (effect == TAPI_SIDE_BUY) {
                order_open_btn_->setEnabled(true);
                order_close_btn_->setHidden(true);
            }
            break;
        }
        case TAPI_ORDER_STATE_CANCELED:
        {
            order_open_btn_->setText("挂单");
            break;
        }
        case TAPI_ORDER_STATE_LEFTDELETED:
        {
            order_open_btn_->setText("挂单");
            break;
        }
        default:
            break;
        }

    }

    void future_platform::slot_close_position(QString commodity_no, QString contract_no)
    {
        contract_line_edit_->setText(commodity_no+contract_no);
        order_open_btn_->setEnabled(false);
        order_close_btn_->setHidden(false);
    }

    void future_platform::slot_withdraw_order(QString order_no)
    {
        order_open_btn_->setText("弃单");
    }

    void future_platform::slot_quote_reconnect()
    {
        QString contract = contract_line_edit_->text();
        if (contract.isEmpty()) return;
        if (operator_btn_->text() == "停止") {
            md_server::get_instance()->get_md_spi()->
                req_sub_market_data(contract.toStdString());
        }
    }

    void future_platform::init()
    {
        string key = "md_info/ip";
        QString ip = common::get_config_value(key).toString();
        key = "md_info/userid";
        QString userid = common::get_config_value(key).toString();
        q_user_line_edit_->setText(ip + "/" + userid);

        key = "md_info/contractid";
        QString md_contractid = common::get_config_value(key).toString();
        contract_line_edit_->setText(md_contractid);

        key = "trader_info/ip";
        ip = common::get_config_value(key).toString();
        key = "trader_info/userid";
        userid = common::get_config_value(key).toString();
        t_user_line_edit_->setText(ip + "/" + userid);

        dot_line_edit_->setText(QString::number(0, 10, 0));

        order_close_btn_->setHidden(true);
        connect(close_btn_, SIGNAL(clicked()),
            this, SLOT(slot_close()));

        connect(q_set_btn_, SIGNAL(clicked()),
            this, SLOT(slot_open_q_dialog()));

        connect(operator_btn_, SIGNAL(clicked()),
            this, SLOT(slot_connnet_dialog()));

        connect(t_set_btn_, SIGNAL(clicked()),
            this, SLOT(slot_open_t_dialog()));

        connect(order_open_btn_, SIGNAL(clicked()),
            this, SLOT(slot_order_open()));

        connect(order_close_btn_, SIGNAL(clicked()),
            this, SLOT(slot_order_close()));

        connect(md_server::get_instance()->get_md_spi(),
            SIGNAL(signals_write_log(QString)),
            this, SLOT(slot_write_log(QString)));

        connect(trader_server::get_instance()->get_trader_spi(),
            SIGNAL(signals_write_log(QString)),
            this, SLOT(slot_write_log(QString)));

        connect(md_server::get_instance()->get_md_spi(), 
            SIGNAL(signals_quote_changed(QString)),
            this, SLOT(slot_quote_changed(QString)));

        connect(trader_server::get_instance()->get_trader_spi(),
            SIGNAL(signals_state_changed(char, char, QString)),
            this, SLOT(slot_state_changed(char, char, QString)));

        connect(trader_server::get_instance()->get_trader_spi(),
            SIGNAL(signals_close_position(QString, QString)),
            this, SLOT(slot_close_position(QString, QString)));

        connect(trader_server::get_instance()->get_trader_spi(),
            SIGNAL(signals_withdraw_order(QString)),
            this, SLOT(slot_withdraw_order(QString)));

        connect(md_server::get_instance()->get_md_spi(),
            SIGNAL(signals_quote_reconnect()),
            this, SLOT(slot_quote_reconnect()));
    }
}