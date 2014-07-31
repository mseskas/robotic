
#include "gui.h"


class img_gui
{
    public:
        img_gui() {}

        static update(IplImage * image);
    protected:
    private:
};

static img_gui::update(IplImage * image)
{
    gui.show_image(image);
    //gui::show_image(image);
}
