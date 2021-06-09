#include "ncurses.h"
#include "Car.cpp"

class Pit
{
private:
    Car* cars[20];

public:
    Pit(){
        for(int i = 0; i < 20; i++){
            cars[i] = new Car(i);
        }
    }

    void init()
    {
        initscr();
        noecho(); // no echo while we getch
        if (has_colors())
        {
            start_color();
            init_pair(1, COLOR_WHITE, COLOR_BLACK);
            init_pair(2, COLOR_RED, COLOR_RED);
            init_pair(3, COLOR_GREEN, COLOR_GREEN);
        }
        printPit();
        getch();
        end();
    }

    void race()
    {
    }

    void end()
    {
        endwin();
    }

    void printPit()
    {
        WINDOW *localWin[10];
        int row, col;
        getmaxyx(stdscr, row, col);

        for (int i = 0; i < 10; i++)
        {
            localWin[i] = newwin(5, 3, 3, 2 * i);
            box(localWin[i], 0, 0);
            wrefresh(localWin[i]);
        }
    }
};

int main(void)
{
    Pit *pitStop;
    pitStop->init();
    pitStop->race();

    return 0;
}
