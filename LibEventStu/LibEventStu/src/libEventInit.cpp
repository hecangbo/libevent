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
	//��ʼ��socket��
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
#else
	//���Թܵ��źţ��������ݸ��ѹرյ�socket
	if (signal(SIGPIPE, SIG_IGN, SIG_ERR));
#endif

	//����libevent�������ļ�
	event_base * base = event_base_new();
	if (base)
	{
		LOGI("event base new sucess!");
	}
	//�����˿�
	//socket,bind,listen�����¼�
	sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(SPORT);
	evconnlistener *ev = evconnlistener_new_bind(base,	//libevent��������
		listen_cb,					//���յ����ӵĻص�����
		base,						//�ص�������ȡ�Ĳ���arg
		LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,		//��ַ���ã�listen�ر�ͬʱ�ر�socket
		10,							//���Ӷ��д�С����Ӧliten����
		(sockaddr*)&sin,							//�󶨵ĵ�ַ�Ͷ˿�
		sizeof(sin)
	);

	//�¼��ַ�����
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