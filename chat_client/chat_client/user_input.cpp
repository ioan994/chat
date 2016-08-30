#include "stdafx.h"

#include <iostream>

#include "user_input.h"
#include "console_ui.h"

user_input::user_input(console_ui& interface) : console_{ interface }
{

}

void user_input::char_entered(char c)
{
   if (c == '\b' && !input_.empty())
   {
      console_.clear_previous_chars(1);
      input_.pop_back();
   }
   else if (c == '\r')
   {
      console_.on_user_send_message(input_);
      input_.clear();
   }
   else
   {
      std::string prev_input = input_;
      input_ += c;
      console_.on_user_edit_message(prev_input, input_);
   }
}

const std::string& user_input::get_input()
{
   return input_;
}