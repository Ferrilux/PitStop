#include <vector>
#include <string>
#include <mutex>
#include <thread>

    enum status
    {
        inPit,
        onTrack,
        waiting,
        dnf
    };

class Car
{
private:
	std::vector<std::unique_ptr<std::mutex>> wheels;
    int id;
    status currentStatus;

public:
    std::thread thread;

    Car(int id): wheels(4){
        this->id = id;
        this->currentStatus = onTrack;

        for(auto& wheel : wheels){
            wheel = std::make_unique<std::mutex>();
        }
    }

    void setStatus(int i){
        switch(i){
            case 0:
                this->currentStatus = inPit;
            case 1:
                this->currentStatus = onTrack;
            case 2:
                this->currentStatus = waiting;
            case 3:
                this->currentStatus = dnf;
            default:
                this->currentStatus = onTrack;
        }
    }

    const char* getStatus(){

        switch(this->currentStatus){
            case inPit:
                return "In Pit";
            case onTrack:
                return "On track";
            case waiting:
                return "Waiting in Pit";
            case dnf:
                return "DNF";
            default:
                return "Undefined";
        }
    }
};