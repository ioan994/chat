#include "stdafx.h"

#include <string>
#include <memory>

#include <boost/asio.hpp>
#include <boost/array.hpp>

#include "chat.h"

void chat::add(const std::shared_ptr<boost::asio::ip::tcp::socket>& socket)
{
   connections_.emplace_back(std::make_unique<chat_participant>(*this, socket));
   connections_.back()->read_header();
}

void chat::remove_participant(const chat_participant& participant)
{
   connections_.erase(std::remove_if(begin(connections_), end(connections_),
      [&participant](const std::unique_ptr<chat_participant>& other) { return other.get() == &participant; }),
      end(connections_));
}