#include "mutex"

class Wheel
{
private:
    std::mutex tire;
    enum color
    {
        red,
        green
    };
};