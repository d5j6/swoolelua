#include "SwooleAsyncClient.h"
#include <swoole/Base.hpp>

namespace swoole
{
	void onConnect(swClient *cli)
	{

		SwooleAsyncClient* swAsyncClient = (SwooleAsyncClient*) cli->object;
		swAsyncClient->OnConnect();
		printf("onConnect\n");
	}

	void onError(swClient *cli)
	{
		printf("onError:%d\n",SwooleG.error);
	}

	void onReceive(swClient *cli, char *data, uint32_t length)
	{
		printf("onReceive:%s,%d\n", data, length);
		cli->close(cli);
	}

	void onClose(swClient *cli)
	{
		printf("onClose\n");
	}
	void onBufferFull(swClient *cli)
	{
		printf("onBufferFull\n");
	}

	void onBufferEmpty(swClient *cli)
	{
		printf("onBufferEmpty\n");
	}

	/*
	 **********************************************************
	 */
	SwooleAsyncClient::SwooleAsyncClient() : m_swCli(nullptr)
	{

	}
	SwooleAsyncClient::~SwooleAsyncClient()
	{
		ReleaseClient();
	}

	bool SwooleAsyncClient::Init(const char* svIp,int svPort,int timeout)
	{
		int ret = 0;
		//检测全局参数是否初始化
		swoole::check_reactor();
		//创建客户端
		m_swCli  = (swClient*)malloc(sizeof(swClient)); 
		ret = swClient_create(m_swCli, SW_SOCK_TCP, SW_SOCK_ASYNC);
		if (ret < 0)
		{
			printf("swClient_create.\n");
			return false;
		}
		//客户端回调参数
		m_swCli->object = (void*)this;
		//客户端回调函数
		m_swCli->onConnect = onConnect;
		m_swCli->onError = onError;
		m_swCli->onReceive = onReceive;
		m_swCli->onClose = onClose;
		m_swCli->onBufferFull = onBufferFull;
		m_swCli->onBufferEmpty = onBufferEmpty;

		m_swCli->reactor_fdtype = SW_FD_STREAM_CLIENT;
		// int socket_fd = cli.socket->fd;
		// swSetNonBlock(socket_fd);
		// SwooleG.main_reactor->add(SwooleG.main_reactor, socket_fd, SW_FD_USER | SW_EVENT_WRITE | SW_EVENT_READ | SW_EVENT_ERROR);
		
		printf("swClient_create1.\n");
		//timeout超时时间(秒)   最后一个参数为1(异步模式)
		//ret = m_swCli->connect(m_swCli, "127.0.0.1", 9501, 1, 1);
		ret = m_swCli->connect(m_swCli, (char*) svIp, svPort, timeout, 1);
		if (ret < 0)
		{
			printf("connect fail.\n");
			return false;
		}
		swoole::event_wait();
		return true;
	}

	void SwooleAsyncClient::ReleaseClient()
	{
		if(m_swCli)
		{
			free(m_swCli);
			m_swCli = nullptr;
		}		
	}

	void SwooleAsyncClient::OnConnect()
	{
		printf("onConnect 1\n");

		// int ret = m_swCli->send(m_swCli, SW_STRL("TCP: hello world"), 1);
		// printf("send ret:%d\n",ret);
		// printf("onConnect end\n");
	}

} //namespace swoole


//Test
/*
int TestSwooleAsyncClient()
{
	swoole::SwooleAsyncClient swAsyncClient;
	swAsyncClient.Init("127.0.0.1", 9501, 1);
	sleep(5);
	printf("End\n");
	return 0;
}
*/