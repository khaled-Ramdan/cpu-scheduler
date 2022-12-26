#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <queue>
#include <set>

using namespace std;

// vector to store all processes

int number_of_processes;
int io_waiting_time;
int instruction_execution_time;

struct Process 
{

    // info for each Process
	string PID;
    int numberOfInstructions, IOPercent, readyTime, insIdx, consumed;
    // instruction Type vector -> to know the type for the instruction where 0 => cpu and 1 => io
    vector<bool>insType; 

    // Initalize info 
	Process() 
    { 
        readyTime = 0;
        PID = ""; 
        numberOfInstructions = 0; 
        IOPercent = 0; 
        insIdx = 0; 
        consumed = 0;
    };

    // constructor of this struct
	Process(string pid, int instructionCount, int iopercent, int arrivalTime)
    {
		PID = pid;
		numberOfInstructions = instructionCount;
		IOPercent = iopercent;
		readyTime = arrivalTime;
        insIdx = 0;
        consumed = 0;
        // resize the instruction_type vector (instruction_type.size() = number_of_instructions) and assign each index by 0
		insType.assign(numberOfInstructions, 0); 
		// call randomizeIO function to determine which instruction is cpu or io
        randomizeIO();
	}

    // determine each instruction is cpu or io
	void randomizeIO() 
    {
        // get the number of io intructions depend on io percentage
        int ioCount = IOPercent * numberOfInstructions / 100;
		// make the first ioCount in the instructions is io instruction
        for (int i = 0; i < ioCount; i++)
		{
            insType[i] = 1;    // means this instruction is io instruction
        }
        // shuffle all instructions randomly
		random_shuffle(insType.begin(), insType.end());
	}

    // sort the processes according to its ready_time
	bool operator< (const Process& p) const 
    {
		return p.readyTime < this->readyTime;
	}
};

struct cmp
{
    bool operator() (pair<int, pair<int, int>>a, pair<int, pair<int, int>>b)
    {
        if (a.first > b.first)return 1;
        if (a.first < b.first)return 0;
        return a.second.first < b.second.first;
    }
};

vector<Process>process;

class MLFQ
{
    public: 

    int levels, time_slice_diff, start_time_slice, cur_time, pointer, topLevel, priority_boost, lastBoost;
    vector<int>timeSliceOfQueue;
    vector<queue<int>>priorityLevelRunning;
    priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, cmp>blocked;
    set<int, greater<int>>usedLevels;

    MLFQ()
    {
        levels           = get_levels();
        time_slice_diff  = get_time_slice_diff(); 
        start_time_slice = 5;
        cur_time         = 0;
        pointer          = 0;
        topLevel         = levels - 1;
        priority_boost   = 1000;
        lastBoost        = 0;
        priorityLevelRunning.resize(levels);
        timeSliceOfQueue.resize(levels);
        get_time_slice_of_each_queue();
    }

    int get_levels()
    {
        cout << "Enter The Number of Levels Of This MLFQ: ";
        int resp;
        cin >> resp;
        return resp;
    }

    int get_time_slice_diff()
    {
        cout << "Enter The Time Slice Difference between two levels in This MLFQ: ";
        int resp;
        cin >> resp;
        return resp;
    }

    void get_time_slice_of_each_queue()
    {
        for (int i = levels - 1, j = 0; i >= 0; i--, j++)
        {
            timeSliceOfQueue[i] = start_time_slice + j * time_slice_diff;
        }
    }

    void solve()
    {

        while (isNotAllFinish())
        {            


            if (cur_time % priority_boost == 0 && cur_time)
            {
                boost();
            }

            fromReadyToRunning();

            if (usedLevels.empty())
            {
            cout << cur_time << ' ' << blocked.size() << '\n';
                // cout << cur_time << ' ' << process[2].readyTime << '\n';
                cur_time++;
                continue;
            }

            int highestUsedLevel = *usedLevels.begin();
            int cur_process      = priorityLevelRunning[highestUsedLevel].front();
            bool flag            = 0;
           
            while (process[cur_process].insIdx < process[cur_process].numberOfInstructions)
            {

                // cout << cur_process << ' ' << cur_time << ' ' << process[cur_process].insIdx << ' ' << process[cur_process].consumed << ' ' << timeSliceOfQueue[highestUsedLevel] << '\n';
                 cout << cur_time << ' ' << blocked.size() << '\n';
                if (isCompleteTimeSlice(process[cur_process].consumed, timeSliceOfQueue[highestUsedLevel]))
                {
                    process[cur_process].consumed = 0;
                    int nextLevel = max(0, highestUsedLevel - 1);
                    priorityLevelRunning[nextLevel].push(cur_process);
                    break;
                }

                if (process[cur_process].insType[process[cur_process].insIdx])
                {
                    block_process(highestUsedLevel, cur_process, cur_time);
                    process[cur_process].consumed--;
                    flag = 1;
                }
                process[cur_process].insIdx++;
                process[cur_process].consumed++; 
                if (flag == 0)
                {
                    cur_time += instruction_execution_time;
                }
                else break;
            }
            
            priorityLevelRunning[highestUsedLevel].pop();
            if (priorityLevelRunning[highestUsedLevel].empty())
            {
                usedLevels.erase(usedLevels.begin());
            }

            // cout << highestUsedLevel << '\n';
            if (flag == 1)
            {
                cout << process[cur_process].PID << " is Blocked at Current Time = " << cur_time << endl; 
            }
            else if (process[cur_process].insIdx == process[cur_process].numberOfInstructions)
            {
                cout << process[cur_process].PID << " is Finished at Current Time = " << cur_time << endl; 
            }
            else 
            {
                cout << process[cur_process].PID << " uses up the Time Slice Current Time = " << cur_time << endl; 
            }
        }

    }

    bool isCompleteTimeSlice(int consumed, int time_slice)
    {
        return consumed >= time_slice;
    }

    void block_process(int level, int id, int cur_time)
    {
        process[id].readyTime = cur_time + io_waiting_time;
        // cout << id << ' ' << level << ' ' << process[id].readyTime << '\n'; 
        blocked.push({process[id].readyTime, {level, id}});
    }

    void fromReadyToRunning()
    {
        // updateCurTime();

        while (blocked.empty() == 0 && blocked.top().first <= cur_time)
        {
            pair<int, int>ready_process = blocked.top().second;
            blocked.pop();
            int cur_level = 0;
            if (blocked.top().first - io_waiting_time >= lastBoost)
            {
                cur_level = ready_process.first;
            }
            priorityLevelRunning[cur_level].push(ready_process.second);
            usedLevels.insert(cur_level);
        }

        while (pointer < number_of_processes && process[pointer].readyTime <= cur_time)
        {
            priorityLevelRunning[topLevel].push(pointer++);
            usedLevels.insert(topLevel);
        }
    }

    // void updateCurTime()
    // {
    //     if (blocked.empty() == 0 && pointer < number_of_processes)
    //     {
    //         cur_time = max(cur_time, min(blocked.top().first, process[pointer].readyTime));
    //     }
    //     else if (blocked.empty() == 0)
    //     {
    //         cur_time = max(cur_time, blocked.top().first);
    //     }
    //     else 
    //     {
    //         cur_time = max(cur_time, process[pointer].readyTime);
    //     }
    //     if (cur_time >= lastBoost + priority_boost)
    //     {
    //         boost();
    //         updateCurTime();
    //     }
    // }

    void boost()
    {
        for (int i = 1; i < levels; i++)
        {
            while (priorityLevelRunning.empty() == 0)
            {
                priorityLevelRunning[0].push(priorityLevelRunning[i].front());
                priorityLevelRunning[i].pop();
            }
        }
        lastBoost += priority_boost;
    }

    bool isNotAllFinish()
    {
        return pointer < number_of_processes || usedLevels.empty() == 0 || blocked.empty() == 0; 
    }

};

int main()
{
    fstream dataSet;
    
    // open the 'Data_set.txt' File 
    dataSet.open("Data_Set.txt", ios::in);   

    // if file Opening failed    
    if(!dataSet)
    {
        cout << "File Opening failed"; 
        return 0;
    } 

    // if reaches here the file is opened

    // get the number of processes
    dataSet >> number_of_processes;

    // get the IO waiting time
    dataSet >> io_waiting_time;

    // get the instruction execution time
    dataSet >> instruction_execution_time;
    

    // loop to get info for each process
    for (int i = 0; i < number_of_processes; i++)
    {
        string PID;
        int number_of_instructions, io_percent, arrival_time;

        // get the pid for this instruction
        dataSet >> PID;

        // get the instruction count for this process
        dataSet >> number_of_instructions;

        // get the IO percentage for this process
        dataSet >> io_percent;

        // get the arrival time for this process
        dataSet >> arrival_time;
 
        // push this process in process vector
        process.push_back(Process(PID, number_of_instructions, io_percent, arrival_time));
    }

    // close the data set file
    dataSet.close();

    // if it reaches here data set is uploaded

    cout << process.size() << '\n';
    for (int i = 0; i < number_of_processes; i++)
    {
        cout << "process num " << i << "   " << process[i].readyTime << ":\t";
        for (int j = 0; j < process[i].numberOfInstructions; j++)
        {
            cout << process[i].insType[j] << ' ';
        }
        cout << "\n\n";
    }

    MLFQ mlfq;
    mlfq.solve();


}
