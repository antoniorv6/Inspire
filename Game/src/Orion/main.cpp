#include <iostream>
#include <Kbhit.h>
#include <OrionServer.hpp>
#include <OrionHTTPRequester.hpp>
#include <Clock.hpp>

int main()
{
   bool activeServer = true;
   OrionServer InspireServer(4,2525);
   InspireServer.Init();
   while(activeServer)
   {
       InspireServer.Update();
       if(kbhit())
       {
           char order = getch();
           if(order == 'q')
           {
               activeServer = false;
               std::cout<<std::endl;
               std::cout<<"Server shutdown order received"<<std::endl;
           }
       }
   }
}