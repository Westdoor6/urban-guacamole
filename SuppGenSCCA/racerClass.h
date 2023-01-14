#ifndef RACER
#define RACER

#include<string>
#include<iostream>

enum class WorkPosition 
{
    courseDesign,
    safetySteward,
    checkIn,
    timing,
    grid,
    noviceCoach,
    startLine,
    workerChief,
    gate,
    corner
};

std::ostream& operator << (std::ostream& out,const WorkPosition pos);

class Racer
{
    public:
        void print_entry_info();
        WorkPosition get_job();
        void set_job(WorkPosition);
        std::string get_name();
        std::string get_name() const;
        std::string get_class();
        Racer();
        Racer(int SccaMemberNo, std::string DriverName, std::string CarClass, int CarNumber, std::string CarMakeModel, WorkPosition job);
    private:
        int sccaMemberNumber;
        std::string driverName;
        std::string carClass;
        int carNumber;
        std::string carMakeModel;
        WorkPosition job;
};

#endif