#include "Wheel.cpp"
#include <string>
    
    enum status
    {
        inPit,
        onTrack,
        waiting
    };

class Car
{
private:
    Wheel wheels[4];
    int id;
    status currentStatus;

public:
    Car(int id){
        this->id = id;
        this->currentStatus = onTrack;
    }

    const char* getStatus(){

        switch(this->currentStatus){
            case inPit:
                return "In Pit";
            case onTrack:
                return "On track";
            case waiting:
                return "Waiting in Pit";
            default:
                return "Undefined";
        }
    }
};