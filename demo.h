#include<string>
#include<iostream>
#include<sstream>
#include<vector>
#include<cstring>
#include<queue>
#include<list>
#include<sstream>
#define process_max_number 30
//定义资源抽象数据类型
struct resource
{
    int rid=-1;//设置该资源类型的资源号
    int used=0;//设置该资源是否被使用
    int wait_request =0;//资源等待请求数
};
//定义资源控制块抽象数据类型
struct RCB
{
    std::string rid="Ri";//设置资源类型名
    int initial = 0;//设置初始化资源数量
    int remaining = 0;//设置该类型资源剩余数量
    std::list<int> wait_list;//设置因申请该资源阻塞的进程号列表
};


//定义进程控制块抽象数据类型
struct PCB{
    std::string pid = " ";
    std::string status = "ready";
    int id = -1;//初始化进程id
    int parent = -1;//设置该进程的父进程id
    int children = -1;//设置该进程的子进程id
    int younger =-1;
    int older = -1;
    int priority = -1;//设置进程优先级，1：用户优先级，2：系统优先级
    resource other_resources[4];//设置4类资源该进程分别拥有的个数
};

//定义进程资源调度类
class Process_resource_management
{
private:
    RCB R1;
    RCB R3;
    RCB R4;
    RCB R2;
    std::list<int> RL[3];
    int delete_number = -1;
public:
//定义进程资源调度类
    Process_resource_management();
        //定义进程资源调度类/* args */);
    ~Process_resource_management();
    int current_running;//定义此时正在运行的进程ip，如果无进程运行，则显示为0，即运行init
    PCB pcb[process_max_number];//定义最大进程数量为Process_max_number
    RCB rcb[4];//定义实验中给出的限定的4类资源
    void init();//系统初始化函数
    void create(std::string pname,int p);//定义创建进程函数，参数：进程名和优先级
    int scheduler();//调度程序
    void destory(int PID);//根据进程id号删除进程
    int contain(std::string pname);//根据进程名返回进程id号
    int resource_remain(int RID);//返回指定类型号资源的数量
    std::string process(int PID);//返回指定进程号的状态
    void request_resource(int RID,int unit);//情况一:当一类资源数量只有一个时
    void request_resources(int RID,int unit);//情况二：当一类资源有多个时。资源类型号以及申请的数量
    void release_resource(int RID,int unit);//情况一：当一类资源数量只有1个时
    void release_resources(int RID,int unit);//情况二：当一类资源有多个时。资源类型号以及释放的数量
    void list_all_process(std::string pname);//显示所有进程及其状态
    void list_all_resources();//显示给定的4类资源及其详细数量
    void timeout();
    void list_PCB();//显示已分配PCB的数据结构
};
//定义进程资源调度类
Process_resource_management::Process_resource_management(/* args */)
{
    for(int i=0;i<process_max_number;i++){
        pcb[i].id=i;
    }
    pcb[0].pid="init";
    pcb[0].priority=0;
    RL[0].push_back(0);//优先级为0的就绪队列插入进程id为0的init进程
    R1.rid="R1";
    R1.initial=1;
    R1.remaining=1;
    R2.rid="R2";
    R2.initial=2;
    R2.remaining=2;
    R3.rid="R3";
    R3.initial=3;
    R3.remaining=3;
    R4.rid="R4";
    R4.initial=4;
    R4.remaining=4;
    rcb[0]=R1;
    rcb[1]=R2;
    rcb[2]=R3;
    rcb[3]=R4;
    current_running=0;
    
}
void Process_resource_management::list_PCB(){
    for(int i=0;i<process_max_number;i++){
        std::string a=" ";
        std::stringstream ss;
        ss<<pcb[i].id;
        ss>>a;
        std::cout<<"      PCB_ID: "+a+"   PCB_Pname:"+pcb[i].pid<<std::endl;
        std::cout<<std::endl;
    }
}
void Process_resource_management::list_all_resources(){
    for(int i=0;i<4;i++){
        std::string remainings=" ";
        std::string initals=" ";
        std::stringstream ss;
        std::stringstream ssr;
        ss<<rcb[i].remaining;
        ss>>remainings;
        ssr<<rcb[i].initial;
        ssr>>initals;
        std::cout<<"     ResourceName: "+rcb[i].rid+"     ResourcesRemaining: "+remainings<<std::endl;
        std::cout<<"     ResourceInital: "+initals<<" "<<std::endl;
        std::cout<<"\n";
    }
}
void Process_resource_management::list_all_process(std::string pname=" "){
    if(pname==" "){
        for(int i=0;i<process_max_number;i++){
            if(pcb[i].pid!=" "){
                std::string ids=" ";
                std::string parents=" ";
                std::string prioritys=" ";
                std::stringstream ss;
                std::stringstream ssr;
                std::stringstream ssrr;
                ss<<pcb[i].id;
                ss>>ids;
                ssr<<pcb[i].parent;
                ssr>>parents;
                ssrr<<pcb[i].priority;
                ssrr>>prioritys;
                std::string a[4];
                for(int j=0;j<4;j++){
                    std::stringstream sss;
                    sss<<pcb[i].other_resources[j].used;
                    sss>>a[j];
                }
                std::cout<<"     ProcessName: "+pcb[i].pid+"     ProcessID: "+ids+"     ProcessStatus: "+pcb[i].status<<std::endl;;
                std::cout<<"     ProcessParentID: "+parents+"     Priority: "+prioritys<<std::endl;
                std::cout<<"     SourceUsed:  R1:"+a[0]+"  R2:"+a[1]+"  R3:"+a[2]+"  R4:"+a[3]<<std::endl;
                std::cout<<"\n";
            }
        }
    }
    else
    {
        for(int i=0;i<process_max_number;i++){
            if(pcb[i].pid==pname){
                std::string ids=" ";
                std::string parents=" ";
                std::string prioritys=" ";
                std::stringstream ss;
                std::stringstream ssr;
                std::stringstream ssrr;
                ss<<pcb[i].id;
                ss>>ids;
                ssr<<pcb[i].parent;
                ssr>>parents;
                ssrr<<pcb[i].priority;
                ssrr>>prioritys;
                std::string a[4];
                for(int j=0;j<4;j++){
                    std::stringstream sss;
                    sss<<pcb[i].other_resources[j].used;
                    sss>>a[j];
                }
                std::cout<<"     ProcessName: "+pcb[i].pid+"     ProcessID: "+ids+"     ProcessStatus: "+pcb[i].status<<std::endl;;
                std::cout<<"     ProcessParentID: "+parents+"     Priority: "+prioritys<<std::endl;
                std::cout<<"     Sourceused:  R1:"+a[0]+"  R2:"+a[1]+"  R3:"+a[2]+"  R4:"+a[3]<<std::endl;
                std::cout<<"\n";
            }
        }
    }
    
}

int Process_resource_management::contain(std::string pname){
    for(int i=0;i<process_max_number;i++){
        if(pname.compare(pcb[i].pid)==0){
            return i;
        }
    }
    return -1;
}


//定义进程资源调度类
Process_resource_management::~Process_resource_management()
{
}
void Process_resource_management::create(std::string pname,int p){
    for(int i=1;i<process_max_number;i++){
        if(pcb[i].pid==" "){
            pcb[i].pid=pname;
            pcb[i].priority=p;
            RL[p].push_back(pcb[i].id);
            pcb[i].parent=current_running;
            for(int j=1;j<process_max_number;j++){
                if(j<i && pcb[j].parent==pcb[i].parent)
                pcb[j].younger=i;
                pcb[i].older=j;
            }
            break;
        }
    }
    scheduler();
}
void Process_resource_management::request_resource(int RID,int unit){
    if(rcb[RID].remaining>=unit){
        rcb[RID].remaining=rcb[RID].remaining-unit;
        pcb[current_running].other_resources[RID].rid=RID;
        pcb[current_running].other_resources[RID].used+=unit;
    }else
    {
        pcb[current_running].status="blocked";
        pcb[current_running].other_resources[RID].wait_request +=unit;
        rcb[RID].wait_list.push_back(current_running);
        RL[pcb[current_running].priority].remove(current_running);
    }
    scheduler();
}
void Process_resource_management::release_resources(int RID,int unit){
    pcb[current_running].other_resources[RID].used -=unit;
    rcb[RID].remaining+=unit;
    int temp_pcb=rcb[RID].wait_list.front();
    while(temp_pcb!=0 && pcb[temp_pcb].other_resources[RID].wait_request<=rcb[RID].remaining){
        rcb[RID].remaining-=pcb[temp_pcb].other_resources[RID].wait_request;
        rcb[RID].wait_list.remove(temp_pcb);
        pcb[temp_pcb].status="ready";
        pcb[temp_pcb].other_resources[RID].used+=pcb[temp_pcb].other_resources[RID].wait_request;
        RL[pcb[temp_pcb].priority].push_back(temp_pcb);
        temp_pcb = rcb[RID].wait_list.front();
    }
}
void Process_resource_management::timeout(){
    int p = pcb[current_running].priority;
    RL[p].remove(current_running);
    pcb[current_running].status="ready";
    RL[p].push_back(current_running);
    scheduler();
}
int Process_resource_management::scheduler(){
    if(!RL[2].empty()){
        current_running=RL[2].front();
        pcb[current_running].status="running";
        std::list<int>::iterator it=RL[1].begin();
        for(int i=0;i<RL[1].size();i++){
            pcb[*it].status="reday";
        }
        pcb[0].status="ready";
        return RL[2].front();
    }else if (!RL[1].empty()){
        current_running=RL[1].front();
        pcb[current_running].status="running";
        pcb[0].status="ready";
        return RL[1].front();
    }else if(!RL[0].empty()){
        current_running=0;
        pcb[current_running].status="running";
        return 0;
    }else{
    return 0;  
}
}
void Process_resource_management::release_resource(int RID,int unit ){
        release_resources(RID,unit);
        scheduler();
}
void Process_resource_management::destory(int PID){
    for(int i=0;i<4;i++){
        if(pcb[PID].other_resources[i].used!=0){
            if(PID==current_running){
            release_resource(i,pcb[PID].other_resources[i].used);}
            rcb[i].remaining+=pcb[PID].other_resources[i].used;
            if(rcb[i].remaining>rcb[i].initial){
                std::cout<<"          error in destory process:initial resources "<<std::endl;
            }
            pcb[PID].other_resources[i].rid=-1;
            pcb[PID].other_resources[i].used=0;
        }
    }
    if(pcb[PID].status=="ready" or pcb[PID].status=="running"){
        int p = pcb[PID].priority;
        RL[p].remove(PID);
    }else if((pcb[PID].status).compare("blocked")==0){
        for(int i=0;i<4;i++){
            rcb[i].wait_list.remove(PID);
        }
    }
    for(int i=0;i<process_max_number;i++){
        if(pcb[i].parent==PID){
            destory(pcb[i].id);
        }
        if(pcb[i].id==PID){
            pcb[i].pid=" ";
            pcb[i].status="ready";
            pcb[i].parent = -1;
            pcb[i].children=-1;
            pcb[i].younger=-1;
            pcb[i].older=-1;
            pcb[i].priority=-1;
        for(int j=0;j<4;j++){
            pcb[i].other_resources[j].rid=-1;
            pcb[i].other_resources[j].wait_request=0;
        }
        }
        if(pcb[i].older==PID){
            pcb[i].older=-1;
        }
        if(pcb[i].younger==PID){
            pcb[i].younger=-1;
        }
    }
    scheduler();
}

//实现初始化进程控制块以及资源
void Process_resource_management::init(){
    for(int i=1;i<process_max_number;i++){
        pcb[i].pid=" ";
        pcb[i].status="ready";//初始化进程状态为就绪状态
        pcb[i].parent=-1;
        pcb[i].children=-1;
        pcb[i].older=-1;
        pcb[i].younger=-1;
        pcb[i].priority=-1;
        for(int j=0;j<4;j++){
            pcb[i].other_resources[j].rid=-1;
            pcb[i].other_resources[j].wait_request=0;
            pcb[i].other_resources[j].used=0;
        }
    }
    current_running=0;
    for(int i=0;i<4;i++){
        while(!rcb[i].wait_list.empty()){
            rcb[i].wait_list.pop_front();
        }
    }
    for(int i=0;i<3;i++){
        while (!RL[i].empty())
        {
            RL[i].pop_back();
        }
        
    }
    pcb[0].pid="init";
    pcb[0].priority=0;
    RL[0].push_back(0);//优先级为0的就绪队列插入进程id为0的init进程
    R1.rid="R1";
    R1.initial=1;
    R1.remaining=1;
    R2.rid="R2";
    R2.initial=2;
    R2.remaining=2;
    R3.rid="R3";
    R3.initial=3;
    R3.remaining=3;
    R4.rid="R4";
    R4.initial=4;
    R4.remaining=4;
    rcb[0]=R1;
    rcb[1]=R2;
    rcb[2]=R3;
    rcb[3]=R4;
    pcb[0].status="running";
}
