// chat_client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>

#include <boost/asio.hpp>
#include <boost/array.hpp>


using namespace boost::asio;

void requestTime(unsigned portNumber)
{
   try
   {
      boost::asio::io_service io_service;

      ip::tcp::endpoint endpoint(ip::address::from_string("127.0.0.1"), portNumber);
      ip::tcp::socket socket(io_service);
      socket.connect(endpoint);

      boost::array<char, 128> buf;
      boost::system::error_code error;

      size_t len = socket.read_some(boost::asio::buffer(buf), error);

      if (error == boost::asio::error::eof)
         return;
      else if (error)
         throw boost::system::system_error(error); // Some other error.

      std::cout.write(buf.data(), len);
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
   for (int req = requests; req != 0; req--)
   {
      std::cin.get();
      requestTime(portNumber);
   }

	return 0;
}

