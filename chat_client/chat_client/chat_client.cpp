// chat_client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <thread>
#include <memory>

#include <boost/asio.hpp>
#include <boost/array.hpp>

#include <atomic>

#include "message_handler.h"

using namespace boost::asio;

//static const std::string ipAddr = "127.0.0.1";
static const std::string ipAddr = "10.30.1.37";
//static const std::string ipAddr = "10.30.1.18";


class client
{
public:
   client(unsigned port_number)
   {
      start(port_number);
   }

   ~client()
   {
      stop();
   }

   void start(unsigned port_number)
   {
      if (started_) return;

      started_ = true;
      listener_thread_ = std::thread([this, port_number]()
      {
         try
         {
            ip::tcp::endpoint endpoint(ip::address::from_string(ipAddr), port_number);
            ip::tcp::socket socket(io_service_);
            socket.connect(endpoint);
            messanger_ = std::make_unique<message_handler>(std::move(socket));
            receive();
            io_service_.run();
         }
         catch (std::exception& e)
         {
            std::cerr << "Exception occured in listener thread: " << e.what() << std::endl;
            stop();
         }
      });
   }

   void write(const std::string& message)
   {
      io_service_.post([this, message](){do_send(message); });
   }

   void stop()
   {
      if (!started_) return;

      started_ = false;
      io_service_.stop();
      listener_thread_.join();
   }

private:

   void receive()
   {
      messanger_->async_receive_message([this](bool result, const std::string& message)
      {
         if (result)
         {
            std::cout <<"anonymous: " << message << "\n";
            receive();
         }
         else
         {
            std::cout << "error\n";
            stop();
         }
      });
   }

   void do_send(const std::string& message)
   {
      messanger_->async_send_message(message, [this](bool result)
      {
         if (!result)
         {
            std::cout << "error\n";
            stop();
         }
      });
   }

   std::atomic<bool> started_ = false;
   io_service io_service_;
   std::unique_ptr<message_handler> messanger_;
   std::thread listener_thread_;
};

int _tmain(int argc, _TCHAR* argv[])
{
   const int requests = 5;
   const int portNumber = 18;
   //requestTime(portNumber);

   client client(portNumber);
   for (int i = 0; i < 5; i++)
   {
      std::string str;
      std::getline(std::cin, str);
      client.write(str);
   }

	return 0;
}

