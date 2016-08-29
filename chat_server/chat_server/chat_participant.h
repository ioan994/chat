#pragma once

#include <memory>

#include <boost/asio.hpp>

#include "chat.h"
#include "message_handler.h"

class chat;

class chat_participant
{
public:
   chat_participant(chat& chat_room, boost::asio::ip::tcp::socket socket);

   chat_participant(const chat_participant&) = delete;
   chat_participant& operator = (const chat_participant&) = delete;

   void start_reading_messages();

private:

   chat& chatRoom_;
   message_handler messanger_;
};