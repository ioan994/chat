#pragma once

#include <string>

#include <boost/asio.hpp>

namespace
{
   int decode_int(const std::vector<char>& data)
   {
      return *((int*)data.data());
   }

   std::string encode_int(int val)
   {
      char* encoded = (char*)&val;
      return encoded;
   }
}

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

   void async_send_message(const std::string& message)
   {
      message_to_send_ = message;
      send_header();
   }

private:

   void send_header()
   {
      boost::asio::async_write(socket_,
         boost::asio::buffer(encode_int(message_to_send_.size())),
         [this](boost::system::error_code ec, std::size_t /*length*/)
      {
         if (!ec)
         {
            send_body();
         }
      });
   }

   void send_body()
   {
      boost::asio::async_write(socket_,
         boost::asio::buffer(message_to_send_), [](boost::system::error_code, std::size_t /*length*/) {});
   }

   template<class MessageCallback>
   void receive_header(const MessageCallback& callback)
   {
      meesage_to_receive_header_.resize(sizeof(int));
      boost::asio::async_read(socket_,
         boost::asio::buffer(meesage_to_receive_header_),
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
      message_to_receive_body_.resize(decode_int(meesage_to_receive_header_));
      boost::asio::async_read(socket_,
         boost::asio::buffer(message_to_receive_body_), [this, callback](boost::system::error_code ec, std::size_t /*length*/)
      {
         if (!ec)
         {
            message_to_receive_body_.push_back('\0');
            callback(true, std::string(message_to_receive_body_.data()));
         }
         else
         {
            callback(false, "");
         }
      });
   }

   boost::asio::ip::tcp::socket socket_;
   std::string message_to_send_;
   std::vector<char> message_to_receive_body_;
   std::vector<char> meesage_to_receive_header_;
};
