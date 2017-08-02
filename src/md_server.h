/******************************************************************************
Copyright (c) 2016. All Rights Reserved.

FileName: md_server.h
Version: 1.0
Date: 2017.4.25

History:
shengkaishan     2017.4.25   1.0     Create
******************************************************************************/

#ifndef __MD_SERVER_H__
#define __MD_SERVER_H__

#include <iostream>
#include <string>
#include <string.h>
#include "singleton.h"
#include "ThostFtdcMdApi.h"
#include "Quote.h"
#include "common.h"
namespace future 
{
    class md_server : public singleton<md_server>
    { 
    private:
        md_server();
        virtual ~md_server();

        friend class singleton<md_server>;
    public:
        void start_server();
        void join_server();

        Quote* get_md_spi()
        {
            return ctpmd_spi_inst;
        }
    private:
        CThostFtdcMdApi* ctpmd_api_inst;
        Quote *ctpmd_spi_inst;
    }; 

}
#endif
