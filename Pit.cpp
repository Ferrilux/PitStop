#include "ncurses.h"
#include "Wheel.cpp"
#include "Car.cpp"

class Pit
{
private:
    Car cars[20];

    int main(void)
    {
        init();
        race();
    }

    void init()
    {
        initscr();
        
    }

    void race(){

    }
};

// int main(void)
// {
//     initscr();
//     printw("Hello World");
//     refresh();
//     getch();
//     endwin();
//     return 0;
// }