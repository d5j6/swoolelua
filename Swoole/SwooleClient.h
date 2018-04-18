#ifndef AT_APP_SWOOLE_CLIENT_H
#define AT_APP_SWOOLE_CLIENT_H

#include <swoole/swoole.h>
#include <swoole/Client.h>
#include <string>
#include <mutex>
#include <vector>

#include "../../CommonLib/RefCountPtr.h"

using std::string;
using std::vector;
using std::mutex;

class SwooleClient: public RefCountedBase
{
public:
	SwooleClient();
	virtual ~SwooleClient();

	REF_IMPLEMENT_COUNTING(LoginSession)

public:
	
	//static SwooleClient* GetSingleton();

	bool SwConnect(char* host,int port);

	int SendData(char* data,uint32_t len,string& recv);

	int SendData(char* data,uint32_t len);
	int RecvData(char* data,uint32_t len);

	void SwClose();

	void SetConnected(bool connected);
	bool GetConnected();

	void SetInUse(bool inuse);
	bool GetInUse();

	bool IsConnected();

	// void onConnect();
 //    void onError();
 //    void onReceive(char *data, uint32_t length);
 //    void onClose();
 //    void onBufferFull();
 //    void onBufferEmpty();
private:
	swClient m_cli;
	bool m_connect;	//是否连接成功
	bool m_use;		//连接是否使用中
	//static SwooleClient* m_swClientSingle;
};

class SwooleClientMgr
{
public:
	SwooleClientMgr();
	~SwooleClientMgr();

	static SwooleClientMgr* GetSingleton();

	bool CreateSwClient(int num);

	RefCountedPtr<SwooleClient> GetSwooleClient();
	
	bool RemoveSwClient(SwooleClient* sc);

	void ReleaseSwMgr();
private:
	static SwooleClientMgr* m_swClientSingle;
	vector<RefCountedPtr<SwooleClient>> m_swClients;
	mutex m_swmgrMutex;
};
#endif