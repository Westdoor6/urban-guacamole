#include "racerClass.h"
#include <vector>
#include <fstream>
#include <regex>

using namespace std;

// Used to make the "entries" vectors more readable
const int DAY1 = 0;
const int DAY2 = 1;

static inline bool is_core_worker(Racer driver) { return driver.get_job() == WorkPosition::safetySteward ||
                                                         driver.get_job() == WorkPosition::courseDesign ||
                                                         driver.get_job() == WorkPosition::noviceCoach; };

static inline string trim_whitespace(string value)
{
    return regex_replace(value, regex("^ +| +$|( ) +"), "$1");
}

// Function to split the CSV entries into a vector of strings.
// Strings can then be used to create a "Racer" object
vector<string> split(string line, char delim = ',')
{
    vector<string> ret;
    string builder;

    for (auto c : line)
    {
        // This is not ideal. It seems hacky to me but
        // deals with the last element of the line from the CSV
        // I could see this possibly dropping someone from the
        // event, but haven't proven that occurs
        //
        // It seems to work properly as of now
        if (c == '\r')
            break;
        if (c != delim)
            builder.push_back(c);
        else
        {
            ret.push_back(builder);
            builder = "";
        }
    }

    ret.push_back(builder);

    return ret;
}

// Once all WorkPositions are assigned for a given run group, move their racing class into that vector
// in runGroups. This makes sure that we don't assign people in the same racing class
// WorkPositions in conflicting run groups
void add_classes_to_run_group(vector<Racer> &entries, vector<Racer> &runGroup)
{
    // Build a "classes" vector based on drivers who have a non "corner" job
    std::vector<string> classes;
    for (auto r : entries)
    {
        if (r.get_job() != WorkPosition::corner && !is_core_worker(r))
        {
            classes.push_back(r.get_class());
        }
    }

    // For all the classes we've pulled above, move all drivers in that class into the
    // run group
    for (auto it = entries.begin(); it != entries.end(); /*In loop*/)
    {
        if (std::find(classes.begin(), classes.end(), it->get_class()) == classes.end())
        {
            it++;
        }
        else
        {
            runGroup.push_back(*it);
            entries.erase(it);
        }
    }
}

// Search through the entries for the event for a given name
// If the name is found, assign that Racer to the appropriate WorkPosition
Racer search_and_set_pos(WorkPosition pos, vector<Racer> &entries)
{
    while (true)
    {
        cout << "Please enter the name of the person who will be the " << pos << endl;
        string name;
        getline(cin, name);
        trim_whitespace(name);

        for (vector<Racer>::iterator r = entries.begin(); r != entries.end(); ++r)
        {
            bool nameFound = (r->get_name() == name);

            if (nameFound && r->get_class() == "N")
            {
                cout << "Driver is a novice, let's try someone else" << endl;
                break;
            }
            if (nameFound && r->get_job() == WorkPosition::corner)
            {
                r->set_job(pos);
                return *r;
            }
            else if (nameFound && r->get_job() != WorkPosition::corner)
            {
                cout << "That driver already has a job, select someone else please." << endl;
                break;
            }
            if (r == entries.end() - 1)
                cout << "Driver could not be found!" << endl;
        }
    }
}

Racer search_and_set_pos(WorkPosition pos, vector<Racer> &entries, int runGroupNum)
{
    cout << endl
         << "For Run Group Number " << runGroupNum + 1 << endl;
    cout << "--------------------------" << endl;
    return search_and_set_pos(pos, entries);
}

void print_run_groups(vector<Racer> groupEntries)
{
    std::vector<string> classes;
    for (auto r : groupEntries)
    {
        if (std::find(classes.begin(), classes.end(), r.get_class()) == classes.end())
        {
            classes.push_back(r.get_class());
        }
    }

    for (string c : classes)
    {
        cout << "\t   ---   " << c << "   ---" << endl;
        for (auto r : groupEntries)
        {
            if (r.get_class() == c)
                cout << "\t   " << r.get_name() << endl;
        }
    }
}

int find_smallest_group_index(vector<vector<Racer>> runGroups)
{
    int smallestGroupIndex = 0;
    int count = runGroups.at(0).size();

    for (int group = 0; group < runGroups.size(); ++group)
    {
        if (runGroups.at(group).size() < count)
        {
            count = runGroups.at(group).size();
            smallestGroupIndex = group;
        }
    }

    return smallestGroupIndex;
}

void distribute_remaining_classes(vector<Racer> &remainingRacers, vector<vector<Racer>> &runGroups)
{
    cout << "Would you like to divide the novices evenly between the run groups (y/n)?" << endl;
    string even;
    cin >> even;

    bool divide = true;
    if (even == "n" || even == "N")
    {
        divide = false;
    }

    while (remainingRacers.size() != 0)
    {
        int smallestGroup = find_smallest_group_index(runGroups);

        string cl = remainingRacers.front().get_class();
        for (vector<Racer>::iterator racer = remainingRacers.begin();
             racer != remainingRacers.end();
             /*In loop*/)
        {
            racer->print_entry_info();
            if (racer->get_class() == "N" && divide == true && cl == "N")
            {
                runGroups.at(smallestGroup).push_back(*racer);
                smallestGroup = (smallestGroup + 1) % runGroups.size();
                remainingRacers.erase(racer);
            }
            else if (racer->get_class() == cl)
            {
                runGroups.at(smallestGroup).push_back(*racer);
                remainingRacers.erase(racer);
            }
            else if (racer != remainingRacers.end())
                ++racer;
        }
    }
}

/*Main Driver for the generator
Will read input from a file provided by the user.
Racer objects will be created for each entry in the CSV file
After that the user will be prompted to assign racers to each of the
core positions for the event. Anyone not assigned to a core position
will be assigned to a corner work position.*/

int main()
{
    cout << "Welcome to the AutoCross Supplemental Generator." << endl;
    cout << "Please enter the name of the CSV file that contains the entries for this event" << endl;

    // string filename;
    // cin >> filename;

    //
    // Temporarily hardcode the filename to make testing easier/quicker
    //
    ifstream infile("entries.csv");

    while (!infile.is_open())
    {
        cout << "File could not be opened. Please enter a valid filename." << endl;
        // cin >> filename;
        // infile.open(filename);
    }

    // Rip Racer info from the provided file
    // This is designed for a CSV
    vector<vector<Racer>> entriesPerDay;
    int numberOfDays = 2;
    for (int i = 0; i < numberOfDays; ++i)
    {
        entriesPerDay.push_back(std::vector<Racer>());
    }
    string line;

    // Get rid of the header/labels. We already know the order
    getline(infile, line);

    while (getline(infile, line))
    {
        vector<string> deets = split(line);

        // If no SCCA member number is presented, assign 000000
        // Likely not needed
        if (deets[5] == "")
            deets[5] = "000000";

        // cout << "D5 : " << deets[5] << " D1 : " << deets[1] << endl;
        Racer r(stoi(deets[5]), deets[2] + " " + deets[3], deets[0], stoi(deets[1]), deets[4], WorkPosition::corner);

        if (deets[6] == "Day 1")
            entriesPerDay[DAY1].push_back(r);
        else if (deets[6] == "Day 2")
            entriesPerDay[DAY2].push_back(r);
    }
    infile.close();

    // Debug output
    cout << "Day 1 Participant Count " << entriesPerDay.at(DAY1).size() << endl;
    cout << "Day 2 Participant Count " << entriesPerDay.at(DAY2).size() << endl;

    vector<WorkPosition> core_pos_for_event = {
        /*  WorkPosition::courseDesign,
            WorkPosition::noviceCoach,
            WorkPosition::safetySteward*/
    };
    vector<WorkPosition> core_pos_each_group = {// WorkPosition::workerChief,
                                                WorkPosition::timing,
                                                // WorkPosition::checkIn,
                                                // WorkPosition::grid,
                                                // WorkPosition::startLine,
                                                WorkPosition::gate};

    for (int day = 0; day < 2; ++day)
    {
        // Variables we need from the user
        // Number of Run Groups
        // Names for core work positions
        cout << "Please enter the desired number of run groups for day " << day + 1 << ":" << endl;
        int numRunGroups;
        cin >> numRunGroups;
        cin.ignore(); // Deal with leftover whitespace in buffer after getting int

        vector<vector<Racer>> runGroups;
        runGroups.resize(numRunGroups);

        for (auto racer : entriesPerDay.at(day))
        {
            cout << racer.get_name() << endl;
        }

        for (auto pos : core_pos_for_event)
        {
            search_and_set_pos(pos, entriesPerDay.at(day));
        }

        for (int i = 0; i < numRunGroups; ++i)
        {
            for (auto pos : core_pos_each_group)
            {
                search_and_set_pos(pos, entriesPerDay.at(day), i);
            }
            add_classes_to_run_group(entriesPerDay.at(day), runGroups.at(i));
        }

        distribute_remaining_classes(entriesPerDay.at(day), runGroups);

        cout << endl
             << "----------    Day " << day + 1 << "    ----------" << endl;
        for (int i = 0; i < numRunGroups; ++i)
        {
            cout << "--- Run Group " << i + 1 << ", " << runGroups.at(i).size() << " Entries ---" << endl;
            print_run_groups(runGroups.at(i));
        }
    }

    return 0;
}