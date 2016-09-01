#pragma once

#include <string>
#include <thread>

#include <boost/asio.hpp>

class ui;

class client
{
public:
   client(ui& interface, const std::string& username, const boost::asio::ip::tcp::endpoint& server_endpoint);

   ~client();

   void send(const std::string& message);

private:

   void start(const boost::asio::ip::tcp::endpoint& endpoint);
   void stop();
   void receive();
   void do_send(const std::string& message);

   std::string username_;
   bool started_ = false;
   ui& interface_;
   boost::asio::io_service io_service_;
   std::unique_ptr<class message_handler> messanger_;
   std::thread listener_thread_;
};