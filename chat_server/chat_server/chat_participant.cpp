#pragma once;

#include "stdafx.h"

#include <string>
#include <iostream>
#include <memory>

#include <boost/asio.hpp>

#include "chat_participant.h"

chat_participant::chat_participant(chat& chat_room, const std::shared_ptr<boost::asio::ip::tcp::socket>& socket)
   : chatRoom_{ chat_room }
   , socket_{ std::move(*socket) }
{
}

void chat_participant::read_header()
{
   header_.resize(sizeof(int));
   boost::asio::async_read(socket_,
      boost::asio::buffer(header_),
      [this](boost::system::error_code ec, std::size_t /*length*/)
   {
      if (!ec)
      {
         read_body();
      }
      else
      {
         chatRoom_.remove_participant(*this);
      }
   });
}

void chat_participant::read_body()
{
   int size = get_message_length();
   message_.resize(size);
   boost::asio::async_read(socket_,
      boost::asio::buffer(message_),
      [this](boost::system::error_code ec, std::size_t /*length*/)
   {
      if (!ec)
      {
         std::cout << "message:\n";
         for (char ch : message_)
            std::cout << ch;
         std::cout << "\n\n";
         read_header();
      }
      else
      {
         chatRoom_.remove_participant(*this);
      }
   });
}


int chat_participant::get_message_length()
{
   return *((int*)header_.data());
}