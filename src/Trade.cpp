/******************************************************************************
Copyright (c) 2016. All Rights Reserved.

FileName: Trade.cpp
Version: 1.0
Date: 2017.4.25

History:
shengkaishan     2017.4.25   1.0     Create
******************************************************************************/


#include "Trade.h"
#include "TradeConfig.h"
#include <iostream>
#include <string.h>
#include "common.h"
#include "applog.h"
#include "trader_server.h"
#include "future_platform.h"

using namespace std;
namespace future
{
    Trade::Trade(void) :
        m_pAPI(NULL),
        m_requestid(0),
        m_brokerid(""),
        m_investorid(""),
        m_bfront_status(false),
        m_blogin_status(false),
        m_bconfirm_status(false),
        m_bposition(false),
        m_border(false),
        m_connect_state(false),
        m_chk_thread(nullptr),
        m_running(true)
    {
        m_map_order.clear();
    }

    Trade::~Trade(void)
    {
        m_map_order.clear();
    }

    void Trade::SetAPI(CThostFtdcTraderApi *pAPI)
    {
        m_pAPI = pAPI;
    }

    void Trade::Run()
    {
        if (NULL == m_pAPI) {
            cout << "Error: m_pAPI is NULL." << endl;
            return;
        }

        int ret = TAPIERROR_SUCCEED;
        //连接服务器IP、端口
        QString log_str = "正在连接交易服务";
        APP_LOG(applog::LOG_INFO) << log_str.toStdString();
        emit signals_write_log(log_str);
        string key = "trader_info/ip";
        QString ip = common::get_config_value(key).toString();
        key = "trader_info/port";
        int port = common::get_config_value(key).toInt();
        string addr;
        addr.append("tcp://").append(ip.toStdString()).append(":").append(std::to_string(port));
        m_pAPI->RegisterSpi(this);                        // 注册事件类
        m_pAPI->RegisterFront(const_cast<char*>(addr.c_str()));                // 设置交易前置地址
        m_pAPI->SubscribePublicTopic(THOST_TERT_RESTART);      // 订阅公共流
        m_pAPI->SubscribePrivateTopic(THOST_TERT_RESUME);     // 订阅私有流
        m_pAPI->Init();                                        // 连接运行
        //等待m_bfront_status
        m_Event.WaitEvent();
        if (!m_bfront_status) {
            return;
        }

        //登录服务器
        log_str = "正在登录交易服务";
        APP_LOG(applog::LOG_INFO) << log_str.toStdString();
        emit signals_write_log(log_str);
        key = "trader_info/brokerid";
        m_brokerid = common::get_config_value(key).toString().toStdString();
        key = "trader_info/userid";
        m_investorid = common::get_config_value(key).toString().toStdString();
        key = "trader_info/passwd";
        //QString passwd = common::getXorEncryptDecrypt(
        //    common::get_config_value(key).toString());
        QString passwd = common::get_config_value(key).toString();
        CThostFtdcReqUserLoginField loginReq;
        memset(&loginReq, 0, sizeof(loginReq));
        strcpy(loginReq.BrokerID, m_brokerid.c_str());
        strcpy(loginReq.UserID, m_investorid.c_str());
        strcpy(loginReq.Password, passwd.toStdString().c_str());
        ret = m_pAPI->ReqUserLogin(&loginReq, m_requestid++);
        if (TAPIERROR_SUCCEED != ret) {
            cout << "ReqUserLogin Error:" << ret << endl;
            return;
        }
        //等待m_blogin_status
        m_Event.WaitEvent();
        if (!m_blogin_status) {
            return;
        }

        //投资者结算结果确认
        log_str = "正在结算结果确认";
        APP_LOG(applog::LOG_INFO) << log_str.toStdString();
        emit signals_write_log(log_str);
        CThostFtdcSettlementInfoConfirmField settlementConfirmReq;
        memset(&settlementConfirmReq, 0, sizeof(settlementConfirmReq));
        strcpy(settlementConfirmReq.BrokerID, m_brokerid.c_str());
        strcpy(settlementConfirmReq.InvestorID, m_investorid.c_str());
        ret = m_pAPI->ReqSettlementInfoConfirm(&settlementConfirmReq, m_requestid++);
        if (TAPIERROR_SUCCEED != ret) {
            cout << "ReqSettlementInfoConfirm Error:" << ret << endl;
            return;
        }
        //等待m_bconfirm_status
        m_Event.WaitEvent();
        if (!m_bconfirm_status) {
            return;
        }

        ret = qry_postion();
        if (TAPIERROR_SUCCEED != ret) {
            cout << "qry_postion Error:" << ret << endl;
            return;
        }
        //等待qry_postion
        m_Event.WaitEvent();
        if (!m_bposition) {
            return;
        }

        ret = qry_order();
        if (TAPIERROR_SUCCEED != ret) {
            cout << "qry_order Error:" << ret << endl;
            return;
        }
        m_Event.WaitEvent();
        if (!m_border) {
            return;
        }

        log_str = "交易服务登录完成";
        APP_LOG(applog::LOG_INFO) << log_str.toStdString();
        emit signals_write_log(log_str);

        m_connect_state = true;
    }

    int Trade::qry_postion()
    {
        CThostFtdcQryInvestorPositionField postionReq;
        memset(&postionReq, 0, sizeof(postionReq));
        strcpy(postionReq.BrokerID, m_brokerid.c_str());
        strcpy(postionReq.InvestorID, m_investorid.c_str());
        //strcpy(postionReq.InstrumentID, g_pTradeInstrumentID);
        std::this_thread::sleep_for(std::chrono::milliseconds(700)); // 有时候需要停顿一会才能查询成功
        return m_pAPI->ReqQryInvestorPosition(&postionReq, m_requestid++);
    }

    int Trade::qry_order()
    {
        CThostFtdcQryOrderField req;
        memset(&req, 0, sizeof(req));
        strcpy(req.BrokerID, m_brokerid.c_str());
        strcpy(req.InvestorID, m_investorid.c_str());

        return m_pAPI->ReqQryOrder(&req, m_requestid++);
    }

    void Trade::order_open(string& account, string& contract, double price)
    {
        QString log_str = QObject::tr("%1%2 %3").arg("挂单卖").arg(contract.c_str()).arg(price);
        APP_LOG(applog::LOG_INFO) << log_str.toStdString();
        emit signals_write_log(log_str);

        int ret = TAPIERROR_SUCCEED;
        //下单
        CThostFtdcInputOrderField orderInsertReq;
        memset(&orderInsertReq, 0, sizeof(orderInsertReq));
        ///经纪公司代码
        strcpy(orderInsertReq.BrokerID, m_brokerid.c_str());
        ///投资者代码
        strcpy(orderInsertReq.InvestorID, m_investorid.c_str());
        ///合约代码
        strcpy(orderInsertReq.InstrumentID, contract.c_str());
        ///报单引用
        strcpy(orderInsertReq.OrderRef, std::to_string(m_requestid).c_str());
        ///报单价格条件: 限价
        orderInsertReq.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
        ///买卖方向: 
        orderInsertReq.Direction = THOST_FTDC_D_Sell;
        ///组合开平标志: 开仓
        orderInsertReq.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
        ///组合投机套保标志
        orderInsertReq.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
        ///价格
        orderInsertReq.LimitPrice = price;
        ///数量：1
        orderInsertReq.VolumeTotalOriginal = 1;
        ///有效期类型: 当日有效
        orderInsertReq.TimeCondition = THOST_FTDC_TC_GFD;
        ///成交量类型: 任何数量
        orderInsertReq.VolumeCondition = THOST_FTDC_VC_AV;
        ///最小成交量: 1
        orderInsertReq.MinVolume = 1;
        ///触发条件: 立即
        orderInsertReq.ContingentCondition = THOST_FTDC_CC_Immediately;
        ///强平原因: 非强平
        orderInsertReq.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
        ///自动挂起标志: 否
        orderInsertReq.IsAutoSuspend = 0;
        ///用户强评标志: 否
        orderInsertReq.UserForceClose = 0;

        ret = m_pAPI->ReqOrderInsert(&orderInsertReq, m_requestid++);
        if (TAPIERROR_SUCCEED != ret) {
            QString log_str = QObject::tr("%1%2").arg("卖开失败，错误码:").
                arg(ret);
            APP_LOG(applog::LOG_INFO) << log_str.toStdString();
            emit signals_write_log(log_str);
            return;
        }
    }
    void Trade::order_withdraw()
    {
        int ret = TAPIERROR_SUCCEED;
        string key = "order_info/ExchangeID";
        QString ExchangeID = common::get_config_value(key).toString();
        key = "order_info/OrderSysID";
        QString OrderSysID = common::get_config_value(key).toString();

        QString log_str = QObject::tr("%1%2").arg("撤单，流水号:").arg(OrderSysID);
        APP_LOG(applog::LOG_INFO) << log_str.toStdString();
        emit signals_write_log(log_str);

        CThostFtdcInputOrderActionField orderActionReq;
        memset(&orderActionReq, 0, sizeof(orderActionReq));
        ///经纪公司代码
        strcpy(orderActionReq.BrokerID, m_brokerid.c_str());
        ///投资者代码
        strcpy(orderActionReq.InvestorID, m_investorid.c_str());
        ///交易所代码
        strcpy(orderActionReq.ExchangeID, ExchangeID.toStdString().c_str());
        ///报单编号
        strcpy(orderActionReq.OrderSysID, OrderSysID.toStdString().c_str());
        ///操作标志
        orderActionReq.ActionFlag = THOST_FTDC_AF_Delete;

        ret = m_pAPI->ReqOrderAction(&orderActionReq, m_requestid++);
        if (TAPIERROR_SUCCEED != ret) {
            QString log_str = QObject::tr("%1%2").arg("撤单失败，错误码:").
                arg(ret);
            APP_LOG(applog::LOG_INFO) << log_str.toStdString();
            emit signals_write_log(log_str);
            return;
        }
    }
    void Trade::order_close(string& account, string& contract, double price)
    {
        QString log_str = QObject::tr("%1%2").arg("市价平仓").arg(contract.c_str());
        APP_LOG(applog::LOG_INFO) << log_str.toStdString();
        emit signals_write_log(log_str);
        int ret = TAPIERROR_SUCCEED;

        CThostFtdcInputOrderField orderInsertReq;
        memset(&orderInsertReq, 0, sizeof(orderInsertReq));
        ///经纪公司代码
        strcpy(orderInsertReq.BrokerID, m_brokerid.c_str());
        ///投资者代码
        strcpy(orderInsertReq.InvestorID, m_investorid.c_str());
        ///合约代码
        strcpy(orderInsertReq.InstrumentID, contract.c_str());
        ///报单引用
        strcpy(orderInsertReq.OrderRef, std::to_string(m_requestid).c_str());
        ///报单价格条件: 限价
        orderInsertReq.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
        ///买卖方向: 
        orderInsertReq.Direction = THOST_FTDC_D_Buy;
        ///组合开平标志: 开仓
        orderInsertReq.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
        ///组合投机套保标志
        orderInsertReq.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
        ///价格
        orderInsertReq.LimitPrice = price;
        ///数量：1
        orderInsertReq.VolumeTotalOriginal = DEFAULT_ORDER_QTY;
        ///有效期类型: 当日有效
        orderInsertReq.TimeCondition = THOST_FTDC_TC_IOC;
        ///成交量类型: 任何数量
        orderInsertReq.VolumeCondition = THOST_FTDC_VC_AV;
        ///最小成交量: 1
        orderInsertReq.MinVolume = 1;
        ///触发条件: 立即
        orderInsertReq.ContingentCondition = THOST_FTDC_CC_Immediately;
        ///强平原因: 非强平
        orderInsertReq.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
        ///自动挂起标志: 否
        orderInsertReq.IsAutoSuspend = 0;
        ///用户强评标志: 否
        orderInsertReq.UserForceClose = 0;

        ret = m_pAPI->ReqOrderInsert(&orderInsertReq, m_requestid++);
        if (TAPIERROR_SUCCEED != ret) {
            QString log_str = QObject::tr("%1%2").arg("买平失败，错误码:").
                arg(ret);
            APP_LOG(applog::LOG_INFO) << log_str.toStdString();
            emit signals_write_log(log_str);
            return;
        }
    }

    void Trade::order_state_handle(const CThostFtdcOrderField *info)
    {
        //! 委托状态类型
        //#define THOST_FTDC_OST_AllTraded '0'                    ///全部成交 (最终状态)
        //#define THOST_FTDC_OST_PartTradedQueueing '1'           ///部分成交还在队列中 （可撤单）
        //#define THOST_FTDC_OST_PartTradedNotQueueing '2'        ///部分成交不在队列中 (最终状态)
        //#define THOST_FTDC_OST_NoTradeQueueing '3'              ///未成交还在队列中 （可撤单）
        //#define THOST_FTDC_OST_NoTradeNotQueueing '4'           ///未成交不在队列中 (最终状态)
        //#define THOST_FTDC_OST_Canceled '5'                     ///撤单 (最终状态)
        //#define THOST_FTDC_OST_Unknown 'a'                      ///未知 (成交时会推送)
        //#define THOST_FTDC_OST_NotTouched 'b'                   ///尚未触发 ？
        //#define THOST_FTDC_OST_Touched 'c'                      ///已触发 ？
        switch (info->OrderStatus) {
        case THOST_FTDC_OST_Unknown:
        case THOST_FTDC_OST_NotTouched:
        {
            QString log_str = QObject::tr("%1").arg("委托已受理");
            APP_LOG(applog::LOG_INFO) << log_str.toStdString();
            emit signals_write_log(log_str);
            break;
        }
        case THOST_FTDC_OST_NoTradeQueueing:
        case THOST_FTDC_OST_NoTradeNotQueueing: //最终状态？
        case THOST_FTDC_OST_Touched:
        {
            string key = "order_info/ExchangeID";
            common::set_config_value(key, string(info->ExchangeID));
            key = "order_info/OrderSysID";
            common::set_config_value(key, string(info->OrderSysID));

            emit signals_state_changed(THOST_FTDC_OST_Touched,
                '0',
                QString::number(0, 10, 0));

            QString log_str = QObject::tr("%1%2").arg("委托已排队,流水号:").
                arg(info->OrderSysID);
            APP_LOG(applog::LOG_INFO) << log_str.toStdString();
            emit signals_write_log(log_str);
            break;
        }
        case THOST_FTDC_OST_PartTradedQueueing:
        case THOST_FTDC_OST_PartTradedNotQueueing: //最终状态？
        {
            APP_LOG(applog::LOG_INFO) << "部分成交";
            break;
        }
        case THOST_FTDC_OST_AllTraded:
        {
            APP_LOG(applog::LOG_INFO) << "完全成交";
            //emit signals_state_changed(THOST_FTDC_OST_AllTraded,
            //    '0',
            //    QString::number(0, 10, 0));
            break;
        }
        case THOST_FTDC_OST_Canceled:
        {
            emit signals_state_changed(THOST_FTDC_OST_Canceled,
                '0',
                QString::number(0, 10, 0));

            QString log_str = QObject::tr("%1%2%3%4").arg("撤单成功,流水号:").
                arg(info->OrderSysID).arg("状态信息：").arg(QString::fromLocal8Bit(info->StatusMsg));;
            APP_LOG(applog::LOG_INFO) << log_str.toStdString();
            emit signals_write_log(log_str);
            break;
        }
        default:
            break;
        }
    }
    void Trade::thread_reconnect()
    {
        while (m_running) {
            if (!m_connect_state) {
                Run();
            }
            Sleep(3000);
        }
    }

    bool Trade::is_error_rsp(CThostFtdcRspInfoField *pRspInfo)
    {
        bool bResult = pRspInfo && (pRspInfo->ErrorID != 0);
        if (bResult)
            std::cerr << "返回错误ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
        return bResult;
    }

    void Trade::OnFrontConnected()
    {
        QString log_str = "交易API连接成功";
        APP_LOG(applog::LOG_INFO) << log_str.toStdString();
        emit signals_write_log(log_str);
        m_bfront_status = true;
        m_Event.SignalEvent();
    }

    void Trade::OnRspUserLogin(
        CThostFtdcRspUserLoginField *pRspUserLogin,
        CThostFtdcRspInfoField *pRspInfo,
        int nRequestID,
        bool bIsLast)
    {
        if (!is_error_rsp(pRspInfo)) {
            QString log_str = "交易API登录成功";
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

    void Trade::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
    {
        is_error_rsp(pRspInfo);
    }

    void Trade::OnFrontDisconnected(int nReason)
    {
        if (!m_running) return;
        QString log_str = QObject::tr("%1%2").arg("交易API断开,断开原因:").
            arg(nReason);
        APP_LOG(applog::LOG_INFO) << log_str.toStdString();
        emit signals_write_log(log_str);

        m_connect_state = false;
        if (m_chk_thread != nullptr) return;
        m_chk_thread = std::make_shared<std::thread>(
            std::bind(&Trade::thread_reconnect, this));
    }

    void Trade::OnHeartBeatWarning(int nTimeLapse)
    {
        std::cerr << "=====网络心跳超时=====" << std::endl;
        std::cerr << "距上次连接时间： " << nTimeLapse << std::endl;
    }

    void Trade::OnRspUserLogout(
        CThostFtdcUserLogoutField *pUserLogout,
        CThostFtdcRspInfoField *pRspInfo,
        int nRequestID,
        bool bIsLast)
    {
        std::cout << __FUNCTION__ << std::endl;
        if (!is_error_rsp(pRspInfo)) {
            std::cout << "账户登出成功" << std::endl;
            std::cout << "经纪商： " << pUserLogout->BrokerID << std::endl;
            std::cout << "帐户名： " << pUserLogout->UserID << std::endl;
        }
        else {
            std::cerr << "返回错误ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << std::endl;
        }
    }

    void Trade::OnRspSettlementInfoConfirm(
        CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm,
        CThostFtdcRspInfoField *pRspInfo,
        int nRequestID,
        bool bIsLast)
    {
        if (!is_error_rsp(pRspInfo)) {
            QString log_str = "交易API结算结果确认成功";
            APP_LOG(applog::LOG_INFO) << log_str.toStdString();
            emit signals_write_log(log_str);
            m_bconfirm_status = true;
        }
        else {
            QString log_str = QObject::tr("%1%2").arg("结算结果确认失败，错误码:").
                arg(pRspInfo->ErrorID);
            APP_LOG(applog::LOG_INFO) << log_str.toStdString();
            emit signals_write_log(log_str);

        }
        m_Event.SignalEvent();

    }

    void Trade::OnRspQryInstrument(
        CThostFtdcInstrumentField *pInstrument,
        CThostFtdcRspInfoField *pRspInfo,
        int nRequestID,
        bool bIsLast)
    {
        if (!is_error_rsp(pRspInfo)) {
            std::cout << "=====查询合约结果成功=====" << std::endl;
            std::cout << "交易所代码： " << pInstrument->ExchangeID << std::endl;
            std::cout << "合约代码： " << pInstrument->InstrumentID << std::endl;
            std::cout << "合约在交易所的代码： " << pInstrument->ExchangeInstID << std::endl;
            std::cout << "执行价： " << pInstrument->StrikePrice << std::endl;
            std::cout << "到期日： " << pInstrument->EndDelivDate << std::endl;
            std::cout << "当前交易状态： " << pInstrument->IsTrading << std::endl;
        }
    }

    void Trade::OnRspQryInvestorPosition(
        CThostFtdcInvestorPositionField *pInvestorPosition,
        CThostFtdcRspInfoField *pRspInfo,
        int nRequestID,
        bool bIsLast)
    {
        if (!is_error_rsp(pRspInfo)) {
            if (pInvestorPosition == nullptr) {
                m_bposition = true;
                m_Event.SignalEvent();
                return;
            }

            if (!bIsLast) {
                CThostFtdcInputOrderField orderInsertReq;
                memset(&orderInsertReq, 0, sizeof(orderInsertReq));
                ///经纪公司代码
                strcpy(orderInsertReq.BrokerID, m_brokerid.c_str());
                ///投资者代码
                strcpy(orderInsertReq.InvestorID, m_investorid.c_str());
                ///合约代码
                strcpy(orderInsertReq.InstrumentID, pInvestorPosition->InstrumentID);
                ///报单引用
                strcpy(orderInsertReq.OrderRef, std::to_string(m_requestid).c_str());
                ///报单价格条件: 限价
                orderInsertReq.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
                ///买卖方向: 
                orderInsertReq.Direction = (pInvestorPosition->PosiDirection == THOST_FTDC_PD_Short) ?
                THOST_FTDC_D_Buy : THOST_FTDC_D_Sell;
                ///组合开平标志: 开仓
                orderInsertReq.CombOffsetFlag[0] = THOST_FTDC_OF_Close;
                ///组合投机套保标志
                orderInsertReq.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
                ///价格
                orderInsertReq.LimitPrice = pInvestorPosition->PreSettlementPrice + 100;
                ///数量：1
                orderInsertReq.VolumeTotalOriginal = pInvestorPosition->Position;
                ///有效期类型: 当日有效
                orderInsertReq.TimeCondition = THOST_FTDC_TC_GFD;
                ///成交量类型: 任何数量
                orderInsertReq.VolumeCondition = THOST_FTDC_VC_AV;
                ///最小成交量: 1
                orderInsertReq.MinVolume = 1;
                ///触发条件: 立即
                orderInsertReq.ContingentCondition = THOST_FTDC_CC_Immediately;
                ///强平原因: 非强平
                orderInsertReq.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
                ///自动挂起标志: 否
                orderInsertReq.IsAutoSuspend = 0;
                ///用户强评标志: 否
                orderInsertReq.UserForceClose = 0;

                int ret = m_pAPI->ReqOrderInsert(&orderInsertReq, m_requestid++);
                if (TAPIERROR_SUCCEED != ret) {
                    QString log_str = QObject::tr("%1%2").arg("买平失败，错误码:").
                        arg(ret);
                    APP_LOG(applog::LOG_INFO) << log_str.toStdString();
                    emit signals_write_log(log_str);
                    return;
                }
            }
            else {
                emit signals_close_position("", pInvestorPosition->InstrumentID);
                m_bposition = true;
                m_Event.SignalEvent();
            }
        }
    }

    void Trade::OnRspQryOrder(CThostFtdcOrderField *pOrder, 
        CThostFtdcRspInfoField *pRspInfo, 
        int nRequestID, 
        bool bIsLast)
    {
        if (!is_error_rsp(pRspInfo)) {
            if (pOrder == nullptr) {
                m_border = true;
                m_Event.SignalEvent();
                return;
            }

            if (pOrder->OrderStatus == THOST_FTDC_OST_PartTradedQueueing ||
                pOrder->OrderStatus == THOST_FTDC_OST_NoTradeQueueing) {
                m_map_order[pOrder->OrderSysID] = pOrder->ExchangeID;
            }

            if (bIsLast) {
                if (m_map_order.size() != 0) {
                    auto last = m_map_order.end();
                    last--;
                    for (auto it = m_map_order.begin(); it != last; it++) {
                        CThostFtdcInputOrderActionField orderActionReq;
                        memset(&orderActionReq, 0, sizeof(orderActionReq));
                        ///经纪公司代码
                        strcpy(orderActionReq.BrokerID, m_brokerid.c_str());
                        ///投资者代码
                        strcpy(orderActionReq.InvestorID, m_investorid.c_str());
                        ///交易所代码
                        strcpy(orderActionReq.ExchangeID, it->second.c_str());
                        ///报单编号
                        strcpy(orderActionReq.OrderSysID, it->first.c_str());
                        ///操作标志
                        orderActionReq.ActionFlag = THOST_FTDC_AF_Delete;

                        int ret = m_pAPI->ReqOrderAction(&orderActionReq, m_requestid++);
                        if (TAPIERROR_SUCCEED != ret) {
                            QString log_str = QObject::tr("%1%2").arg("撤单失败，错误码:").
                                arg(ret);
                            APP_LOG(applog::LOG_INFO) << log_str.toStdString();
                            emit signals_write_log(log_str);
                            return;
                        }
                    }
                    string key = "order_info/ExchangeID";
                    common::set_config_value(key, last->second);
                    key = "order_info/OrderSysID";
                    common::set_config_value(key, string(last->first));
                    emit signals_withdraw_order(last->first.c_str());
                }


                m_border = true;
                m_Event.SignalEvent();
            }
        }
    }

    void Trade::OnRspQryTrade(CThostFtdcTradeField *pTrade, 
        CThostFtdcRspInfoField *pRspInfo, 
        int nRequestID, 
        bool bIsLast)
    {
        std::cout << __FUNCTION__ << std::endl;
    }

    void Trade::OnRspOrderInsert(
        CThostFtdcInputOrderField *pInputOrder,
        CThostFtdcRspInfoField *pRspInfo,
        int nRequestID,
        bool bIsLast)
    {
        std::cout << __FUNCTION__ << std::endl;
        if (!is_error_rsp(pRspInfo)) {
            std::cout << "=====报单录入成功=====" << std::endl;
            std::cout << "合约代码： " << pInputOrder->InstrumentID << std::endl;
            std::cout << "价格： " << pInputOrder->LimitPrice << std::endl;
            std::cout << "数量： " << pInputOrder->VolumeTotalOriginal << std::endl;
            std::cout << "开仓方向： " << pInputOrder->Direction << std::endl;
        } 
        else {
            QString log_str = QObject::tr("交易核心返回报单失败,错误码:%1,错误信息:%2").
                arg(pRspInfo->ErrorID).
                arg(QString::fromLocal8Bit(pRspInfo->ErrorMsg));
            APP_LOG(applog::LOG_INFO) << log_str.toStdString();
            emit signals_write_log(log_str);
        }
    }

    void Trade::OnRspOrderAction(
        CThostFtdcInputOrderActionField *pInputOrderAction,
        CThostFtdcRspInfoField *pRspInfo,
        int nRequestID,
        bool bIsLast)
    {
        std::cout << __FUNCTION__ << std::endl;
        if (!is_error_rsp(pRspInfo)) {
            std::cout << "=====报单操作成功=====" << std::endl;
            std::cout << "合约代码： " << pInputOrderAction->InstrumentID << std::endl;
            std::cout << "操作标志： " << pInputOrderAction->ActionFlag;
        } 
        else {
            QString log_str = QObject::tr("交易核心返回撤单失败,错误码:%1,错误信息:%2").
                arg(pRspInfo->ErrorID).
                arg(QString::fromLocal8Bit(pRspInfo->ErrorMsg));
            APP_LOG(applog::LOG_INFO) << log_str.toStdString();
            emit signals_write_log(log_str);
        }
    }

    void Trade::OnRtnOrder(CThostFtdcOrderField *pOrder)
    {
        std::cout << __FUNCTION__ << std::endl;
        std::cout << "=====收到报单应答=====" << std::endl;
        APP_LOG(applog::LOG_INFO)
            << "OrderRef： " << pOrder->OrderRef << " "
            << "FrontID： " << pOrder->FrontID << " "
            << "SessionID： " << pOrder->SessionID << " "
            << "Direction： " << pOrder->Direction << " "
            << "CombOffsetFlag： " << pOrder->CombOffsetFlag << " "
            << "CombHedgeFlag： " << pOrder->CombHedgeFlag << " "
            << "LimitPrice： " << pOrder->LimitPrice << " "
            << "VolumeTotalOriginal： " << pOrder->VolumeTotalOriginal << " "
            << "RequestID： " << pOrder->RequestID << " "
            << "OrderLocalID： " << pOrder->OrderLocalID << " "
            << "ExchangeID： " << pOrder->ExchangeID << " "
            << "TraderID： " << pOrder->TraderID << " "
            << "OrderSubmitStatus： " << pOrder->OrderSubmitStatus << " "
            << "TradingDay： " << pOrder->TradingDay << " "
            << "OrderSysID： " << pOrder->OrderSysID << " "
            << "OrderStatus： " << pOrder->OrderStatus << " "
            << "OrderType： " << pOrder->OrderType << " "
            << "VolumeTraded： " << pOrder->VolumeTraded << " "
            << "VolumeTotal： " << pOrder->VolumeTotal << " "
            << "InsertDate： " << pOrder->InsertDate << " "
            << "InsertTime： " << pOrder->InsertTime << " "
            << "SequenceNo： " << pOrder->SequenceNo << " "
            << "StatusMsg： " << QString::fromLocal8Bit(pOrder->StatusMsg).toStdString();
        if (NULL == pOrder) return;
        //if (pOrder->OrderStatus != THOST_FTDC_OST_Canceled
        //    && pOrder->OrderStatus != THOST_FTDC_OST_AllTraded
        //    && pOrder->OrderStatus != THOST_FTDC_OST_PartTradedQueueing
        //    && pOrder->OrderStatus != THOST_FTDC_OST_PartTradedNotQueueing) {
        //        APP_LOG(applog::LOG_INFO) << "报单成功，"
        //            << "状态:" << pOrder->OrderStatus << ","
        //            << "委托编号:" << pOrder->OrderSysID ;
        //        QString log_str = QObject::tr("报单成功,状态:%1,委托编号:%2").
        //            arg(pOrder->OrderStatus).
        //            arg(pOrder->OrderSysID);
        //        APP_LOG(applog::LOG_INFO) << log_str.toStdString();
        //        emit signals_write_log(log_str);
        //}
        order_state_handle(pOrder);
    }

    void Trade::OnRtnTrade(CThostFtdcTradeField *pTrade)
    {
        std::cout << __FUNCTION__ << std::endl;
        std::cout << "=====报单成功成交=====" << std::endl;
        APP_LOG(applog::LOG_INFO)
            << "OrderRef： " << pTrade->OrderRef << " "
            << "ExchangeID： " << pTrade->ExchangeID << " "
            << "TradeID： " << pTrade->TradeID << " "
            << "Direction： " << pTrade->Direction << " "
            << "OrderSysID： " << pTrade->OrderSysID << " "
            << "OffsetFlag： " << pTrade->OffsetFlag << " "
            << "HedgeFlag： " << pTrade->HedgeFlag << " "
            << "Price： " << pTrade->Price << " "
            << "Volume： " << pTrade->Volume << " "
            << "TradeDate： " << pTrade->TradeDate << " "
            << "TradeTime： " << pTrade->TradeTime << " "
            << "TradeType： " << pTrade->TradeType << " "
            << "TraderID： " << pTrade->TraderID << " "
            << "OrderLocalID： " << pTrade->OrderLocalID << " "
            << "SequenceNo： " << pTrade->SequenceNo << " "
            << "TradingDay： " << pTrade->TradingDay << " "
            << "TradeSource： " << pTrade->TradeSource;

        emit signals_state_changed(THOST_FTDC_OST_AllTraded,
            pTrade->Direction,
            QString::number(pTrade->Price, 10, 2));

        QString log_str;
        if (pTrade->Direction == THOST_FTDC_D_Sell) {
            log_str = QObject::tr("卖%1交易成功，流水号%2").
                arg(pTrade->Price).
                arg(pTrade->TradeID);
        }
        else {
            log_str = QObject::tr("%1平仓成功，流水号%2").
                arg(pTrade->Price).
                arg(pTrade->TradeID);
        }
        APP_LOG(applog::LOG_INFO) << log_str.toStdString();
        emit signals_write_log(log_str);
    }


    ///报单录入错误回报
    void Trade::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
    {
        std::cout << __FUNCTION__ << std::endl;
    }

    ///报单操作错误回报
    void Trade::OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo)
    {
        std::cout << __FUNCTION__ << std::endl;
    }
}