#include <iostream>
#include <string>


#include "activities/activities.h"

using namespace std;


int main()
{
    activities main_act;

    int cmd ;

    while(true)
    {
        main_act.print_activities();
        cout << "insert new activity - ";
        cin >> cmd;
        cout << endl;

        if (0 == cmd)
            break;

        main_act.act(cmd);
    }

    //main_act.~activities();   - called in every return statement

    return 0;
}
