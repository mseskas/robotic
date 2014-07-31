#include <iostream>
#include <string>


#include "activities/activities.h"

#include "gui.h"


using namespace std;


int main()
{

    activities main_act;

    gui main_gui(main_act);


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
