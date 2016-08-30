#include "stdafx.h"

#include "ui.h"
#include "message_handler.h"

#include "client.h"

using namespace boost::asio;

client::client(ui& interface, const ip::tcp::endpoint& endpoint) : interface_{ interface }
{
   start(endpoint);
}

client::~client()
{
   stop();
}

void client::start(const ip::tcp::endpoint& endpoint)
{
   if (started_) return;

   started_ = true;
   listener_thread_ = std::thread([this, &endpoint]()
   {
      try
      {
         ip::tcp::socket socket(io_service_);
         socket.connect(endpoint);
         messanger_ = std::make_unique<message_handler>(std::move(socket));
         receive();
         io_service_.run();
      }
      catch (std::exception& e)
      {
         interface_.on_error(std::string("Exception occured in listener thread: ") + e.what() + "\n");
      }
   });
}

void client::send(const std::string& message)
{
   io_service_.post([this, message](){do_send(message); });
}

void client::stop()
{
   if (!started_) return;

   started_ = false;
   io_service_.stop();
   listener_thread_.join();
}


void client::receive()
{
   messanger_->async_receive_message([this](bool result, const std::string& message)
   {
      if (result)
      {
         interface_.on_message(message);
         receive();
      }
      else
      {
         interface_.on_error("Receive message failed, connection closed.");
      }
   });
}

void client::do_send(const std::string& message)
{
   messanger_->async_send_message(message, [this](bool result)
   {
      if (!result)
      {
         interface_.on_error("Send message failed, connection closed.");
      }
   });
}
