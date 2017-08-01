/******************************************************************************
Copyright (c) 2016. All Rights Reserved.

FileName: trader_server.h
Version: 1.0
Date: 2017.4.25

History:
shengkaishan     2017.4.25   1.0     Create
******************************************************************************/


#ifndef __TRADER_SERVER_H__
#define __TRADER_SERVER_H__

#include <iostream>
#include <string>
#include <string.h>
#include "singleton.h"
#include "common.h"
#include "iTapTradeAPI.h"
#include "Trade.h"

namespace future
{
    class future_platform;
    class trader_server 
        : public singleton<trader_server>
    { 
    private:
        trader_server();
        virtual ~trader_server();

        friend class singleton<trader_server>;
    public:
        void start_server();
        void join_server();

        void set_future_platform(future_platform* inst)
        {
            m_future_platform = inst;
        }
        future_platform* get_future_platform()
        {
            return m_future_platform;
        }

        Trade* get_trader_spi()
        {
            return ctptrader_spi_inst;
        }
    private:
        ITapTradeAPI* ctptrader_api_inst;
        Trade* ctptrader_spi_inst;

        future_platform* m_future_platform;
    }; 

}
#endif
