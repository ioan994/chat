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

//static const std::string defaultAddr = "127.0.0.1";
static const std::string defaultAddr = "92.249.111.144";
static const std::string ipAddr = "10.30.1.37";
//static const std::string ipAddr = "10.30.1.18";

static const int portNumber = 18;

int _tmain(int argc, _TCHAR* argv[])
{
   const int requests = 5;
   
   std::string address = argc <= 1 ? defaultAddr : std::string((char*)(argv[1]));

   //requestTime(portNumber);

   try
   {
      console_ui cli;
      client client(cli, ip::tcp::endpoint(ip::address::from_string(address), portNumber));
      cli.set_chat_client(&client);
      cli.start_listening_keystrokes();
   }
   catch (std::exception& e)
   {
      std::cout << "Something went wrong: " << e.what();
   }
   std::cin.get();

	return 0;
}

