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
#include "Trade.h"

namespace future
{
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

        Trade* get_trader_spi()
        {
            return ctptrader_spi_inst;
        }
    private:
        CThostFtdcTraderApi* ctptrader_api_inst;
        Trade* ctptrader_spi_inst;
    }; 

}
#endif
