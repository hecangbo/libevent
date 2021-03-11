#include "libEventInit.h"

#include "../third/log4z.h"
#include "event2/event.h"
#include <event2/listener.h>
#ifndef _WIN32
#include <signal.h>
#endif
#include <string.h>


#define SPORT 50001
void listen_cb(struct evconnlistener *e, evutil_socket_t s, struct sockaddr *a, int socklen, void *agr)
{
	LOGI("listen_cb....");
}

libEventInit::libEventInit(void)
{

}
libEventInit::~libEventInit(void)
{

}

void libEventInit::init()
{
	LOGI("Libevent Start....");
#ifdef _WIN32
	//初始化socket库
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
#else
	//忽略管道信号，发送数据给已关闭的socket
	if (signal(SIGPIPE, SIG_IGN, SIG_ERR));
#endif

	//创建libevent的上下文件
	event_base * base = event_base_new();
	if (base)
	{
		LOGI("event base new sucess!");
	}
	//监听端口
	//socket,bind,listen，绑定事件
	sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(SPORT);
	evconnlistener *ev = evconnlistener_new_bind(base,	//libevent的上下文
		listen_cb,					//接收到连接的回调函数
		base,						//回调函数获取的参数arg
		LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,		//地址重用，listen关闭同时关闭socket
		10,							//连接队列大小，对应liten函数
		(sockaddr*)&sin,							//绑定的地址和端口
		sizeof(sin)
	);

	//事件分发处理
	if (base)
	{
		event_base_dispatch(base);
	}

	evconnlistener_free(ev);
	if (base)
	{
		event_base_free(base);
	}
#ifdef _WIN32
	WSACleanup();
#endif
}