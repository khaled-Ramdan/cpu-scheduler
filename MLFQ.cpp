#include"common.h"
// vector to store all processes

int number_of_processes;
int io_waiting_time;
int instruction_execution_time;
vector<Process>process;

struct cmp
{
    bool operator() (pair<int, pair<int, int>>a, pair<int, pair<int, int>>b)
    {
        if (a.first > b.first)return 1;
        if (a.first < b.first)return 0;
        return a.second.first < b.second.first;
    }
};

class MLFQ
{
    private:

         // info of MLFQ
        int levels;                                  // Number of Priority Levels of the MLFQ
        int start_time_slice;                        // Time Slice for the Top Most level
        int time_slice_diff;                         // Difference between the time slice between any two consective levels
        int cur_time;                                // current Time Of The Scheduling
        int pointer;                                 // point to the first Process which doesn't enter the System
        int topLevel;                                // The number of index of the Top Most Level
        int priority_boost;                          // period to boost
        int lastBoost;                               // the Time of last boost 

        vector<int>timeSliceOfQueue;                 // Time Slice of each Queue
        vector<queue<int>>priorityLevelRunning;      // Rinning Processes for each level
        priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, cmp>blocked;     // blocked Processes
        set<int, greater<int>>usedLevels;            // which levels are used and sorted from higher to lower

    public:

        MLFQ()                                      // MLFQ Constructor
        {
            levels           = get_levels();
            time_slice_diff  = get_time_slice_diff(); 
            start_time_slice = 5;
            cur_time         = 0;
            pointer          = 0;
            topLevel         = levels - 1;
            priority_boost   = 20;
            lastBoost        = 0;
            priorityLevelRunning.resize(levels);
            timeSliceOfQueue.resize(levels);
            get_time_slice_of_each_queue();
        }

    private: 

        int get_levels()                            // get The Number of Levels of The MLFQ from The User
        {
            cout << "Enter The Number of Levels Of This MLFQ: ";
            int resp;
            cin >> resp;
            return resp;
        }

        int get_time_slice_diff()                   // get The Time Slice Difference from The User
        {
            cout << "Enter The Time Slice Difference between two levels in This MLFQ: ";
            int resp;
            cin >> resp;
            return resp;
        }

        void get_time_slice_of_each_queue()         // assign Time Slice of each Queue
        {
            for (int i = topLevel, j = 0; i >= 0; i--, j++)
            {
                timeSliceOfQueue[i] = start_time_slice + j * time_slice_diff;
            }
        }

    public: 

        void scheduler()                            // To run The Processes
        {

            while (isNotAllFinish())                // while The processes is not finished The Scheduling still running
            {            

                // for every period Time(priority_boost) The Processes boosted to the top most level
                if (cur_time % priority_boost == 0 && cur_time)
                {
                    boost();
                }

                // append The ready Processes in their Levels
                fromReadyToRunning();

                if (usedLevels.empty())            // if There is No Processes to run  => snap
                {
                    cur_time++;
                    continue;
                }

                int highestUsedLevel = *usedLevels.begin();     // get The Highest Used Level
                int cur_process      = priorityLevelRunning[highestUsedLevel].front();
                bool flag            = 0;                       // if still 0 that meaning the process is not blocked otherwise the process is blocked
            
                //  loop until the process is finished or blocked or it uses up the allotment time slice
                while (process[cur_process].insIdx < process[cur_process].numberOfInstructions)
                {

                    if (isCompleteTimeSlice(process[cur_process].consumed, timeSliceOfQueue[highestUsedLevel]))
                    {
                        // if the process completes the allotment time slice it will move down to the next level
                        process[cur_process].consumed = 0;
                        int nextLevel = max(0, highestUsedLevel - 1);      // get the next level and if the current level is 0 stay in the same level
                        priorityLevelRunning[nextLevel].push(cur_process);
                        usedLevels.insert(nextLevel);
                        break;
                    }

                    if (process[cur_process].insType[process[cur_process].insIdx])
                    {
                        // if the process is blocked => io instruction
                        block_process(highestUsedLevel, cur_process, cur_time);
                        process[cur_process].consumed--;
                        flag = 1;
                    }
                    process[cur_process].insIdx++;      // go to The Next Instruction
                    process[cur_process].consumed++;    
                    if (flag == 0)
                    {
                        cur_time += instruction_execution_time;
                    }
                    else break;
                }

                // pop The current process                
                priorityLevelRunning[highestUsedLevel].pop();
                if (priorityLevelRunning[highestUsedLevel].empty())
                {
                    usedLevels.erase(usedLevels.begin());
                }

                cout << highestUsedLevel << ' ' << timeSliceOfQueue[highestUsedLevel] << '\n';
                if (flag == 1)
                {
                    cout << process[cur_process].pId << " is Blocked at Current Time = " << cur_time << endl; 
                }
                else if (process[cur_process].insIdx == process[cur_process].numberOfInstructions)
                {
                    cout << process[cur_process].pId << " is Finished at Current Time = " << cur_time << endl; 
                }
                else 
                {
                    cout << process[cur_process].pId << " uses up the Time Slice Current Time = " << cur_time << endl; 
                }
            }

        }

    private:
    
        bool isCompleteTimeSlice(int consumed, int time_slice)  // check if is the process uses up the allotment time slice or not
        {
            return consumed >= time_slice;
        }

        void block_process(int level, int id, int cur_time) // block the Process => io Instruction and after io waiting time the process back ready
        {
            process[id].readyTime = cur_time + io_waiting_time;
            blocked.push({process[id].readyTime, {level, id}});
        }

        void fromReadyToRunning()                 // append The Ready and New Processes in Their Levels
        {

            while ((blocked.empty() == 0 && blocked.top().first <= cur_time) &&
                   (pointer < number_of_processes && process[pointer].readyTime <= cur_time))
            {
                if (blocked.top().first <= process[pointer].readyTime)appendFromBlocked();
                else appendFromNewProc();
            }

            while (blocked.empty() == 0 && blocked.top().first <= cur_time)
            {
                appendFromBlocked();
            }

            while (pointer < number_of_processes && process[pointer].readyTime <= cur_time)
            {
                appendFromNewProc();
            }
           
        }

        void appendFromBlocked()
        {
            // change The state of Ready Process to running and apend it in its level
            pair<int, int>ready_process = blocked.top().second;
            blocked.pop();
            int cur_level = topLevel;
            if (blocked.top().first - io_waiting_time >= lastBoost)
            {
                cur_level = ready_process.first;
            }
            priorityLevelRunning[cur_level].push(ready_process.second);
            usedLevels.insert(cur_level);
            return;
        }

        void appendFromNewProc()
        {
            // append the new Process in The top Level
            priorityLevelRunning[topLevel].push(pointer++);
            usedLevels.insert(topLevel);
            return;
        }

        void boost()                              // boost all processing in The System to The TOP MOSt Level
        {
            // loop for all level and remove its processes and push them in the top level
            for (int i = topLevel - 1; i >= 0; i--)
            {
                while (priorityLevelRunning[i].empty() == 0)
                {
                    // push The process in The Top level 
                    priorityLevelRunning[topLevel].push(priorityLevelRunning[i].front());
                    // remove The process from The current Level
                    priorityLevelRunning[i].pop();
                }
            }

            lastBoost += priority_boost;
            
            usedLevels.clear();         // clear The Used Levels
            // The only used level now is The Top or there is no levels used
            if (priorityLevelRunning[topLevel].empty() == 0)
            {
                usedLevels.insert(topLevel);
            }
            
            return;
        }

        bool isNotAllFinish()                     // check if all processes finished or not
        {
            return pointer < number_of_processes || usedLevels.empty() == 0 || blocked.empty() == 0; 
        }

};

void MLFQS(const vector<Process>& v,int n,int m,int k)
{
    process = v;
    number_of_processes = n;
    io_waiting_time = m;
    instruction_execution_time = k;
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
    mlfq.scheduler();


}
