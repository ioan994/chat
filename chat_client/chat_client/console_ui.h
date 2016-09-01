#pragma once

#include <mutex>

#include "ui.h"
#include "user_input.h"

class console_ui : public ui
{
public:
   console_ui();
   virtual ~console_ui();

   void start_listening_keystrokes();
   void clear_previous_chars(unsigned count) const;
   void on_user_edit_message(const std::string& message_before, const std::string& user_edited_message) const;

   virtual void on_message(const std::string& username, const std::string& message) const override;
   virtual void on_error(const std::string& message) const override;
   virtual void on_user_send_message(const std::string& user_sent_message) const override;

private:

   void output_without_interrupting_user_input(const std::string& message) const;

   // Character erasure works only within single line. Bug.
   void rewind_cursor_position_by(unsigned positions) const;
   void print_spaces(unsigned count) const;
   void print_n_chars(char ch, unsigned count) const;

private:

   user_input user_input_;
   mutable std::mutex console_mutex_;
};

