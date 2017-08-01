/******************************************************************************
Copyright (c) 2016. All Rights Reserved.

FileName: common.h
Version: 1.0
Date: 2017.4.25

History:
shengkaishan     2017.4.25   1.0     Create
******************************************************************************/


#ifndef __COMMON_H__
#define __COMMON_H__

#include <string>
#include <QSettings>
#include <QFile>
#include <QTextCodec>
#include <QDebug>
using namespace std;

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#if defined(_MSC_VER) && (_MSC_VER > 1600)
// Coding: UTF-8
#pragma execution_character_set("utf-8")
#endif
#endif

namespace future
{
    const int MD_TYPE = 0;
    const int TRADER_TYPE = 1;

    const string close_picture = "./picture/close.ico";
    const string log_picture = "./picture/future.ico";
    class common
    {
    public:
        common(){};
        ~common(){};
        static QString get_env_path()
        {
            char* server_path = getenv("FUTURE_PLATFORM_HOME");
            if (NULL == server_path) {
                throw "环境变量未设置";
            }

            return server_path;
        }

        static QVariant get_config_value(string &key)
        {
            //QString server_path = get_env_path();
            //QString conf_path = server_path + "/config/config.ini";
            QString conf_path = QObject::tr("./config/config.ini");
            if (!QFile::exists(conf_path)) {
                return QVariant();
            }
            QSettings settings(conf_path, QSettings::IniFormat);
            return settings.value(key.c_str());
        }

        static void set_config_value(string &key, string &value)
        {
            try {
                //QString server_path = get_env_path();
                //QString conf_path = server_path + "/config/config.ini";
                QString conf_path = QObject::tr("./config/config.ini");
                if (!QFile::exists(conf_path)) {
                    return;
                }

                QSettings settings(conf_path, QSettings::IniFormat);
                settings.setValue(key.c_str(), value.c_str());
            }
            catch (...) {
            }
        }

        static QString getXorEncryptDecrypt(const QString &str, 
            const char &key = 17)
        {
            QString result;
            QByteArray bs = qstringToByte(str);

            for (int i = 0; i < bs.size(); i++) {
                bs[i] = bs[i] ^ key;
            }

            result = byteToQString(bs);
            return result;
        }

    private:
        static QString byteToQString(const QByteArray &byte)
        {
            QString result;
            if (byte.size() > 0) {
                QTextCodec *codec = QTextCodec::codecForName("utf-8");
                result = codec->toUnicode(byte);
            }

            return result;
        }
        static QByteArray qstringToByte(const QString &strInfo)
        {
            QByteArray result;
            if (strInfo.length() > 0) {
                QTextCodec *codec = QTextCodec::codecForName("utf-8");
                result = codec->fromUnicode(strInfo);
            }

            return result;
        }
    };
}
#endif