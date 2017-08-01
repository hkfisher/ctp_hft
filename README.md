# future_platform
QT做界面，使用esunny外盘api实现一个快速行情、交易工具。

# 编译环境
windows7 + vs2013

# qt版本
qt-opensource-windows-x86-msvc2013-5.5.1.exe
qt-vs-addin-1.2.4-opensource.exe

# esunny api版本
9.0行情API和9.0交易API
http://www.esunny.com.cn/index.php?m=content&c=index&a=lists&catid=71

# QA
行情和交易同时使用，头文件iTapAPICommDef.h/TapAPICommDef.h出现冲突，现取交易头文件iTapAPICommDef.h,
并在TapQuoteAPIDataType.h文件中修改
#include "iTapAPICommDef.h"
using namespace ITapTrade;

# 依赖QT,安装配置可参考
http://blog.csdn.net/liang19890820/article/details/49874033

