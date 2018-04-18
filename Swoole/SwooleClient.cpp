#include "../../SourceCode/Public.h"
#include "SwooleClient.h"
//SwooleClient* SwooleClient::m_swClientSingle = nullptr;

SwooleClient::SwooleClient()
	: m_connect(false)
	, m_use(false)
{

}

SwooleClient::~SwooleClient()
{
	SwClose();
}

// SwooleClient* SwooleClient::GetSingleton()
// {
// 	if(nullptr == m_swClientSingle)
// 		m_swClientSingle = new SwooleClient();
// 	return m_swClientSingle;
// }

bool SwooleClient::SwConnect(char* host,int port)
{
	int ret = 0;
	//SW_SOCK_UDP
	ret = swClient_create(&m_cli, SW_SOCK_TCP, SW_SOCK_SYNC);
	if (ret < 0)
	{
		return false;
	}
	m_cli.protocol.package_length_type = 'N';
	m_cli.protocol.package_length_size = 4;
	m_cli.protocol.package_length_offset = 0;
	ret = m_cli.connect(&m_cli, host, port, 20, 0);
	if (ret < 0)
	{
		return false;
	}
	SetConnected(true);
	return true;
}

int SwooleClient::SendData(char* data,uint32_t len,string& recv)
{
	int ret = 0;
	//客户端是否连接
	if(!GetConnected())
		return -1;
	//发送数据
	ret = SendData(data,len);
	if(ret <= 0)
	{
		return -2;
	}
	//接收读取数据大小
	char* buff = (char*)malloc(SW_BUFFER_SIZE_STD);
	ret = m_cli.recv(&m_cli, buff, 4, MSG_WAITALL);
	if(ret <= 0)
	{
		free(buff);
		return -3;
	}
	//解析数据长度
	uint32_t datalen = ntohl(*((uint32_t *) buff));
	if(datalen == 4)
	{
		//返回数据为空
		free(buff);
		return 0;
	}else if(datalen > 2*1024*1024)
	{
		//数据长度过大
		free(buff);
		return -4;
	}else if(datalen > SW_BUFFER_SIZE_STD)
	{
		//重新分配接收内存
		buff = (char*)realloc(buff,datalen+1);
	}

	//阻塞接收数据内容
	ret = m_cli.recv(&m_cli, buff, datalen, MSG_WAITALL);
	if(ret <= 0)
	{
		free(buff);
		return -5;
	}
	buff[datalen] = 0;
	recv = buff;
	return ret;
}

int SwooleClient::SendData(char* data,uint32_t len)
{
	int ret = 0;
	if(!GetConnected())
		return -1;

	//len += 4;
	uint32_t lenth = htonl(len);

	ret = m_cli.send(&m_cli, (char*)&lenth, 4, 0);

	ret = m_cli.send(&m_cli, data, len, 0);
	if (ret < 0)
	{
		return -2;
	}
	return ret;
}

int SwooleClient::RecvData(char* data,uint32_t len)
{
	int ret = 0;
	if(!GetConnected())
		return -1;

	ret = m_cli.recv(&m_cli, data, len, 0);
	if (ret < 0)
	{
		return -3;
	}
	return ret;
}

void SwooleClient::SwClose()
{
	m_cli.close(&m_cli);
	swClient_free(&m_cli);
}

void SwooleClient::SetConnected(bool connected)
{
	m_connect = connected;
}

bool SwooleClient::GetConnected()
{
	return m_connect;
}

void SwooleClient::SetInUse(bool inuse)
{
	m_use = inuse;
}

bool SwooleClient::GetInUse()
{
	return m_use;
}

bool SwooleClient::IsConnected()
{
	if (!m_cli.socket)
    {
        return false;
    }
    return m_cli.socket->active;
}


// void SwooleClient::onConnect()
// {
// 	printf("onConnect\n");
// }

// void SwooleClient::onError()
// {
// 	printf("onError\n");
// }

// void SwooleClient::onReceive(char *data, uint32_t length)
// {
// 	printf("onReceive:%s,%d\n", data, length);
// }

// void SwooleClient::onClose()
// {
// 	printf("onClose\n");
// }
// void SwooleClient::onBufferFull()
// {
// 	printf("onBufferFull\n");
// }

// void SwooleClient::onBufferEmpty()
// {
// 	printf("onBufferEmpty\n");
// }









////////////////////////////////////////////////////////////////////////////////////////////////
SwooleClientMgr* SwooleClientMgr::m_swClientSingle = nullptr;

SwooleClientMgr::SwooleClientMgr()
{

}

SwooleClientMgr::~SwooleClientMgr()
{

}

SwooleClientMgr* SwooleClientMgr::GetSingleton()
{
	if(m_swClientSingle == nullptr)
		m_swClientSingle = new SwooleClientMgr();
	return m_swClientSingle;
}

bool SwooleClientMgr::CreateSwClient(int num)
{
	m_swmgrMutex.lock();
	for(int index = 0; index < num; index++)
	{
		RefCountedPtr<SwooleClient> sc = new SwooleClient();
		m_swClients.push_back(sc);
	}
	m_swmgrMutex.unlock();
	return true;
}

RefCountedPtr<SwooleClient> SwooleClientMgr::GetSwooleClient()
{
	std::lock_guard<std::mutex> lg(m_swmgrMutex);
	
	for(auto iter = m_swClients.begin(); iter != m_swClients.end(); iter++)
	{
		if((*iter)->GetInUse())
			continue;
		(*iter)->SetInUse(true);
		return (*iter);
	}
	return nullptr;
}

bool SwooleClientMgr::RemoveSwClient(SwooleClient* sc)
{
	bool bRet = false;
	m_swmgrMutex.lock();
	for(auto iter = m_swClients.begin(); iter != m_swClients.end(); iter++)
	{
		if((*iter) == sc)
		{
			bRet = true;
			m_swClients.erase(iter);
			break;
		}
	}
	m_swmgrMutex.unlock();
	return bRet;
}

void SwooleClientMgr::ReleaseSwMgr()
{
	m_swmgrMutex.lock();
	m_swClients.clear();
	m_swmgrMutex.unlock();

	delete m_swClientSingle;
	m_swClientSingle = nullptr;
}