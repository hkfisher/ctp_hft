#ifndef __FUTURE_PLATFORM_H__
#define __FUTURE_PLATFORM_H__

#ifdef WIN32
#ifndef _DEBUG
#define WIN32_LEAN_AND_MEAN
#endif
#endif
#include <QtWidgets/QDialog>
#include <QtWidgets/QtWidgets>
#include "set_dialog.h"

//#include "ui_future_platform.h"
namespace future
{
    class future_platform : public QDialog
    {
        Q_OBJECT

    public:
        future_platform(QWidget *parent = 0);
        ~future_platform();

    private slots:
        void slot_close();
        void slot_open_q_dialog();
        void slot_open_t_dialog();
        void slot_connnet_dialog();
        void slot_order_open();
        void slot_order_close();

        void slot_write_log(QString str);
        void slot_quote_changed(QString last_price);
        void slot_state_changed(char state, char effect, QString deal_price);
        void slot_close_position(QString commodity_no, QString contract_no);
        void slot_withdraw_order(QString order_no);
        void slot_quote_reconnect();

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
        void start_server();

    public:
        //Ui::future_platformClass ui;
        QWidget* top_wgt_;
        QLabel* title_label_;
        QToolButton* close_btn_;

        QWidget* body_wgt_;
        //1 line
        QLabel* q_user_label_;
        QLineEdit* q_user_line_edit_;
        QPushButton* q_set_btn_;

        //2 line
        QLabel* contract_label_;
        QLineEdit* contract_line_edit_;
        QPushButton* operator_btn_;

        //3 line
        QLabel* quote_label_;
        QLineEdit* quote_line_edit_;
        QLabel* quote_diff_label_;

        //4 line
        QLabel* t_user_label_;
        QLineEdit* t_user_line_edit_;
        QPushButton* t_set_btn_;

        //5 line
        QLabel* dot_label_;
        QLineEdit* dot_line_edit_;
        QPushButton* order_open_btn_;

        //6 line
        QLabel* deal_price_label_;
        QLineEdit* deal_price_line_edit_;
        QPushButton* order_close_btn_;

        QWidget* bottom_wgt_;
        QLabel* log_label_;
        QTextBrowser* log_text_edit_;

        //set_dialog
        set_dialog q_set_dialog_;
        set_dialog t_set_dialog_;

        QPoint windowPos;
        QPoint mousePos;
        QPoint dPos;
    private:
        void init();

    };
}
#endif // FUTURE_PLATFORM_H
