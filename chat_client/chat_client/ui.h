#pragma once

#include <string>


class client;

class ui
{
public:

   ui();
   virtual ~ui(){}

   void set_chat_client(client* cl);

   virtual void on_message(const std::string& message) = 0;
   virtual void on_error(const std::string& message) = 0;
   virtual void on_user_send_message(const std::string& user_sent_message) = 0;

protected:

   client* client_ = nullptr;
};
