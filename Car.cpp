#include "Wheel.cpp"

class Car
{
    Wheel wheels[4];
    enum status
    {
        inPit,
        onTrack,
        waiting
    };
};