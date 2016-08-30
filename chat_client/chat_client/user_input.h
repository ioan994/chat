#pragma once

#include <string>

class console_ui;

class user_input
{
public:
   user_input(console_ui& interface);

   void char_entered(char c);

   const std::string& get_input();

private:

   std::string input_;
   console_ui& console_;
};