// chat_server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <ctime>
#include <iostream>
#include <string>
#include <memory>
#include <cstdlib>
#include <thread>

#include <boost/asio.hpp>
#include <boost/array.hpp>

#include "chat.h"
#include "chat_participant.h"

using namespace boost::asio;

class server
{
public:
   server(unsigned port_number) : acceptor_(io_service_, ip::tcp::endpoint(ip::tcp::v4(), port_number))
   {
      start();
   }

   ~server()
   {
      stop();
   }

   void start()
   {
      if (started_) return;

      started_ = true;
      listener_thread_ = std::thread([this]()
      {
         try
         {
            accept_next();
            io_service_.run();
         }
         catch (std::exception& e)
         {
            std::cerr << "Exception occured in listener thread: " << e.what()<<std::endl;
         }
      });
   }

   void stop()
   {
      if (!started_) return;

      started_ = false;
      io_service_.stop();
      listener_thread_.join();
   }

private:

   void accept_next()
   {
      auto socketPtr = std::make_shared<ip::tcp::socket>(io_service_);
      acceptor_.async_accept(*socketPtr, [this, socketPtr](const boost::system::error_code& error){ handle_connection(error, socketPtr); });
   }

   void handle_connection(const boost::system::error_code& error, std::shared_ptr<ip::tcp::socket> socket)
   {
      if (!error)
      {
         chat_room_.add(socket);
      }
      accept_next();
   }

   bool started_ = false;
   io_service io_service_;
   ip::tcp::acceptor acceptor_;
   chat chat_room_;
   std::thread listener_thread_;
};


int _tmain(int argc, _TCHAR* argv[])
{
   const unsigned portNumber = 18;
   server s(portNumber);

   std::cin.get();

	return 0;
}

