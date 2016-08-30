#pragma once

#include <string>
#include <thread>

#include <boost/asio.hpp>

class ui;

class client
{
public:
   client(ui& interface, const boost::asio::ip::tcp::endpoint& endpoint);

   ~client();

   void start(const boost::asio::ip::tcp::endpoint& endpoint);

   void send(const std::string& message);
   void stop();

private:

   void receive();
   void do_send(const std::string& message);

   bool started_ = false;
   ui& interface_;
   boost::asio::io_service io_service_;
   std::unique_ptr<class message_handler> messanger_;
   std::thread listener_thread_;
};