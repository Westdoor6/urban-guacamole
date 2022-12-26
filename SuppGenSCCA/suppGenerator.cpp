#include "racerClass.h"
#include <vector>
#include <fstream>


static inline bool isCoreWorker (Racer driver) {return    driver.getJob() == WorkPosition::safetySteward || 
                                                            driver.getJob() == WorkPosition::courseDesign ||
                                                            driver.getJob() == WorkPosition::noviceCoach; };

std::vector<std::string> split(std::string line, char delim = ',')
{
    std::vector<std::string> ret;
    std::string builder;
        
    for(auto c : line)
    {
        if( c != delim)
            builder.push_back(c);
        else
        {
            ret.push_back(builder); 
            builder = "";
        }
    }

    return ret;
}

int main()
{
    std::cout << "Welcome to the AutoCross Supplemental Generator." << std::endl;
    std::cout << "Please enter the name of the file that contains the entries for this event" << std::endl;
    
    std::string filename;
    std::cin >> filename;
    
    //
    // Temporarily hardcode the filename to make testing easier/quicker
    //
    std::ifstream infile("entries.csv");

    while(!infile.is_open())
    {
        std::cout << "File could not be opened. Please enter a valid filename." << std::endl;
        //std::cin >> filename;
        infile.open(filename);
    }

    std::vector<Racer> entries;
    std::string line;

    // Get rid of the header/labels. We already know the order
    getline(infile, line);

    while(getline(infile, line))
    {   
        std::vector<std::string> deets = split(line);
        
        // If no SCCA member no is presented assign 000000
        if(deets[5] == "")
            deets[5] = "000000";

        std::cout << "D5 : " << deets[5] << " D1 : " << deets[1] << std::endl;
        Racer r(stoi(deets[5]),deets[2] + " " + deets[3], deets[0], std::stoi(deets[1]), deets[4],WorkPosition::corner);
        entries.push_back(r);
    }

    for(int i = 0 ; i < 5 ; ++i)
    {
        entries.at(i).printEntryInfo();
    }

    infile.close();
    return 0;
}