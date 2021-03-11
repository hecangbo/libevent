#include "libEventConf.h"
#include "../third/log4z.h"
#include "event2/event.h"
#include <string.h>
#include <iostream>
#ifndef _WIN32
#include <signal.h>
#endif
using namespace std;
libEventConf::libEventConf(void)
{

}
libEventConf::~libEventConf()
{

}
void libEventConf::init()
{
#ifdef _WIN32
	//��ʼ��socket��
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
#else
	//���Թܵ��źţ��������ݸ��ѹرյ�socket
	if (signal(SIGPIPE, SIG_IGN, SIG_ERR));
#endif
	//��������������
	event_config *conf = event_config_new();
	//��ʾ֧�ֵ�����ģʽ
	const char **methods = event_get_supported_methods();
	LOGI("supported_methods:");
	for (int i = 0; methods[i] != NULL; ++i)
	{
		LOGD(methods[i]);
	}

	//��������
	//������EV_FEATURE_FDS�������������޷�����,��windows��EV_FEATURE_FDS��Ч
	//event_config_require_features(conf, EV_FEATURE_ET| EV_FEATURE_O1| EV_FEATURE_FDS| EV_FEATURE_EARLY_CLOSE);
	//event_config_require_features(conf,  EV_FEATURE_FDS);	//����ģ�Ͳ�֧��epoll
	
	//��������ģ�ͣ�ʹ��select(����ģ��ѡ��˳��:epoll, poll, select)
	event_config_avoid_method(conf, "epoll");		//��˼�ǲ�ʹ��epoll
	event_config_avoid_method(conf, "poll");		//��˼�ǲ�ʹ��poll

	//��ʼ������libevent������
	event_base * base = event_base_new_with_config(conf);
	if (conf)
	{
		event_config_free(conf);
	}


	if (!base)
	{
		LOGI("event base new with with config failed!");
		base = event_base_new();
		if (!base)
		{
			cerr << "event_base_new failed!" << endl;
			return;
		}
	}
	else
	{
		//��ȡ��ǰ����ģ��
		cout << "current method is:" << event_base_get_method(base) << endl;

		//ȷ�������Ƿ���Ч
		UINT  num = event_base_get_features(base);
		if (num&EV_FEATURE_ET)
		{
			LOGI("EV_FEATURE_ET events are supported");
		}
		else
		{
			LOGI("EV_FEATURE_ET events are not supported");
		}
		if (num&EV_FEATURE_O1)
		{
			LOGI("EV_FEATURE_ET events are supported");
		}
		else
		{
			LOGI("EV_FEATURE_ET events are not supported");
		}
		if (num&EV_FEATURE_FDS)
		{
			LOGI("EV_FEATURE_ET events are supported");
		}
		else
		{
			LOGI("EV_FEATURE_ET events are not supported");
		}
		if (num&EV_FEATURE_EARLY_CLOSE)
		{
			LOGI("EV_FEATURE_ET events are supported");
		}
		else
		{
			LOGI("EV_FEATURE_ET events are not supported");
		}

		LOGI("event base new with config sucess!");
		event_base_free(base);
	}
}