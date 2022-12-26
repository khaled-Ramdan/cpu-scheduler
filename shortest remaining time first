#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
struct Process
{
    ll readyTime, pId, numberOfInstructions, IOPercent, insIdx, executionTime;
    vector<bool>insType;//0=>cpu - 1=> io
    Process()
    {
        readyTime = 0;
        pId = 0;
        numberOfInstructions = 0;
        IOPercent = 0;
        insIdx = 0;
        executionTime = 0;
    };
    Process(ll id, ll instructionCount, ll iopercent, ll arrivalTime)
    {
        pId = id;
        numberOfInstructions = instructionCount;
        IOPercent = iopercent;
        readyTime = arrivalTime;
        insIdx = 0;
        insType.assign(numberOfInstructions, 0);
        randomizeIO();
    }
    void randomizeIO()
    {
        ll ioCount = IOPercent * numberOfInstructions / 100;
        for (int i = 0; i < ioCount; i++)
            insType[i] = 1;
        executionTime = numberOfInstructions - ioCount;
        random_shuffle(insType.begin(), insType.end());
    }
    bool operator< (const Process& p) const
    {
        if (p.readyTime == this->readyTime)
            return p.executionTime < this->executionTime;
        else
            return p.readyTime < this->readyTime;
    }
};

struct processMetaData
{
    ll  arrival = -1, completion = -1, firstTun = -1;
    ll responseTime()
    {
        return firstTun - arrival;
    }
    ll turnAroundTime()
    {
        return completion - arrival;
    }
};

void metaDataDisplay(map<ll, processMetaData>procData, ll numberOfProcesses)
{
    ll totalTurnAround = 0, totalResponse = 0;
    for (auto& i : procData)
    {
        cout << "process with pid = " << i.first << "\n";
        cout << "turn around time = " << " " << i.second.turnAroundTime() << " "
            << "Response Time = " << i.second.responseTime() << "\n";
        totalTurnAround += i.second.turnAroundTime();
        totalResponse += i.second.responseTime();
    }
    cout << "\nAverage TurnAround Time = " << totalTurnAround / numberOfProcesses << " "
        << "Average Response Time = " << totalResponse / numberOfProcesses << "\n";
}
void print(ll st, ll en, ll pid, bool fin = 0)
{
    if (~pid)
        cout << "process with id " << pid << " from " << st << " to " << en << "\n";
    else
        cout << "free cpu from " << st << " " << en << "\n";
    if (fin)
        cout << "Process is treminated....\n";
}

void shortestRemainingTimeFirst(vector<Process>& v, ll IOTime)
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
            print(currentProc.second, cTime, currentProc.first, procData[currentProc.first].completion != -1);
            currentProc = make_pair(proc.pId, proc.readyTime);
        }
        if (proc.readyTime > cTime)
        {
            print(cTime, proc.readyTime - 1,  -1);
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
    print(currentProc.second, cTime, currentProc.first, 1);
    metaDataDisplay(procData, v.size());

}







int main()
{
    srand(time(0));
    int noProcess, IOTime, InsExeTime;

    cin >> noProcess >> IOTime >> InsExeTime;
    vector<Process>v(noProcess);
    for (int i = 0; i < noProcess; i++)
    {
        int pid, insCount, IOper, startTime;
        cin >> pid >> insCount >> IOper >> startTime;
        v[i] = Process(pid, insCount, IOper, startTime);
    }
    shortestRemainingTimeFirst(v, IOTime);
    return 0;
}
