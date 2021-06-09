#include "ncurses.h"
#include "Car.cpp"
#include<iostream>
#include<vector>

class Pit
{
private:
    Car* cars[20];
    bool occupied;

public:
    Pit(){
        for(int i = 0; i < 20; i++){
            cars[i] = new Car(i);
        }
        this->occupied = false;

    }

    void init()
    {
        WINDOW * pitWindow;
        WINDOW * info;

        initscr();
        noecho(); // no echo while we getch
        if (has_colors())
        {
            start_color();
            init_pair(1, COLOR_WHITE, COLOR_BLACK);
            init_pair(2, COLOR_RED, COLOR_RED);
            init_pair(3, COLOR_GREEN, COLOR_GREEN);
        }
        printPit(pitWindow, info);
        getch();


        end(); //temp
    }

    void race()
    {
    }

    void end()
    {
        endwin();
    }

    void printPit(WINDOW* pit, WINDOW* info)
    {
        int row, col;
        getmaxyx(stdscr, row, col);
        
        // Okno obsługujące wyświetlanie pit-stopów
        pit = newwin(row-1, (col/2)+10, 0, 1);
        refresh();
        box(pit,0,0);
        mvwprintw(pit, 1, ((col/2)+1)/2, "Pit-Stop");
        wrefresh(pit);

        // Okno informacyjne
        info = newwin(row-1, (col/2)-10, 0, col/2 + 11);
        box(info, 0, 0);
        mvwprintw(info, 1, ((col/2) - 8)/2, "Info");
        wrefresh(info);
        getch();
    }
};

int main(void)
{
    Pit *pitStop;
    pitStop->init();
    pitStop->race();

    return 0;
}