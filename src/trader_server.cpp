/******************************************************************************
Copyright (c) 2016. All Rights Reserved.

FileName: trader_server.cpp
Version: 1.0
Date: 2017.4.25

History:
shengkaishan     2017.4.25   1.0     Create
******************************************************************************/


#include "trader_server.h"
#include "applog.h"
#include "iTapAPIError.h"
//#include "Trade.h"
#include "TradeConfig.h"

namespace future 
{
    trader_server::trader_server()
    {
        //取得API的版本信息
        cout << GetITapTradeAPIVersion() << endl;

        //创建API实例
        string key = "trader_info/authcode";
        QString authcode = common::get_config_value(key).toString();

        TAPIINT32 iResult = TAPIERROR_SUCCEED;
        TapAPIApplicationInfo stAppInfo;
        strcpy(stAppInfo.AuthCode, authcode.toStdString().c_str());
        strcpy(stAppInfo.KeyOperationLogPath, "log");
        ctptrader_api_inst = CreateITapTradeAPI(&stAppInfo, iResult);
        ctptrader_spi_inst = new Trade();
    }
    trader_server::~trader_server()
    {
        ctptrader_spi_inst->m_running = false;
        if (ctptrader_api_inst) {
            //ctptrader_api_inst->SetAPINotify(nullptr);
            ctptrader_api_inst->Disconnect();
            FreeITapTradeAPI(ctptrader_api_inst);
        }
        if (ctptrader_spi_inst->m_chk_thread != nullptr) {
            if (ctptrader_spi_inst->m_chk_thread->joinable()) {
                ctptrader_spi_inst->m_chk_thread->join();
            }
        }
        delete ctptrader_spi_inst;
        ctptrader_spi_inst = NULL;
    }

    void trader_server::start_server()
    {  
        TAPIINT32 iErr = TAPIERROR_SUCCEED;
        //设定服务器IP、端口
        string key = "trader_info/ip";
        QString ip = common::get_config_value(key).toString();
        key = "trader_info/port";
        int port = common::get_config_value(key).toInt();
        iErr = ctptrader_api_inst->SetHostAddress(ip.toStdString().c_str(), port);
        if (TAPIERROR_SUCCEED != iErr) {
            cout << "SetHostAddress Error:" << iErr << endl;
            return;
        }

        ctptrader_api_inst->SetAPINotify(ctptrader_spi_inst);
        //启动
        ctptrader_spi_inst->SetAPI(ctptrader_api_inst);
        ctptrader_spi_inst->Run();
    }

    void trader_server::join_server()
    {
        cout << "end" << __FUNCTION__ << endl;
    }
}
