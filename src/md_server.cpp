/******************************************************************************
Copyright (c) 2016. All Rights Reserved.

FileName: md_server.cpp
Version: 1.0
Date: 2017.4.25

History:
shengkaishan     2017.4.25   1.0     Create
******************************************************************************/

#include "md_server.h"
#include "applog.h"
#include "QuoteConfig.h"

namespace future 
{
    md_server::md_server()
    {
        //创建API实例
        ctpmd_api_inst = CThostFtdcMdApi::CreateFtdcMdApi("./md_con/");   // 创建行情实例
        ctpmd_spi_inst = new Quote();
    }
    md_server::~md_server()
    {
        ctpmd_spi_inst->m_running = false;
        if (ctpmd_api_inst) {
            ctpmd_api_inst->Release();
        }
        if (ctpmd_spi_inst->m_chk_thread != nullptr) {
            if (ctpmd_spi_inst->m_chk_thread->joinable()) {
                ctpmd_spi_inst->m_chk_thread->join();
            }
        }
        delete ctpmd_spi_inst;
        ctpmd_spi_inst = NULL;
    }

    void md_server::start_server()
    {
        //启动测试，订阅行情
        ctpmd_spi_inst->SetAPI(ctpmd_api_inst);
        ctpmd_spi_inst->Run();
    }

    void md_server::join_server()
    {
        cout << __FUNCTION__ << endl;
        ctpmd_api_inst->Join();
    }
}
