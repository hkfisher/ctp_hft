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

#include "iTapTradeAPI.h"
#include "SimpleEvent.h"
#include <map>
#include<QObject>
#include<QString>
#include <memory>
#include <thread>
#include <atomic>
using namespace std;
using namespace ITapTrade;
namespace future
{
    class Trade : public QObject, public ITapTradeAPINotify
    {
        Q_OBJECT
    public:
        Trade(void);
        ~Trade(void);

        void SetAPI(ITapTradeAPI *pAPI);
        void Run();

        void qry_postion();
        void qry_order();
        void order_open(string& account, string& contract, double price);
        void order_withdraw();
        void order_close(string& account, string& contract);

    signals:
        void signals_write_log(QString str);
        void signals_state_changed(char state, char effect, QString deal_price);
        void signals_close_position(QString commodity_no, QString contract_no);
        void signals_withdraw_order(QString order_no);

    private:
        void order_state_handle(const TapAPIOrderInfoNotice *info);
        void thread_reconnect();
    public:
        //对ITapTradeAPINotify的实现
        virtual void TAP_CDECL OnConnect();

        virtual void TAP_CDECL OnRspLogin(TAPIINT32 errorCode, const TapAPITradeLoginRspInfo *loginRspInfo);

        virtual void TAP_CDECL OnAPIReady();

        virtual void TAP_CDECL OnDisconnect(TAPIINT32 reasonCode);

        virtual void TAP_CDECL OnRspChangePassword(TAPIUINT32 sessionID, TAPIINT32 errorCode);

        virtual void TAP_CDECL OnRspSetReservedInfo(TAPIUINT32 sessionID, TAPIINT32 errorCode, const TAPISTR_50 info);

        virtual void TAP_CDECL OnRspQryAccount(TAPIUINT32 sessionID, TAPIUINT32 errorCode, TAPIYNFLAG isLast, const TapAPIAccountInfo *info);

        virtual void TAP_CDECL OnRspQryFund(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPIFundData *info);

        virtual void TAP_CDECL OnRtnFund(const TapAPIFundData *info);

        virtual void TAP_CDECL OnRspQryExchange(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPIExchangeInfo *info);

        virtual void TAP_CDECL OnRspQryCommodity(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPICommodityInfo *info);

        virtual void TAP_CDECL OnRspQryContract(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPITradeContractInfo *info);

        virtual void TAP_CDECL OnRtnContract(const TapAPITradeContractInfo *info);

        virtual void TAP_CDECL OnRtnOrder(const TapAPIOrderInfoNotice *info);

        virtual void TAP_CDECL OnRspOrderAction(TAPIUINT32 sessionID, TAPIUINT32 errorCode, const TapAPIOrderActionRsp *info);

        virtual void TAP_CDECL OnRspQryOrder(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPIOrderInfo *info);

        virtual void TAP_CDECL OnRspQryOrderProcess(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPIOrderInfo *info);

        virtual void TAP_CDECL OnRspQryFill(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPIFillInfo *info);

        virtual void TAP_CDECL OnRtnFill(const TapAPIFillInfo *info);

        virtual void TAP_CDECL OnRspQryPosition(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPIPositionInfo *info);

        virtual void TAP_CDECL OnRtnPosition(const TapAPIPositionInfo *info);

        virtual void TAP_CDECL OnRspQryClose(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPICloseInfo *info);

        virtual void TAP_CDECL OnRtnClose(const TapAPICloseInfo *info);

        virtual void TAP_CDECL OnRtnPositionProfit(const TapAPIPositionProfitNotice *info);

        virtual void TAP_CDECL OnRspQryCurrency(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPICurrencyInfo *info);

        virtual void TAP_CDECL OnRspQryTradeMessage(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPITradeMessage *info);

        virtual void TAP_CDECL OnRtnTradeMessage(const TapAPITradeMessage *info);

        virtual void TAP_CDECL OnRspQryHisOrder(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPIHisOrderQryRsp *info);

        virtual void TAP_CDECL OnRspQryHisOrderProcess(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPIHisOrderProcessQryRsp *info);

        virtual void TAP_CDECL OnRspQryHisMatch(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPIHisMatchQryRsp *info);

        virtual void TAP_CDECL OnRspQryHisPosition(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPIHisPositionQryRsp *info);

        virtual void TAP_CDECL OnRspQryHisDelivery(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPIHisDeliveryQryRsp *info);

        virtual void TAP_CDECL OnRspQryAccountCashAdjust(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPIAccountCashAdjustQryRsp *info);

        virtual void TAP_CDECL OnRspQryBill(TAPIUINT32 sessionID, TAPIINT32 errorCode, TAPIYNFLAG isLast, const TapAPIBillQryRsp *info);

        virtual void TAP_CDECL OnExpriationDate(ITapTrade::TAPIDATE date, int days);

        virtual void TAP_CDECL OnRspQryAccountFeeRent(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPIAccountFeeRentQryRsp *info);

        virtual void TAP_CDECL OnRspQryAccountMarginRent(ITapTrade::TAPIUINT32 sessionID, ITapTrade::TAPIINT32 errorCode, ITapTrade::TAPIYNFLAG isLast, const ITapTrade::TapAPIAccountMarginRentQryRsp *info);

    private:
        ITapTradeAPI *m_pAPI;
        TAPIUINT32  m_uiSessionID;
        SimpleEvent m_Event;
        bool        m_bIsAPIReady;
        bool        m_bContract;
        bool        m_bposition;
        bool        m_border;
        map<string, string> m_map_contract;

        bool        m_connect_state;

        map<string, char> m_map_order;
    public:
        atomic<bool> m_running;
        std::shared_ptr<std::thread> m_chk_thread;
    };
}
#endif // TRADE_H
