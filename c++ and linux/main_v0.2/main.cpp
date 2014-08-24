#include <iostream>
#include <string>

#include "gui_control.h"

#include "activities/activities.h"

#include "gui_main.h"


using namespace std;

int main()
{

    gui_display * display = new gui_display();

    activities * main_act = new activities();

    main_act->set_display_gui(display);

    gui_control * control = new gui_control(main_act);

    gui_main * main_gui = new gui_main(display, control);


    int cmd;
    while(true)
    {

        main_act->print_activities();
        cout << "insert new activity - ";
        cin >> cmd;
        cout << endl;

        if (0 == cmd)  // closes program at 0 or any key than is not a number
            break;

        main_act->act(cmd);
    }

    //main_act.~activities();   - called in every return statement
    return 0;
}
