#pragma once

#include <string>
#include <queue>

#include <boost/asio.hpp>

#include "message.h"

class message_handler
{
public:
   message_handler(boost::asio::ip::tcp::socket socket) : socket_{ std::move(socket) }
   {
   }

   template<class MessageCallback>
   void async_receive_message(const MessageCallback& callback)
   {
      receive_header(callback);
   }

   template <class SendMessageCallback>
   void async_send_message(const std::string& msg, const SendMessageCallback& callback)
   {
      messages_to_send_.emplace(msg);
      if (messages_to_send_.size() == 1)
         send_header(callback);
   }

private:

   template <class SendMessageCallback>
   void send_header(const SendMessageCallback& callback)
   {
      boost::asio::async_write(socket_,
         boost::asio::buffer(messages_to_send_.front().get_header_buffer()),
         [this, callback](boost::system::error_code ec, std::size_t /*length*/)
      {
         if (!ec)
         {
            send_body(callback);
         }
         else
         {
            callback(false);
         }
      });
   }

   template <class SendMessageCallback>
   void send_body(const SendMessageCallback& callback)
   {
      boost::asio::async_write(socket_,
         boost::asio::buffer(messages_to_send_.front().get_body_buffer()), [this, callback](boost::system::error_code ec, std::size_t /*length*/)
      {
         callback(!ec);
         messages_to_send_.pop();
         if (!messages_to_send_.empty())
            send_header(callback);
      });
   }

   template<class MessageCallback>
   void receive_header(const MessageCallback& callback)
   {
      boost::asio::async_read(socket_,
         boost::asio::buffer(message_to_receive_.get_header_buffer()),
         [this, callback](boost::system::error_code ec, std::size_t /*length*/)
      {
         if (!ec)
         {
            receive_body(callback);
         }
         else
         {
            callback(false, "");
         }
      });
   }

   template<class MessageCallback>
   void receive_body(const MessageCallback& callback)
   {
      message_to_receive_.prepare_body_for_reading();
      boost::asio::async_read(socket_,
         boost::asio::buffer(message_to_receive_.get_body_buffer()), [this, callback](boost::system::error_code ec, std::size_t /*length*/)
      {
         if (!ec)
         {
            callback(true, message_to_receive_.get_body_string());
         }
         else
         {
            callback(false, "");
         }
      });
   }

private:
   boost::asio::ip::tcp::socket socket_;
   std::queue<message> messages_to_send_;
   message message_to_receive_;
};
