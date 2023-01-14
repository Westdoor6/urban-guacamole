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

// Once a Racer is assigned a WorkPosition, move their racing class into that vector
// in runGroups. This makes sure that we don't assign people in the same racing class
// WorkPositions in conflicting run groups
void add_class_to_run_group(Racer worker, vector<Racer> &entries, vector<Racer> &runGroup)
{
    string carClass = worker.get_class();

    for (auto it = entries.begin(); it != entries.end(); /*In loop*/)
    {
        if (it->get_class() == carClass)
        {
            runGroup.push_back(*it);
            entries.erase(it);
        }
        else
            it++;
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
            if (r->get_name() == name && r->get_job() == WorkPosition::corner)
            {
                r->set_job(pos);
                return *r;
            }
            else if (r->get_name() == name && r->get_job() != WorkPosition::corner)
            {
                cout << "That driver already has a job, select someone else please." << endl;
                break;
            }
        }
        cout << "Driver could not be found!" << endl;
    }
}

Racer search_and_set_pos(WorkPosition pos, vector<Racer> &entries, int runGroupNum)
{
    cout << "For Run Group Number " << runGroupNum + 1 << endl;
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
        cout << c << endl;
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
    vector<vector<Racer>> entries;
    int numberOfDays = 2;
    for (int i = 0; i < numberOfDays; ++i)
    {
        entries.push_back(std::vector<Racer>());
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
            entries[DAY1].push_back(r);
        else if (deets[6] == "Day 2")
            entries[DAY2].push_back(r);
    }
    infile.close();

    // Debug output
    cout << "Day 1 Participant Count " << entries.at(DAY1).size() << endl;
    cout << "Day 2 Participant Count " << entries.at(DAY2).size() << endl;

    vector<WorkPosition> core_pos_for_event = {WorkPosition::courseDesign, WorkPosition::noviceCoach};
    vector<WorkPosition> core_pos_each_group = {WorkPosition::safetySteward,
                                                // WorkPosition::workerChief,
                                                // WorkPosition::timing,
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

        for (auto r : entries.at(day))
        {
            cout << r.get_name() << endl;
        }

        for (auto pos : core_pos_for_event)
        {
            search_and_set_pos(pos, entries.at(day));
        }

        for (int i = 0; i < numRunGroups; ++i)
        {
            for (auto pos : core_pos_each_group)
            {
                Racer temp = search_and_set_pos(pos, entries.at(day), i);
                add_class_to_run_group(temp, entries.at(day), runGroups.at(i));
                print_run_groups(runGroups.at(i));
            }
        }
    }

    return 0;
}