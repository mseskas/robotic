#include <iostream>
#include <string>


#include "activities.h"

using namespace std;


int main()
{

    activities main_act;

    string cmd = "dumb drive";

    //main_act.act(cmd);


    while(true)
    {
        if ('c' == cin.get())
            break;
    }
    main_act.~activities();

    return 0;
}
