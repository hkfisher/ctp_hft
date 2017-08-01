/******************************************************************************
Copyright (c) 2016. All Rights Reserved.

FileName: Quote.h
Version: 1.0
Date: 2017.4.25

History:
shengkaishan     2017.4.25   1.0     Create
******************************************************************************/


#ifndef QUOTE_H
#define QUOTE_H

#include "ThostFtdcMdApi.h"
#include "SimpleEvent.h"
#include <map>
#include <QObject>
#include <QString>
#include <thread>
#include <memory>
#include <atomic>
using namespace std;
namespace future
{
    class Quote : public QObject, public CThostFtdcMdSpi
    {
        Q_OBJECT
    public:
        Quote(void);
        ~Quote(void);

        void SetAPI(CThostFtdcMdApi* pAPI);
        int Run();

        void req_sub_market_data(string& contract);
        void req_unsub_market_data(string& contract);

    signals:
        void signals_write_log(QString str);
        void signals_quote_changed(QString last_price);
        void signals_quote_reconnect();

    private:
        void thread_reconnect();

    public:
        ///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
        void OnFrontConnected();

        ///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
        ///@param nReason 错误原因
        ///        0x1001 网络读失败
        ///        0x1002 网络写失败
        ///        0x2001 接收心跳超时
        ///        0x2002 发送心跳失败
        ///        0x2003 收到错误报文
        void OnFrontDisconnected(int nReason);

        ///心跳超时警告。当长时间未收到报文时，该方法被调用。
        ///@param nTimeLapse 距离上次接收报文的时间
        void OnHeartBeatWarning(int nTimeLapse);

        ///登录请求响应
        void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

        ///登出请求响应
        void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

        ///错误应答
        void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

        ///订阅行情应答
        void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

        ///取消订阅行情应答
        void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

        ///深度行情通知
        void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

    private:
        CThostFtdcMdApi* m_pAPI;
        int    request_id;
        SimpleEvent m_Event;
        bool        m_bfront_status;
        bool        m_bIsAPIReady;

        map<string, string> m_map_contract;
        bool        m_connect_state;
    public:
        atomic<bool> m_running;
        std::shared_ptr<std::thread> m_chk_thread;
    };
}
#endif // QUOTE_H
