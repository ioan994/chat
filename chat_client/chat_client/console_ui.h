#pragma once
#include "ui.h"
#include "user_input.h"


class console_ui : public ui
{
public:
   console_ui();
   virtual ~console_ui();

   void start_listening_keystrokes();
   void clear_previous_chars(unsigned count);
   void on_user_edit_message(const std::string& message_before, const std::string& user_edited_message);

   virtual void on_message(const std::string& message) override;
   virtual void on_error(const std::string& message) override;
   virtual void on_user_send_message(const std::string& user_sent_message) override;

private:

   void output_without_interrupting_user_input(const std::string& message);
   void rewind_cursor_position_by(unsigned positions);
   void print_spaces(unsigned count);
   void print_n_chars(char ch, unsigned count);

private:

   user_input user_input_;
};

