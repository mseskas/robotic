#include <iostream>
#include <string>


#include "activities/activities.h"

#include "gui.h"


using namespace std;


int main()
{

    activities main_act;

    gui main_gui(main_act);



   // time_t t = time(0);   // get time now
   // struct tm * now = localtime( & t );

   // string datetime = to_string(now->tm_year + 1900) + "-" + to_string(now->tm_mon + 1) + "-"+ to_string(now->tm_mday)
   //             + " " + to_string(now->tm_hour) + ":" + to_string(now->tm_min)  + ":" + to_string(now->tm_sec);





    int cmd = 10000;

    while(true)
    {

        main_act.print_activities();
        cout << "insert new activity - ";
        cin >> cmd;
        cout << endl;

        if (0 == cmd)  // closes program at 0 or any key than is not a number
            break;

        main_act.act(cmd);
    }

    //main_act.~activities();   - called in every return statement
    return 0;
}
