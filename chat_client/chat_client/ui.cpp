#include "stdafx.h"

#include <iostream>


#include "ui.h"
#include "client.h"


ui::ui()
{
}

void ui::set_chat_client(client* cl)
{
   client_ = cl;
}
