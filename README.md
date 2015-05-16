# tradingstrategyking
Automatically exported from code.google.com/p/tradingstrategyking

# 策略为王 #

 ★★1.编译注意事项   
         源码包括NetTS,StkLib,StkNet,StkUI,XpFace五个工程， 使用VS.net 2005 打开工作区Tsking_vc80.sln ，然后点生成，生成解决方案，生成成功后在trunk\Bin会生成可执行文件StkUI.exe，      运行即可。
   
  ★★1.工程XPFace介绍    
       主要是本软件的一些界面美化的类，放在了这个DLL里面。   
   
  ★★2.工程StkLib介绍    
       这个工程是实现股票数据结构、技术指标计算的动态连接库，代码与平台无关。   
       其中比较重要的几个文件如下：    
       Database.h           取数据文件接口定义    
       QianLong.h           钱龙格式数据文件接口定义    
       SelfDB.h             自有格式数据文件接口定义，除了除权数据和行情数据外，   
                            其他与钱龙一样    
       Stock.h               股票数据结构定义    
       Technique.h         技术指标类定义    
       Container.h          股票信息数据CStockInfo的数组类   
       Express.h            股票列表视图的列变量定义，以及自定义列的表达式计算    
       Strategy.h           策略定义    
       Profile.h             记录软件的用户配置并保存    
      BaseData.cpp       基本资料数据结构类实现    
       Database.cpp        数据文件读写实现    
       DRData.cpp          除权数据结构数组类实现    
       KData.cpp           K线数据结构数组类实现    
       Report.cpp           行情数据结构数组类实现    
       Minute.cpp           行情分时数据结构数组类实现    
      Outline.cpp           行情额外数据结构数组类实现    
      QianLong.cpp         钱龙数据文件读写实现   
      SelfDB.cpp            自有格式数据文件读写实现    
      Stock.cpp              股票数据结构实现   
      StStore.cpp            数据文件读写实现    
      TechCL.cpp            自有技术指标   
      TechEnergy.cpp        能量类技术指标   
      TechKLine.cpp         K线叠加类技术指标   
      Technique.cpp         技术指标基类   
      TechSwing.cpp        摆动类技术指标   
      TechTrend.cpp         趋势类技术指标   
      TechOther.cpp         其它类技术指标   
      Container.cpp          股票信息数据CStockInfo的数组类实现   
      Express.cpp           股票列表视图的列变量定义，以及自定义列的表达式

计算实现    

Strategy.cpp            策略定义实现    
     Profile.cpp           记录软件的用户配置并保存工程StkLib的几个全局静态变量：    
 g_stockcontainer：AfxGetStockContainer()     可以得到该变量的引用，   该变量记录所有股票的行情信息和基本信息，CStockInfo的数组。   
g_domaincontainer：AfxGetDomainContainer()   可以得到该变量的引用，   
                                                          该变量记录所有板块的信息。    
g_groupcontainer：AfxGetGroupContainer()     可以得到该变量的引用，该    
                                                       变量记录所有分组的信息。    
g_stdatabase：AfxGetDB()        可以得到该变量的引用，该变量实现了本软件的   
                                                          数据文件接口。    
_stprofile：AfxGetProfile()          可以得到该变量的引用，该变量记录当前软   
                                                          件的一些设置。    
                           
  ★★3.工程StkNet介绍    
     这个工程是实现网络数据自动下载功能的动态连接库。其中比较重要的几个文件如下：    
     NetDB.h                              一些数据下载，安装的接口   
     HttpClt.h                             HTTP协议客户端类   
    InfoZip.h                              压缩/解压缩Zip文件    
     Markup.h                            XML解析   
     XMLDoc.h                           XML辅助    
    secret.h                            主要包括一些注册机的加密算法和读取硬盘序列号的类。   
     NetDB.cpp                          一些数据下载，安装的接口实现   
     HttpClt.cpp                          HTTP协议客户端类实现   
     InfoZip.cpp                          压缩/解压缩    
     Markup.cpp                         XML解析实现    
     Secret.cpp                          加密文件，读取硬盘系列号等    
     XMLDoc.cpp                        XML辅助实现    
    secret.cpp              主要包括一些注册机的加密算法和读取硬盘序列号的类实    现。     其中工程中的Tongshi       Files为支持通视卡行情的实现程序    
   其中工程中的Yongxin       Files为支持清华永新卡行情的实现程序   
     其中工程中的Receiver       Files为本软件的行情接收的实现程序，其中将通视卡和清 华永新卡的数据结构转换成本软件内的通用数据结构，报价（REPORT）、分时（MINUTE）、额外（OUTLINE）、K线（KDATA）、除权信息（DRDATA）     程StkNet的全局静态变量：    
 g_sview：AfxGetSView()可以得到该变量的引用，该变量记录注册信息。    
   
   
 ★★ 4.工程StkUI介绍   

该工程生成最终可执行文件，主要是软件的界面。主要几个文件如下：   
BaseView.cpp                       基本资料视图    
ChildFrm.cpp                       子窗口框架    
MainFrm.cpp                           主框架，响应菜单命令    
StkUI.cpp                                   应用程序类，主程序初始化，初始数据读取等    
StaticDoc.cpp                   文档类   
WizardView.cpp               向导视图   
SimuView.cpp                       策略视图    
MarketView.cpp               实时行情视图   
SListView.cpp                   股票列表视图   
StockGraph.cpp               技术指标视图画图类   
 DrawTech.cpp                       画具体技术指标的函数，属于CGraphView类的方法    
 GraphView.cpp                   技术指标视图   
 StrategyView.cpp                           策略树视图    
 GroupView.cpp                   股票分组树视图   
 TechsView.cpp                   技术指标树视图

 

★★ 5. NetTS

主要涉及策略为王的网络模块,包括服务器的连接,服务器代理的设置
 主要信息的保存,数据包的发送,接受,来不及处理的数据包的缓存.

主要类介绍如下:
     CTSCache  数据包的分割,提取,缓存到文件,文件中数据包的恢复
     CQSProfile  关于QuoteServer和Proxy的系统设置的保存和恢复
     CProxySetDlg 代理设置对话框

 WinTrace  痕迹追踪对象

 CSPTime   基本数据时间的操作
    CProxySocket 代理连接的处理,包括HTTP代理,Socks4,Socks5等情况
    tw.h文件中的函数主要涉及到一些数据的基本结构
    CTSWnd   句柄管理及消息的存储和分发
