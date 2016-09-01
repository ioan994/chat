#include "stdafx.h"

#include <string>
#include <memory>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/array.hpp>

#include "chat.h"
#include "chat_participant.h"

#include "messages/generated/string_message.pb.h"

void chat::add(boost::asio::ip::tcp::socket socket)
{
   connections_.emplace_back(std::make_unique<chat_participant>(*this, std::move(socket)));
   connections_.back()->start_reading_messages();
}

void chat::remove_participant(const chat_participant& participant)
{
   connections_.erase(std::remove_if(begin(connections_), end(connections_),
      [&participant](const std::unique_ptr<chat_participant>& other) { return other.get() == &participant; }),
      end(connections_));
}

void chat::message_received(const std::string& message, const chat_participant& source)
{
   StringMessage parsed_message;
   parsed_message.ParseFromString(message);
   std::cout << "message received: " << &source << " " << parsed_message.username() << ": " << parsed_message.message() << "\n";
   for (auto& participant : connections_)
   {
      if (&source != participant.get()) participant->send_message(message);
   }
}