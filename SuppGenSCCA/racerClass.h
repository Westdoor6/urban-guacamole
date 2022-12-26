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
    corner
};

class Racer
{
    public:
        void printEntryInfo();
        WorkPosition getJob();
        void setJob(WorkPosition);
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