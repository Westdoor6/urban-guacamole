#include "racerClass.h"

std::ostream& operator << (std::ostream& out,const WorkPosition pos){
    const char* s = 0;

   switch(pos)
   {
        case(WorkPosition::checkIn):
            s = "Check In";
            break;     
        case(WorkPosition::corner):
            s = "Corner Worker";
            break;   
        case(WorkPosition::courseDesign):
            s = "Course Designer";
            break;
        case(WorkPosition::grid):
            s = "Grid";
            break;
        case(WorkPosition::noviceCoach):
            s = "Novice Coach";
            break;
        case(WorkPosition::safetySteward):
            s = "Safety Steward";
            break;
        case(WorkPosition::startLine):
            s = "Starting Line";
            break;
        case(WorkPosition::workerChief):
            s = "Worker Chief";
            break;
        case(WorkPosition::gate):
            s = "Gate/Perimiter";
            break;
        case(WorkPosition::timing):
            s = "Timing Operator";
            break;
    }

    return out << s;
}

Racer::Racer()
{
    sccaMemberNumber = 606060;
    driverName = "Baby Driver";
    carClass = "AIR";
    carMakeModel = "Mazda Prius";
    carNumber = 999;
    job = WorkPosition::corner;
}

Racer::Racer(int SccaMemberNo, std::string DriverName, std::string CarClass, int CarNumber, std::string CarMakeModel, WorkPosition Job = WorkPosition::corner)
{
    sccaMemberNumber = SccaMemberNo;
    driverName = DriverName;
    carClass = CarClass;
    carNumber = CarNumber;
    carMakeModel = CarMakeModel;
    job = Job;
}

void Racer::print_entry_info()
{
    std::cout << "SCCA Member Number --- " << sccaMemberNumber << std::endl;
    std::cout << "Driver Name --- " << driverName << std::endl;
    std::cout << "Worker Position --- " << job << std::endl;
    std::cout << "Car Class --- " << carClass << std::endl;
    std::cout << "Car Number --- " << carNumber << std::endl;
    std::cout << "Car Make/Model --- " << carMakeModel << std::endl;
}

WorkPosition Racer::get_job()
{
    return job;
}
void Racer::set_job(WorkPosition newJob)
{
    job = newJob;
}

std::string Racer::get_name()
{
    return driverName;
}

std::string Racer::get_name() const
{
    return driverName;
}

std::string Racer::get_class()
{
    return carClass;   
}