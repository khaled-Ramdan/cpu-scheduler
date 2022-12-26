#include"FCFS.h"

void FCFS_Scheduler(const vector<Process>& v, ll insTime, ll ioTime)
{
    map<ll, processMetaData>procData;
    ll mx=0;
	for (auto& i : v) {
		procData[i.pId].arrival = i.readyTime;
        mx=max(mx,i.readyTime);
        procData[i.pId].firstTun=mx;
        ll numberofioI=i.IOPercent * i.numberOfInstructions / 100;
        ll z=ioTime*numberofioI+(i.numberOfInstructions-numberofioI)*insTime;
        print(mx,mx+z,i.pId,1,0);
        mx+=z;
        procData[i.pId].completion=mx;
	}
    metaDataDisplay(procData,v.size());
}

void SJF(const vector<Process>& v, ll insTime, ll ioTime){
    priority_queue<Process, vector<Process>, SRTFcompare>APP;
    priority_queue<Process, vector<Process>, SJFcompare>q2;
    map<ll, processMetaData>procData;
    
    for (auto& i : v)
    {
        APP.push(i);
        procData[i.pId].arrival = i.readyTime;
    }
    if(v[0].readyTime)print(0,v[0].readyTime,-1,0,0);
    q2.push(v[0]);
    Process proc = APP.top();
            APP.pop();
    ll time=v[0].readyTime;
    while (APP.size()||q2.size())
    {
        if(q2.size())
        {
            proc=q2.top();
            q2.pop();
            print(time,time+proc.executionTime,proc.pId,1,0);
            procData[proc.pId].firstTun=time;
            time+=proc.executionTime;
            procData[proc.pId].completion=time;
            while(APP.size()&&time>=APP.top().readyTime)
            {        
                proc=APP.top();
                APP.pop();
                q2.push(proc);
            }
        }
        else if(APP.size())
        {
            proc=APP.top();
            APP.pop();
            print(time,proc.readyTime,-1,0,0);
            time=proc.readyTime;
            q2.push(proc);
        }                
        // cout<<time<<" "<<v.size()<<APP.size()<<'\n';
    }
    metaDataDisplay(procData,v.size());
}






