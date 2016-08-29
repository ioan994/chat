#pragma once

#include <string>

#include <boost/asio.hpp>

class chat_participant;

class chat
{
public:
   void add(boost::asio::ip::tcp::socket socket);
   void remove_participant(const chat_participant& participant);
   void message_received(const std::string& message, const chat_participant& source);

private:
   std::vector<std::unique_ptr<chat_participant>> connections_;
};