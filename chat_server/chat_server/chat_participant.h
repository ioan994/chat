#pragma once

#include <memory>

#include <boost/asio.hpp>

#include "chat.h"
class chat;

class chat_participant
{
public:
   chat_participant(chat& chat_room, const std::shared_ptr<boost::asio::ip::tcp::socket>& socket);

   chat_participant(const chat_participant&) = delete;
   chat_participant& operator = (const chat_participant&) = delete;

   void read_header();
   void read_body();

private:

   int get_message_length();

   chat& chatRoom_;
   std::vector<char> header_;
   std::vector<char> message_;
   boost::asio::ip::tcp::socket socket_;
};