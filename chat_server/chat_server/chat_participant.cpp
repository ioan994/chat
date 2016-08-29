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
   auto& chatRoom = chatRoom_;
   messanger_.async_receive_message([this, &chatRoom](bool result, const std::string& message)
   {
      if (!result)
      {
         chatRoom.remove_participant(*this);
      }
      else
      {
         std::cout << message<<'\n';
         start_reading_messages();
      }
   });
}
