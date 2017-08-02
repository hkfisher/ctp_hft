/******************************************************************************
Copyright (c) 2016. All Rights Reserved.

FileName: Trade.h
Version: 1.0
Date: 2017.4.25

History:
shengkaishan     2017.4.25   1.0     Create
******************************************************************************/


#ifndef TRADE_H
#define TRADE_H

#include "ThostFtdcTraderApi.h"
#include "SimpleEvent.h"
#include <map>
#include<QObject>
#include<QString>
#include <memory>
#include <thread>
#include <atomic>
using namespace std;
namespace future
{
    class Trade : public QObject, public CThostFtdcTraderSpi
    {
        Q_OBJECT
    public:
        Trade(void);
        ~Trade(void);

        void SetAPI(CThostFtdcTraderApi *pAPI);
        void Run();

        //void qry_postion();
        //void qry_order();
        void order_open(string& account, string& contract, double price);
        void order_withdraw();
        void order_close(string& account, string& contract);

    signals:
        void signals_write_log(QString str);
        void signals_state_changed(char state, char effect, QString deal_price);
        void signals_close_position(QString commodity_no, QString contract_no);
        void signals_withdraw_order(QString order_no);

    private:
        void order_state_handle(const CThostFtdcOrderField *info);
        void thread_reconnect();

        bool is_error_rsp(CThostFtdcRspInfoField *pRspInfo); // 是否收到错误信息
    public:
        ///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
        void OnFrontConnected();

        ///登录请求响应
        void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

        ///错误应答
        void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

        ///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
        void OnFrontDisconnected(int nReason);

        ///心跳超时警告。当长时间未收到报文时，该方法被调用。
        void OnHeartBeatWarning(int nTimeLapse);

        ///登出请求响应
        void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

        ///投资者结算结果确认响应
        void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

        ///请求查询合约响应
        void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

        ///请求查询投资者持仓响应
        void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

        ///报单录入请求响应
        void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

        ///报单操作请求响应
        void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

        ///报单通知
        void OnRtnOrder(CThostFtdcOrderField *pOrder);

        ///成交通知
        void OnRtnTrade(CThostFtdcTradeField *pTrade);

        ///报单录入错误回报
        virtual void OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo);

        ///报单操作错误回报
        virtual void OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo);

    private:
        CThostFtdcTraderApi *m_pAPI;
        int    m_requestid;
        string m_brokerid;
        string m_investorid;
        SimpleEvent m_Event;
        bool        m_bfront_status;
        bool        m_blogin_status;
        bool        m_bconfirm_status;
        bool        m_bposition;
        bool        m_border;
        bool        m_connect_state;
        map<string, char> m_map_order;
    public:
        atomic<bool> m_running;
        std::shared_ptr<std::thread> m_chk_thread;
    };
}
#endif // TRADE_H
