#include "racerClass.h"

static inline bool isCoreWorker (Entry driver) {return    driver.getJob() == WorkPosition::safetySteward || 
                                                            driver.getJob() == WorkPosition::courseDesign ||
                                                            driver.getJob() == WorkPosition::noviceCoach; };

int main()
{
    std::cout << "Welcome to the AutoCross Supplemental Generator." << std::endl;
    std::cout << "Please enter the name of the file that contains the entries for this event" << std::endl;
    
    std::string filename;
    std::cin >> filename;

   
    return 0;
}