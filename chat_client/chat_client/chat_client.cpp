// chat_client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>

#include <boost/asio.hpp>
#include <boost/array.hpp>


using namespace boost::asio;

static const std::string ipAddr = "127.0.0.1";
//static const std::string ipAddr = "10.30.1.37";
//static const std::string ipAddr = "10.30.1.18";


void requestTime(unsigned portNumber)
{
   try
   {
      boost::asio::io_service io_service;

      ip::tcp::endpoint endpoint(ip::address::from_string(ipAddr), portNumber);
      ip::tcp::socket socket(io_service);
      socket.connect(endpoint);

      boost::system::error_code error;

      for (int i = 0; i < 5; i++)
      {
         std::string str;
         std::getline(std::cin, str);

         int size = str.size();
         char* h = (char*)(&size);
         size_t len = socket.write_some(boost::asio::buffer(h, sizeof(int)), error);
         size_t len2 = socket.write_some(boost::asio::buffer(str), error);
      }

      if (error == boost::asio::error::eof)
         return;
      else if (error)
         throw boost::system::system_error(error); // Some other error.
   }
   catch (std::exception& e)
   {
      std::cerr << e.what() << std::endl;
   }
}

int _tmain(int argc, _TCHAR* argv[])
{
   const int requests = 5;
   const int portNumber = 18;
   requestTime(portNumber);

	return 0;
}

