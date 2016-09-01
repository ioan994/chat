#include "stdafx.h"

#include <conio.h>
#include <iostream>



#include "client.h"
#include "console_ui.h"

console_ui::console_ui() : user_input_{ *this }
{
}

console_ui::~console_ui()
{
}

void console_ui::start_listening_keystrokes()
{
   while (true)
      user_input_.char_entered(_getch());
}

void console_ui::on_message(const std::string& username, const std::string& message) const
{
   std::unique_lock<std::mutex> lock(console_mutex_);
   output_without_interrupting_user_input(username + ": " + message + "\n");
}

void console_ui::on_error(const std::string& message) const
{
   std::unique_lock<std::mutex> lock(console_mutex_);
   output_without_interrupting_user_input(message + "\n");
}

void console_ui::on_user_send_message(const std::string& user_sent_message) const
{
   {
      std::unique_lock<std::mutex> lock(console_mutex_);
      clear_previous_chars(user_input_.get_input().size());
      std::cout << "you: " << user_sent_message << "\n";
   }

   if (client_)
      client_->send(user_sent_message);
}

void console_ui::on_user_edit_message(const std::string& message_before, const std::string& user_edited_message) const
{
   std::unique_lock<std::mutex> lock(console_mutex_);
   clear_previous_chars(message_before.size());
   std::cout << user_edited_message;
}

void console_ui::output_without_interrupting_user_input(const std::string& message) const
{
   clear_previous_chars(user_input_.get_input().size());
   std::cout << message;
   std::cout << user_input_.get_input();
}

void console_ui::clear_previous_chars(unsigned count) const
{
   rewind_cursor_position_by(count);
   print_spaces(count);
   rewind_cursor_position_by(count);
}

void console_ui::rewind_cursor_position_by(unsigned positions) const
{
   print_n_chars('\b', positions);
}

void console_ui::print_spaces(unsigned count) const
{
   print_n_chars(' ', count);
}

void console_ui::print_n_chars(char ch, unsigned count) const
{
   while (count-- > 0)
   {
      std::cout << ch;
   }
}