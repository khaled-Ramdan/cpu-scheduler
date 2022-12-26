struct SJFcompare
{
    bool operator() (const Process& p, const Process& t) const
    {
        if (p.readyTime == t.readyTime)
            return p.executionTime > t.executionTime;
        else
            return p.readyTime > t.readyTime;
    }
};



void SJF(const vector<Process>& v, ll insTime, ll ioTime){
    priority_queue<Process, vector<Process>, SJFcompare>APP;
    map<ll, processMetaData>procData;
    
    for (Process& i : v)
    {
        APP.push(i);
        procData[i.pId].arrival = i.readyTime;
    }
    
       
       while (APP.size())
    {
        Process proc = APP.top();
        APP.pop();
        
        
  
    }

}
