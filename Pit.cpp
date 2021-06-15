#include "ncursesw/ncurses.h"
#include "Car.cpp"
#include "locale.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <mutex>
#include <thread>
#include <random>
#include <atomic>

class Pit
{
private:
    std::vector<Car*> cars;
    std::atomic<bool> isOver;
    std::atomic<bool> crash;
    int row, col;
    WINDOW *pitWindow;
    WINDOW *infoWindow;

public:
    Pit(){
        for(int i = 0; i < 20; i++){
            Car* newCar = new Car(i);
            cars.push_back(newCar);
        }
    }

    void init()
    {

        initscr();
        noecho(); // no echo while we getch
        curs_set(0); // no cursor
        setlocale(LC_ALL, ""); // enable utf-8 print
        getmaxyx(stdscr, row, col);
        pitWindow = printPit(pitWindow, row, col);
        infoWindow = printInfo(infoWindow, row, col);

        startRace();
        end(); //temp
    }

    void startRace()
    {
        std::thread observe(&Pit::checkInput, this);
        for (int i = 0; i < 20; i++){
            cars[i]->thread = std::thread(&Pit::race, this, i);
        }
        observe.join();
    }

    void race (int id)
    {
        while(!this->isOver){
            if(this->crash){
                changeTires(id);
            }
            cars[id]->setStatus(onTrack);
            updateInfo(id);
            drive();
            cars[id]->setStatus(waiting);
            updateInfo(id);
            changeTires(id);
            
        }
    };

    void drive(){
        auto timeDriving = randomTime(10);
        sleepFor(timeDriving);
    }

    void changeTires(int carID){

        std::unique_lock<std::mutex> lockFrontLeft(*cars[carID]->wheels[0], std::defer_lock);
        std::unique_lock<std::mutex> lockFrontRight(*cars[carID]->wheels[1], std::defer_lock);
        std::unique_lock<std::mutex> lockRearLeft(*cars[carID]->wheels[2], std::defer_lock);
        std::unique_lock<std::mutex> lockRearRight(*cars[carID]->wheels[3], std::defer_lock);
        
        std::try_lock(lockFrontLeft, lockFrontRight, lockRearLeft, lockRearRight);
        
        updatePit(carID, 0, 3);
        updatePit(carID, 1, 3);
        updatePit(carID, 2, 3);
        updatePit(carID, 3, 3);
        cars[carID]->setStatus(inPit);
        updateInfo(carID);
        
        auto changeTime = randomTime(5);
        sleepFor(changeTime);

        cars[carID]->setStatus(onTrack);
        updateInfo(carID);

        freePit(carID);
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
        mvwprintw(pit, 1, col/4, "Pit-Stop");
        
        for(int i = 0; i < 5; i++){
            mvwprintw(pit, i*9 + 3, col/8, "Team #%d", i + 1);
            mvwprintw(pit, i*9 + 4, col/8 - 1, "O        O");
            mvwprintw(pit, i*9 + 5, col/8 - 1, " /------\\ ");
            mvwprintw(pit, i*9 + 6, col/8 - 1, " |      |");
            mvwprintw(pit, i*9 + 7, col/8 - 1, " | Free |");
            mvwprintw(pit, i*9 + 8, col/8 - 1, " |      |");
            mvwprintw(pit, i*9 + 9, col/8 - 1, " \\------/");
            mvwprintw(pit, i*9 + 10, col/8 - 1, "O        O");
        }

        for(int i = 0; i < 5; i++){
            mvwprintw(pit, i*9 + 3, 2 * col/6, "Team #%d", i + 6);
            mvwprintw(pit, i*9 + 4, 2 * col/6 - 1, "O        O");
            mvwprintw(pit, i*9 + 5, 2 * col/6 - 1, " /------\\ ");
            mvwprintw(pit, i*9 + 6, 2 * col/6 - 1, " |      |");
            mvwprintw(pit, i*9 + 7, 2 * col/6 - 1, " | Free |");
            mvwprintw(pit, i*9 + 8, 2 * col/6 - 1, " |      |");
            mvwprintw(pit, i*9 + 9, 2 * col/6 - 1, " \\------/");
            mvwprintw(pit, i*9 + 10, 2 * col/6 - 1, "O        O");
        }

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

    void updatePit(int carID, int tire, int color){
        getmaxyx(stdscr, row, col);
        
        if (has_colors())
        {
            start_color();
            switch(color){
                case 1:
                    init_pair(1, COLOR_WHITE, COLOR_BLACK);
                    wattron(this->pitWindow, COLOR_PAIR(1));
                case 2:
                    init_pair(2, COLOR_RED, COLOR_RED);
                    wattron(this->pitWindow,COLOR_PAIR(2));
                case 3:
                    init_pair(3, COLOR_GREEN, COLOR_GREEN);
                    wattron(this->pitWindow, COLOR_PAIR(3));
            }
        }

        if(carID < 10){
            switch(tire){
                case 0:
                    mvwprintw(this->pitWindow, (carID%5)*9 + 4, col/8 - 1, "O");
                case 1:
                    mvwprintw(this->pitWindow, (carID%5)*9 + 4, col/8 + 8, "O");
                case 2:
                    mvwprintw(this->pitWindow, (carID%5)*9 + 10, col/8 - 1, "O");
                case 3:
                    mvwprintw(this->pitWindow, (carID%5)*9 + 10, col/8 + 8, "O");
            }
            wrefresh(this->pitWindow);
            switch(color){
                case 1:
                    wattroff(this->pitWindow, COLOR_PAIR(1));
                case 2:
                    wattroff(this->pitWindow, COLOR_PAIR(2));
                case 3:
                    wattroff(this->pitWindow, COLOR_PAIR(3));
            }
            mvwprintw(this->pitWindow, (carID%5)*9 + 7, col/8 - 1, " | Car %d|", carID + 1);
        }
        else{
            switch(tire){
                case 0:
                    mvwprintw(this->pitWindow, (carID%5)*9 + 4, 2 * col/6 - 1, "O");
                case 1:
                    mvwprintw(this->pitWindow, (carID%5)*9 + 4, 2 * col/6 + 8, "O");
                case 2:
                    mvwprintw(this->pitWindow, (carID%5)*9 + 10, 2 * col/6 - 1, "O");
                case 3:
                    mvwprintw(this->pitWindow, (carID%5)*9 + 10, 2 * col/6 + 8, "O");
            }
            wrefresh(this->pitWindow);
            switch(color){
                case 1:
                    wattroff(this->pitWindow, COLOR_PAIR(1));
                case 2:
                    wattroff(this->pitWindow, COLOR_PAIR(2));
                case 3:
                    wattroff(this->pitWindow, COLOR_PAIR(3));
            }
            mvwprintw(this->pitWindow, (carID%5)*9 + 7, 2 * col/6 - 1, " | Car %d|", carID + 1);
        }
        wrefresh(this->pitWindow);
    }

    void freePit(int carID){
        if(carID < 10){
            mvwprintw(this->pitWindow, (carID%5)*9 + 7, col/8 - 1, " | Free |");
        }
        else{
            mvwprintw(this->pitWindow, (carID%5)*9 + 7, 2 * col/6 - 1, " | Free |");
        }
        updatePit(carID, 0, 1);
        updatePit(carID, 1, 1);
        updatePit(carID, 2, 1);
        updatePit(carID, 3, 1);
        wrefresh(this->pitWindow);
    }

    void updateInfo(int carID){
        getmaxyx(stdscr, row, col);
        mvwprintw(this->infoWindow, (carID + 2) + 5, col/15 + 10, cars[carID]->getStatus());
        wrefresh(this->infoWindow);
    }

    void checkInput(){
        char sign = ' ';
        while (!this->isOver) {
            sign = getchar();
            if (sign == 'q') {
                this->isOver = true;
                for(auto& car : cars){
                    car->thread.join();
                }
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