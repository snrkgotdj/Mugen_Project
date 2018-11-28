#pragma once

//#define SERVER_IP "127.0.0.1"
#define SERVER_IP "172.30.1.2"
#define PORT 2018

enum PACKET_HEADER
{
	PH_JOIN,
	PH_LOGIN,
	PH_KEY_INPUT,
	PH_KEY_OUTPUT,
	PH_POSITION,
	PH_END
};