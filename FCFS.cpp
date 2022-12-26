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






