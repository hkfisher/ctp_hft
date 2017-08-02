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
#include "TradeConfig.h"

namespace future 
{
    trader_server::trader_server()
    {
        //创建API实例
        ctptrader_api_inst = CThostFtdcTraderApi::CreateFtdcTraderApi("./tr_con/");
        ctptrader_spi_inst = new Trade();
    }
    trader_server::~trader_server()
    {
        ctptrader_spi_inst->m_running = false;
        if (ctptrader_api_inst) {
            ctptrader_api_inst->Release();
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
        //启动
        ctptrader_spi_inst->SetAPI(ctptrader_api_inst);
        ctptrader_spi_inst->Run();
    }

    void trader_server::join_server()
    {
        cout << __FUNCTION__ << endl;
        ctptrader_api_inst->Join();
    }
}
