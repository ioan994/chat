#pragma once

#include <string>
#include <array>
#include <vector>

struct header
{
   using header_buffer = std::array < char, sizeof(unsigned) >;
   void set_body_size(unsigned size);
   unsigned get_body_size();

   header_buffer buffer_;
};

struct body
{
   using body_buffer = std::vector < char > ;

   std::string get_body_string();
   void set_body_string(const std::string& string);

   std::vector<char> buffer_;
};

class message
{
public:
   message();
   message(const std::string& message);

   header::header_buffer& get_header_buffer();
   body::body_buffer& get_body_buffer();
   void prepare_body_for_reading();
   std::string get_body_string();

private:
   header header_;
   body body_;
};

