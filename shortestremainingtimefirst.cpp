#include"STCF.h"
void shortestRemainingTimeFirst(vector<Process>& v, ll IOTime,ll ITIME)
{
    priority_queue<Process>app;
    map<ll, processMetaData>procData;
    for (Process& i : v)
    {
        app.push(i);
        procData[i.pId].arrival = i.readyTime;
    }
    ll cTime = 0;
    pair<ll, ll>currentProc = { app.top().pId,app.top().readyTime };//id - when it take the cpu
    while (app.size())
    {
        Process proc = app.top();
        app.pop();
        if (proc.readyTime < cTime) {
            proc.readyTime = cTime;
            app.push(proc);
            continue;
        }
        if (proc.pId != currentProc.first) {
            print(currentProc.second, cTime, currentProc.first, procData[currentProc.first].completion != -1,0);
            currentProc = make_pair(proc.pId, proc.readyTime);
        }
        if (proc.readyTime > cTime)
        {
            print(cTime, proc.readyTime - 1,  -1,0,0);
            cTime = proc.readyTime;
        }
        if (procData[proc.pId].firstTun == -1)procData[proc.pId].firstTun = cTime;
        if (proc.insType[proc.insIdx] == 0)
        {
            proc.insIdx++;
            cTime++;
            proc.readyTime = cTime;
            if (proc.insIdx != proc.numberOfInstructions)
                app.push(proc);
            if (proc.insIdx == proc.numberOfInstructions)
                procData[proc.pId].completion = cTime;
            continue;
        }
        proc.insIdx++;
        cTime++;
        proc.readyTime = cTime + IOTime;
        if (proc.insIdx != proc.numberOfInstructions)
            app.push(proc);
        if (proc.insIdx == proc.numberOfInstructions)
            procData[proc.pId].completion = proc.readyTime;
    }
    print(currentProc.second, cTime, currentProc.first, 1,0);
    metaDataDisplay(procData, v.size());

}



