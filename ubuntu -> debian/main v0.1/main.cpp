#include <iostream>
#include <string>


#include "activities.h"

using namespace std;


int main()
{

    activities main_act;

    int cmd ;

    //main_act.act(cmd);


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

    main_act.~activities();

    return 0;
}
