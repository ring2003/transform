#include "lce.h"
#include "CConfigMgr.h"
#include "CProcCenter.h"
using namespace std;
using namespace lce;


int main(int argc,char *argv[])
{
	//初始化配置信息
	if( !CConfigMgr::getInstance().init(argv[0]) )
	{
	    printf("config init:%s\n",CConfigMgr::getInstance().getErrMsg());
		return 0;
	}

	SConfig & stConfig = CConfigMgr::getInstance().getConfig();

	//初始化通信管理
    if(CCommMgr::getInstance().init() < 0)
    {
        LOG_ERROR("comm init:%s\n",CCommMgr::getInstance().getErrMsg());
        return 0;
    }

    stConfig.iTcpSrvId = CCommMgr::getInstance().createSrv(CCommMgr::SRV_TCP,stConfig.sTcpIp,
                                            stConfig.wTcpPort,
                                            stConfig.dwTcpRecvBufSize,
                                            stConfig.dwTcpSendBufSize,
                                            stConfig.dwMaxTcpRecvBufSize,
                                            stConfig.dwMaxTcpSendBufSize);


    if(stConfig.iTcpSrvId < 0)
    {
        LOG_ERROR("create tcp:%s\n",CCommMgr::getInstance().getErrMsg());
        return 0;
    }

	CCommMgr::getInstance().setProcessor(stConfig.iTcpSrvId,&CProcCenter::getInstance(),CCommMgr::PKG_H2LT3);


    stConfig.iHttpSrvId = CCommMgr::getInstance().createSrv(CCommMgr::SRV_TCP,stConfig.sHttpIp,
                                            stConfig.wHttpPort,
                                            stConfig.dwHttpRecvBufSize,
                                            stConfig.dwHttpSendBufSize,
                                            stConfig.dwMaxHttpRecvBufSize,
                                            stConfig.dwMaxHttpSendBufSize);

    if(stConfig.iHttpSrvId < 0)
    {
        LOG_ERROR("create http:%s\n",CCommMgr::getInstance().getErrMsg());
        return 0;
    }

	CCommMgr::getInstance().setProcessor(stConfig.iHttpSrvId,&CProcCenter::getInstance(),CCommMgr::PKG_HTTP);

	//初始化处理中心
	if(CProcCenter::getInstance().init() < 0)
	{
		LOG_ERROR("proccenter init:%s\n",CProcCenter::getInstance().getErrMsg());
		return 0;
	}

	CProcCenter::getInstance().run();//开启处理线程池


    CCommMgr::getInstance().start();

    return 0;
}
