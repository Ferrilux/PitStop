#include <vector>
#include <string>
#include <mutex>
#include <thread>
#include "Status.cpp"

class Car
{
private:
    int id;
    Status currentStatus;

public:
    std::thread thread;
	std::vector<std::unique_ptr<std::mutex>> wheels;

    Car(int id): wheels(4){
        this->id = id;
        this->currentStatus = onTrack;

        for(auto& wheel : wheels){
            wheel = std::make_unique<std::mutex>();
        }
    }

    void setStatus(Status s){
        this->currentStatus = s;
    }

    const char* getStatus(){

        switch(this->currentStatus){
            case inPit:
                return "In Pit        ";
            case onTrack:
                return "On track      ";
            case waiting:
                return "Waiting in Pit";
            case dnf:
                return "DNF           ";
            default:
                return "Undefined     ";
        }
    }

};