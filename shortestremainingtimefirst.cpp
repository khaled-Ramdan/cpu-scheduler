#include"STCF.h"
void shortestRemainingTimeFirst(vector<Process>& v, ll IOTime, ll insTime)
{
    if (v.empty()) {
        cout << "No processes\n";
        return;
    }
    priority_queue<Process, vector<Process>, SRTFcompare>app;
    map<ll, processMetaData>procData;
    for (Process& i : v)
    {
        app.push(i);
        procData[i.pId].arrival = i.readyTime;
    }
    ll cTime = 0, lft = 0;
    pair<ll, ll>currentProc = { -1,0 };//id - when it take the cpu
    while (app.size())
    {
        Process proc = app.top();
        app.pop();
        if (proc.readyTime < cTime) {
            //comming process readyTime is less than current time
            proc.readyTime = cTime;
            app.push(proc);
            continue;
        }
        if (proc.readyTime > cTime)
        {//free cpu from cTime => readyTime
            cout << "cpu is free now. current time = " << cTime << endl;
            cTime = proc.readyTime;
            app.push(proc);
            continue;
        }
        //check process context switch
        if (proc.pId != currentProc.first) {
            cout << "process " << proc.pId << " starts running. current Time = " << cTime << endl;
            currentProc = make_pair(proc.pId, cTime);
        }
        if (procData[proc.pId].firstTun == -1)procData[proc.pId].firstTun = cTime;
        if (proc.insType[proc.insIdx] == 0)
        {// cpu instruction
            proc.insIdx++;
            cTime += insTime;
            proc.readyTime = cTime;
            if (proc.insIdx != proc.numberOfInstructions)
                app.push(proc);
            if (proc.insIdx == proc.numberOfInstructions) {
                procData[proc.pId].completion = cTime;
                cout << "Process is terminated. current Time = " << cTime << endl;
            }
            continue;
        }
        //io instruction
        cout << "process "<<currentProc.first<<" issues IO at " << cTime << ".....\n";
        currentProc.second = cTime;
        proc.insIdx++;
        cTime += insTime;
        proc.readyTime = cTime + IOTime;
        if (proc.insIdx != proc.numberOfInstructions)
            app.push(proc);
        if (proc.insIdx == proc.numberOfInstructions) {
            procData[proc.pId].completion = proc.readyTime;
            cout << "process  terminates after IO at " << proc.readyTime << endl;
        }
    }

    cout << endl;
    procData.erase(-1);
    metaDataDisplay(procData, v.size());

}
