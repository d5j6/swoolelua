#ifndef AT_APP_SWOOLE_ASYNC_CLIENT_H
#define AT_APP_SWOOLE_ASYNC_CLIENT_H

//异步客户端

#include <swoole/swoole.h>
#include <swoole/Client.h>

namespace swoole
{

	// static int php_swoole_event_onRead1(swReactor *reactor, swEvent *event)
	// {
	// 	printf("onread\n");
	// 	return SW_OK;
	// }

	// static int php_swoole_event_onWrite1(swReactor *reactor, swEvent *event)
	// {
	// 	// swClient* cli = (swClient*)event->socket->object;
	// 	// if(cli)
	// 	// {

	// 	// }
	// 	swReactor_onWrite(reactor,event);
	// 	printf("onwrite\n");
	// 	return SW_OK;
	// }

	// static int php_swoole_event_onError1(swReactor *reactor, swEvent *event)
	// {
	// 	printf("onerror\n");
	// 	return SW_OK;
	// }

	void onConnect(swClient *cli);
	void onError(swClient *cli);
	void onReceive(swClient *cli, char *data, uint32_t length);
	void onClose(swClient *cli);
	void onBufferFull(swClient *cli);
	void onBufferEmpty(swClient *cli);

	class SwooleAsyncClient
	{
	public:
		SwooleAsyncClient();
		~SwooleAsyncClient();

		bool Init(const char* svIp,int svPort,int timeout);
		void ReleaseClient();

		void OnConnect();
	private:
		swClient* m_swCli;
	};

}//namespace swoole

#endif //AT_APP_SWOOLE_ASYNC_CLIENT_H