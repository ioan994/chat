#include "stdafx.h"

#include <string>
#include <iostream>
#include <memory>

#include <boost/asio.hpp>

#include "chat_participant.h"
#include "message_handler.h"

chat_participant::chat_participant(chat& chat_room, boost::asio::ip::tcp::socket socket)
   : chatRoom_{ chat_room }
   , messanger_ { std::move(socket) }
{
}

void chat_participant::start_reading_messages()
{
   read_next_message();
}

void chat_participant::send_message(const std::string& message)
{
   auto& chatRoom = chatRoom_;
   messanger_.async_send_message(message, [this, &chatRoom](bool result)
   {
      if (!result)
      {
         chatRoom.remove_participant(*this);
      }
   });
}

void chat_participant::read_next_message()
{
   auto& chatRoom = chatRoom_;
   messanger_.async_receive_message([this, &chatRoom](bool result, const std::string& message)
   {
      if (!result)
      {
         chatRoom.remove_participant(*this);
      }
      else
      {
         chatRoom.message_received(message,*this);
         read_next_message();
      }
   });
}
