#include "ncurses.h"
#include "Car.cpp"
#include<iostream>
#include<vector>
#include<mutex>
#include<thread>
#include<random>
#include<atomic>

class Pit
{
private:
    // Car* cars[20];
    std::vector<Car*> cars;
    std::atomic<bool> isOver;
    std::atomic<bool> crash;
    bool occupied;
    // WINDOW *pitWindow;
    // WINDOW *infoWindow;

public:
    Pit(){
        for(int i = 0; i < 20; i++){
            Car* newCar = new Car(i);
            cars.push_back(newCar);
        }
        this->occupied = false;
    }

    void init()
    {
        WINDOW *pitWindow;
        WINDOW *infoWindow;
        int row, col;

        initscr();
        noecho(); // no echo while we getch
        if (has_colors())
        {
            start_color();
            init_pair(1, COLOR_WHITE, COLOR_BLACK);
            init_pair(2, COLOR_RED, COLOR_RED);
            init_pair(3, COLOR_GREEN, COLOR_GREEN);
        }
        getmaxyx(stdscr, row, col);
        pitWindow = printPit(pitWindow, row, col);
        infoWindow = printInfo(infoWindow, row, col);
        getch();

        race(pitWindow, infoWindow);
        end(); //temp
    }

    void race(WINDOW* pitWindow, WINDOW* infoWindow)
    {
        //std::thread observe(&Pit::checkQuit, this);

    }

    void end()
    {
        endwin();
    }
    
    // Okno obsługujące wyświetlanie pit-stopów
    WINDOW* printPit(WINDOW* pit, int row, int col)
    {       

        pit = newwin(row-1, (col/2)+10, 0, 1);
        refresh();
        box(pit,0,0);
        mvwprintw(pit, 1, ((col/2)+1)/2, "Pit-Stop");
        wrefresh(pit);

        return pit;
    }
    
    // Okno informacyjne
    WINDOW* printInfo(WINDOW* info, int row, int col){

        info = newwin(row-1, (col/2)-10, 0, col/2 + 11);
        box(info, 0, 0);
        mvwprintw(info, 1, ((col/2) - 8)/2, "Info");
        mvwprintw(info, 3, 5, "How to operate this program: q - quit, c - initiate crash");

        for(int i = 0; i < 20; i++){
            mvwprintw(info, (i + 2) + 5, col/15, "Car %d - ", i+1);
            mvwprintw(info, (i + 2) + 5, col/15 + 10, cars[i]->getStatus());
        }
        wrefresh(info);

        return info;
    }

    void checkInput(){
        char sign = ' ';
        while (!this->isOver) {
            sign = getchar();
            if (sign == 'q') {
                this->isOver = true;
                // TODO - end all threads
                // for (auto& t : threads) {
                //     t.join();
                // }
            }
            if(sign == 'c'){
                this->crash = true;
            }
	    }
    }

    int randomTime(int time) {
        time = time * 1000;
        std::mt19937 mt(std::random_device{}());
        std::uniform_real_distribution<double> num(0.8 * time, 1.2 * time);

        return num(mt);
    }

    void sleepFor(int time) {
        std::chrono::milliseconds duration(time);
        std::this_thread::sleep_for(duration);
    }
};



int main(void)
{
    Pit *pitStop = new Pit();
    pitStop->init();

    return 0;
}