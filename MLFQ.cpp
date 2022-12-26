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
                 cout << highestUsedLevel << ' ' << cur_process << ' ' << cur_time << ' ' << blocked.size() << '\n';
                if (isCompleteTimeSlice(process[cur_process].consumed, timeSliceOfQueue[highestUsedLevel]))
                {
                    process[cur_process].consumed = 0;
                    int nextLevel = max(0, highestUsedLevel - 1);
                    priorityLevelRunning[nextLevel].push(cur_process);
                    usedLevels.insert(nextLevel);
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

void MLFQS(const vector<Process>& v,int n,int m,int k)
{
    process=v;
    number_of_processes=n;
    io_waiting_time=m;
    instruction_execution_time=k;
    // if it reaches here data set is uploaded

    // cout << process.size() << '\n';
    // for (int i = 0; i < number_of_processes; i++)
    // {
    //     cout << "process num " << i << "   " << process[i].readyTime << ":\t";
    //     for (int j = 0; j < process[i].numberOfInstructions; j++)
    //     {
    //         cout << process[i].insType[j] << ' ';
    //     }
    //     cout << "\n\n";
    // }

    MLFQ mlfq;
    mlfq.solve();


}
