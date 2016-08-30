// chat_client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <thread>
#include <memory>
#include <conio.h>

#include <boost/asio.hpp>
#include <boost/array.hpp>

#include "client.h"
#include "console_ui.h"


using namespace boost::asio;

//static const std::string ipAddr = "127.0.0.1";
static const std::string ipAddr = "10.30.1.37";
//static const std::string ipAddr = "10.30.1.18";

static const int portNumber = 18;

int _tmain(int argc, _TCHAR* argv[])
{
   const int requests = 5;
   
   //requestTime(portNumber);

   console_ui cli;
   client client(cli, ip::tcp::endpoint(ip::address::from_string(ipAddr), portNumber));
   cli.set_chat_client(&client);
   cli.start_listening_keystrokes();

	return 0;
}

