#include "stdafx.h"

#include "message.h"

void header::set_body_size(unsigned size)
{
   memcpy(buffer_.data(), (void*)&size, buffer_.size());
}

unsigned header::get_body_size()
{
   return *(unsigned*)buffer_.data();
}

std::string body::get_body_string()
{
   std::string body{ buffer_.begin(), buffer_.end() };
   return body;
}

void body::set_body_string(const std::string& string)
{
   buffer_.resize(string.size()+1);
   std::copy(string.begin(), string.end(), buffer_.begin());
   buffer_[buffer_.size() - 1] = '\0';
}

message::message()
{
}

message::message(const std::string& message)
{
   body_.set_body_string(message);
   header_.set_body_size(body_.buffer_.size());
}

header::header_buffer& message::get_header_buffer()
{
   return header_.buffer_;
}

body::body_buffer& message::get_body_buffer()
{
   return body_.buffer_;
}

void message::prepare_body_for_reading()
{
   body_.buffer_.resize(header_.get_body_size());
}

std::string message::get_body_string()
{
   return body_.get_body_string();
}