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
	//初始化socket库
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
#else
	//忽略管道信号，发送数据给已关闭的socket
	if (signal(SIGPIPE, SIG_IGN, SIG_ERR));
#endif
	//创建配置上下文
	event_config *conf = event_config_new();
	//显示支持的网络模式
	const char **methods = event_get_supported_methods();
	LOGI("supported_methods:");
	for (int i = 0; methods[i] != NULL; ++i)
	{
		LOGD(methods[i]);
	}

	//设置特征
	//设置了EV_FEATURE_FDS，其它特征就无法设置,在windows中EV_FEATURE_FDS无效
	//event_config_require_features(conf, EV_FEATURE_ET| EV_FEATURE_O1| EV_FEATURE_FDS| EV_FEATURE_EARLY_CLOSE);
	//event_config_require_features(conf,  EV_FEATURE_FDS);	//网络模型不支持epoll
	
	//设置网络模型，使用select(网络模型选择顺序:epoll, poll, select)
	event_config_avoid_method(conf, "epoll");		//意思是不使用epoll
	event_config_avoid_method(conf, "poll");		//意思是不使用poll

	//初始化配置libevent上下文
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
		//获取当前网络模型
		cout << "current method is:" << event_base_get_method(base) << endl;

		//确认特征是否生效
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