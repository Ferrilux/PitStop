#include "Wheel.cpp"

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
};