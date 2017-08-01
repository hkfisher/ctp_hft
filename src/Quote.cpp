/******************************************************************************
Copyright (c) 2016. All Rights Reserved.

FileName: Quote.cpp
Version: 1.0
Date: 2017.4.25

History:
shengkaishan     2017.4.25   1.0     Create
******************************************************************************/


#include "Quote.h"
#include "QuoteConfig.h"
//#include <Windows.h>
#include <iostream>
#include <string.h>
#include "common.h"
#include "applog.h"
#include "md_server.h"
#include "future_platform.h"

using namespace std;
namespace future
{

    Quote::Quote(void) :
        m_pAPI(NULL),
        m_requestid(0),
        m_bfront_status(false),
        m_blogin_status(false),
        m_connect_state(false),
        m_chk_thread(nullptr),
        m_running(true)
    {
        m_map_contract.clear();
    }


    Quote::~Quote(void)
    {
        m_map_contract.clear();
    }


    void Quote::SetAPI(CThostFtdcMdApi *pAPI)
    {
        m_pAPI = pAPI;
    }


    int Quote::Run()
    {
        if (NULL == m_pAPI) {
            cerr << "error: m_pAPI is NULL." << endl;
            return -1;
        }

        int ret = TAPIERROR_SUCCEED;
        //连接服务器IP、端口
        QString log_str = "正在连接行情服务";
        APP_LOG(applog::LOG_INFO) << log_str.toStdString();
        emit signals_write_log(log_str);
        string key = "md_info/ip";
        QString ip = common::get_config_value(key).toString();
        key = "md_info/port";
        int port = common::get_config_value(key).toInt();
        string addr;
        addr.append("tcp://").append(ip.toStdString()).append(":").append(std::to_string(port));
        m_pAPI->RegisterSpi(this);                                          // 注册事件类
        m_pAPI->RegisterFront(const_cast<char*>(addr.c_str()));             // 设置行情前置地址
        m_pAPI->Init();                                                     // 连接运行
        //等待m_bfront_status
        m_Event.WaitEvent();
        if (!m_bfront_status) {
            return ret;
        }

        //登录服务器
        log_str = "正在登录行情服务";
        APP_LOG(applog::LOG_INFO) << log_str.toStdString();
        emit signals_write_log(log_str);
        string key = "md_info/brokerid";
        QString md_brokerid = common::get_config_value(key).toString();
        key = "md_info/userid";
        QString md_userid = common::get_config_value(key).toString();
        key = "md_info/passwd";
        QString md_passwd = common::getXorEncryptDecrypt(
            common::get_config_value(key).toString());
        CThostFtdcReqUserLoginField loginReq;
        memset(&loginReq, 0, sizeof(loginReq));
        strcpy(loginReq.BrokerID, md_brokerid.toStdString.c_str());
        strcpy(loginReq.UserID, md_userid.toStdString().c_str());
        strcpy(loginReq.Password, md_passwd.toStdString().c_str());
        ret = m_pAPI->ReqUserLogin(&loginReq, m_requestid++);
        if (TAPIERROR_SUCCEED != ret) {
            cout << "ReqUserLogin Error:" << ret << endl;
            return ret;
        }

        //等待m_blogin_status
        m_Event.WaitEvent();
        if (!m_blogin_status) {
            return ret;
        }

        log_str = "行情服务登录完成";
        APP_LOG(applog::LOG_INFO) << log_str.toStdString();
        emit signals_write_log(log_str);
        emit signals_quote_reconnect();

        m_connect_state = true;
        return ret;
    }

    void Quote::req_sub_market_data(string& contract)
    {
        QString log_str = QObject::tr("%1%2%3").arg("订阅").arg(contract.c_str()).arg("行情");
        APP_LOG(applog::LOG_INFO) << log_str.toStdString();
        emit signals_write_log(log_str);

        int ret = TAPIERROR_SUCCEED;
        //订阅行情
        char ppInstrumentID[1][10];
        strcpy(ppInstrumentID[0], contract.c_str());
        ret = m_pAPI->SubscribeMarketData((char**)ppInstrumentID, 1);
        if (TAPIERROR_SUCCEED != ret) {
            cout << "SubscribeMarketData Error:" << ret << endl;
            return;
        }
    }

    void Quote::req_unsub_market_data(string& contract)
    {
        QString log_str = QObject::tr("%1%2%3").arg("取消订阅").arg(contract.c_str()).arg("行情");
        APP_LOG(applog::LOG_INFO) << log_str.toStdString();
        emit signals_write_log(log_str);
        int ret = TAPIERROR_SUCCEED;
        //订阅行情
        char ppInstrumentID[1][10];
        strcpy(ppInstrumentID[0], contract.c_str());
        ret = m_pAPI->UnSubscribeMarketData((char**)ppInstrumentID, 1);
        if (TAPIERROR_SUCCEED != ret) {
            cout << "UnSubscribeMarketData Error:" << ret << endl;
            return;
        }
    }

    void Quote::thread_reconnect()
    {
        while (m_running) {
            if (!m_connect_state) {
                Run();
            }
            Sleep(3000);
        }
    }

    // ---- ctp_api回调函数 ---- //
    // 连接成功应答
    void Quote::OnFrontConnected()
    {
        QString log_str = "md API连接成功";
        APP_LOG(applog::LOG_INFO) << log_str.toStdString();
        emit signals_write_log(log_str);
        m_bfront_status = true;
        m_Event.SignalEvent();
    }

    // 断开连接通知
    void Quote::OnFrontDisconnected(int nReason)
    {
        if (!m_running) return;
        QString log_str = QObject::tr("%1%2").arg("API断开,断开原因:").
            arg(nReason);
        APP_LOG(applog::LOG_INFO) << log_str.toStdString();
        emit signals_write_log(log_str);

        m_connect_state = false;
        if (m_chk_thread != nullptr) return;
        m_chk_thread = std::make_shared<std::thread>(
            std::bind(&Quote::thread_reconnect, this));
    }

    // 心跳超时警告
    void Quote::OnHeartBeatWarning(int nTimeLapse)
    {
        std::cerr << "=====网络心跳超时=====" << std::endl;
        std::cerr << "距上次连接时间： " << nTimeLapse << std::endl;
    }

    // 登录应答
    void Quote::OnRspUserLogin(
        CThostFtdcRspUserLoginField *pRspUserLogin,
        CThostFtdcRspInfoField *pRspInfo,
        int nRequestID,
        bool bIsLast)
    {
        bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
        if (!bResult) {
            QString log_str = "md API登录成功";
            APP_LOG(applog::LOG_INFO) << log_str.toStdString();
            emit signals_write_log(log_str);
            m_blogin_status = true;
        }
        else {
            QString log_str = QObject::tr("%1%2").arg("登录失败，错误码:").
                arg(pRspInfo->ErrorID);
            APP_LOG(applog::LOG_INFO) << log_str.toStdString();
            emit signals_write_log(log_str);
            
        }
        m_Event.SignalEvent();
    }

    // 登出应答
    void Quote::OnRspUserLogout(
        CThostFtdcUserLogoutField *pUserLogout,
        CThostFtdcRspInfoField *pRspInfo,
        int nRequestID,
        bool bIsLast)
    {
        std::cout << __FUNCTION__ << std::endl;
        bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
        if (!bResult) {
            std::cout << "账户登出成功" << std::endl;
            std::cout << "经纪商： " << pUserLogout->BrokerID << std::endl;
            std::cout << "帐户名： " << pUserLogout->UserID << std::endl;
        }
        else {
            std::cerr << "返回错误ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
        }
    }

    // 错误通知
    void Quote::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
    {
        std::cout << __FUNCTION__ << std::endl;
        bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
        if (bResult)
            std::cerr << "返回错误ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
    }


    // 订阅行情应答
    void Quote::OnRspSubMarketData(
        CThostFtdcSpecificInstrumentField *pSpecificInstrument,
        CThostFtdcRspInfoField *pRspInfo,
        int nRequestID,
        bool bIsLast)
    {
        bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
        if (!bResult) {
            QString log_str = "行情订阅成功";
            APP_LOG(applog::LOG_INFO) << log_str.toStdString();
            emit signals_write_log(log_str);
        }
        else {
            QString log_str = QObject::tr("%1%2").arg("行情订阅失败，错误码：").
                arg(pRspInfo->ErrorID);
            APP_LOG(applog::LOG_INFO) << log_str.toStdString();
            emit signals_write_log(log_str);

        }
    }

    // 取消订阅行情应答
    void Quote::OnRspUnSubMarketData(
        CThostFtdcSpecificInstrumentField *pSpecificInstrument,
        CThostFtdcRspInfoField *pRspInfo,
        int nRequestID,
        bool bIsLast)
    {
        std::cout << __FUNCTION__ << std::endl;
        bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
        if (!bResult) {
            std::cout << "=====取消订阅行情成功=====" << std::endl;
            std::cout << "合约代码： " << pSpecificInstrument->InstrumentID << std::endl;
        }
        else
            std::cerr << "返回错误ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
    }

    // 行情详情通知
    void Quote::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
    {
        if (NULL != pDepthMarketData) {
            emit signals_quote_changed(QString::number(pDepthMarketData->LastPrice, 10, 3));
        }
    }
}