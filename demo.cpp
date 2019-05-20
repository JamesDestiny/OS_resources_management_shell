#include<iostream>
#include<string>
#include<string.h>
#include<algorithm>
#include<cctype>
#include<unistd.h>
#include<sstream>
#include"demo.h"
#include<vector>
#include<time.h>
#define Max 1024
using namespace std;
bool allisnum(string l){
    //判断字符串是否全为数字
    for(int i=0;i<l.size();i++){
        int tmp=(int)l[i];
        if(tmp>=48 && tmp <=57){
            continue;
        }
        else
        {
            return false;
        }
        
    }
    return true;
}
bool cr_ornot(string l,string *a){
    //判断输入是否为操作创建进程的三目运算符,是返回1，否返回0
    istringstream iss;
    string s;
    iss.str(l);//将string类型的l复制给字符串流
    int i=0;
    while(iss >> s){//将输入流变成一个字符串数组
        a[i]=s;
        i++;
    }
    //如果满足cr操作，则字符串个数为3，且第一个字符串为cr，最后一个字符串只能为1或2
    if(a[0]=="cr"&&i==3&&(a[2]=="1"||a[2]=="2")){
        return 1;
    }else{
        return 0;
    }
}
bool de_ornot(string l,string *a){
    istringstream iss;
    string s;
    iss.str(l);
    int i=0;
    while(iss >> s){
        a[i]=s;
        i++;
    }
    //如果满足de操作，则字符串个数为2，且第一个字符串为de，最后一个为进程名字所以无所谓
    if(i==2&&a[0]=="de"){
        return 1;
    }else
    {
        return 0;
    }
}
bool req_ornot(string l,string *a){
    istringstream iss;
    string s;
    iss.str(l);
    //string a[50];
    int i=0;
    while(iss >> s){
        a[i]=s;
        i++;
    }
    //如果满足req操作，则字符串个数为3，且第一个字符串为req，最后一个字符串必须全为数字
    if(i==3&&a[0]=="req"&&allisnum(a[2])){
        return 1;
    }else
    {
        return 0;
    }
}
bool rel_ornot(string l,string *a){
    istringstream iss;
    string s;
    iss.str(l);
    int i=0;
    while(iss >> s){
        a[i]=s;
        i++;
    }
    //如果满足rel操作，则字符串个数为3，且第一个字符串为rel，最后一个字符串必须全为数字
    if(i==3&&a[0]=="rel"&&allisnum(a[2])){
        return 1;
    }else{
        return 0;
    }
}
bool show_ornot(string l,string *a){
    istringstream iss;
    string s;
    iss.str(l);
    int i=0;
    while(iss >> s){
        a[i]=s;
        i++;
    }
    //如果满足show操作，则字符串个数为2，且第一个字符串为show，最后一个字符串无要求，因为是进程名字
    if(i==2&&a[0]=="show"){
        return 1;
    }else{
        return 0;
    }
}
int main(){
    vector<string> history;
    Process_resource_management prm;
    prm.init();
    sleep(2);
    std::cout<<"            init succeed"<<endl;
    std::cout<<"            Current_running_process:   "+prm.pcb[prm.current_running].pid<<" "<<std::endl;
    for(;;){//for循环实现shell功能入口
        char input[Max];//定义用户输入的字符串缓冲区
        string in="";
        string a_req[3];//存储判断指令返回后的字符串，如第一个为资源或者进程的名字，第二个为资源数量或者进程优先级
        string a_rel[3];
        string a_cr[3];
        string a_de[2];
        string a_show[2];
        cout<<"<Test-Shell># ";//设置shell初始化显示
        fflush(stdout);//刷新cout打出的命令
        std::cin.getline(input,50);
        for(int i=0;i<strlen(input);i++){
            //将输入字符串赋值给in，单用in负责匹配单目运算符
            in+=input[i];
        }
        transform(in.begin(),in.end(),in.begin(),::tolower);
        history.push_back(in);//将命令存在历史命令的动态字符串数组中
        if(in=="quit"){//判断是否为退出条件，输入quit时退出shell
            return 0;
        }else if (in=="init")//此时输入为初始化命令init
        {
            /* code */
            prm.init();
            std::cout<<"          init succeed"<<endl;
            std::cout<<"          Current_running_process:   "+prm.pcb[prm.current_running].pid<<" "<<std::endl;
        }
        else if (cr_ornot(in,a_cr))
        {
            /* code */
            int priority;
            stringstream ss;
            ss<<a_cr[2];
            ss>>priority;
            prm.create(a_cr[1],priority);
            std::cout<<"          create process succeed"<<endl;
            std::cout<<"          Current_running_process:    "+prm.pcb[prm.current_running].pid<<" "<<std::endl;
        }
        else if (de_ornot(in,a_de))
        {
            /* code */
            int t = prm.contain(a_de[1]);
            if(t==-1){
                std::cout<<"       wrong opreators(process not existed)"<<" ";
            }else
            {
            prm.destory(t);
            std::cout<<"          destory process succeed"<<endl;
            std::cout<<"          Current_running_process:    "+prm.pcb[prm.current_running].pid<<" "<<std::endl;
            }
        }
        else if (in=="to")
        {
            prm.timeout();
            std::cout<<"          timeout succeed"<<endl;
            std::cout<<"          Current_running_process:    "+prm.pcb[prm.current_running].pid<<" "<<std::endl;
        }
        else if (req_ornot(in,a_req))
        {
            /* code */
            int unit=0;
            string name =a_req[1];
            stringstream ss;
            ss<<a_req[2];
            ss>>unit;
            if((name =="r1")&& unit ==1){
                if(prm.rcb[0].remaining>=unit){
                prm.request_resource(0,unit);
                std::cout<<"        requset resources succeed"<<endl;
                std::cout<<"        Current_running_process:    "+prm.pcb[prm.current_running].pid<<" "<<std::endl;}
                else
                {
                    std::cout<<"        error (invalid requesets: the request number of resources is more than remains"<<endl;
                }
                
            }else if((name =="r2") && (0<unit && unit <=2)){
                if(prm.rcb[1].remaining>=unit){
                prm.request_resource(1,unit);
                std::cout<<"        requset resources succeed"<<endl;
                std::cout<<"        Current_running_process:    "+prm.pcb[prm.current_running].pid<<" "<<endl;}
                else
                {
                    std::cout<<"        error (invalid requesets: the request number of resources is more than remains"<<endl;
                }
            }else if ((name=="r3") && (unit>0 && unit<=3)){
                if(prm.rcb[2].remaining>=unit){
                prm.request_resource(2,unit);
                std::cout<<"        requset resources succeed"<<endl;
                std::cout<<"        Current_running_process:    "+prm.pcb[prm.current_running].pid<<" "<<endl;}
                else
                {
                    std::cout<<"        error (invalid requesets: the request number of resources is more than remains"<<endl;
                }
                
            }else if ((name =="r4") && (unit >0 && unit<=4))
            {
                if(prm.rcb[3].remaining>=unit){
                prm.request_resource(3,unit);
                std::cout<<"        requset resources succeed"<<endl;
                std::cout<<"        Current_running_process:    "+prm.pcb[prm.current_running].pid<<" "<<endl;}
                else
                {
                    std::cout<<"        error (invalid requesets: the request number of resources is more than remains"<<endl;
                }
                
            }else
            {
                std::cout<<"        error (invalid requests: less opreator or more opreator)"<<" "<<endl;
            }
        }
        else if (rel_ornot(in,a_rel))
        {   
            int unit=0;
            string name =a_rel[1];
            stringstream ss;
            ss<<a_rel[2];
            ss>>unit;
            if((name=="r1")&& unit ==1){
                if(prm.pcb[prm.current_running].other_resources[0].used>=unit){
                    prm.release_resource(0,unit);
                    std::cout<<"        release resources succeed"<<endl;
                    std::cout<<"        Current_running_process:    "+prm.pcb[prm.current_running].pid<<" "<<endl;
                }else
                {
                    std::cout<<"    error (invalid release: the relese number of resources is more than used)"<<" "<<endl;;
                }
            }else if((name=="r2") && (unit>0 && unit <=2)){
                if(prm.pcb[prm.current_running].other_resources[1].used>=unit){
                    prm.release_resource(1,unit);
                    std::cout<<"        release resources succeed"<<endl;
                    std::cout<<"        Current_running_process:    "+prm.pcb[prm.current_running].pid<<" "<<endl;
                }else{
                    std::cout<<"        error (invalid release: the relese number of resources is more than used)"<<" "<<endl;
                }
            }else if((name=="r3")&& (unit>0 && unit<=3)){
                if(prm.pcb[prm.current_running].other_resources[2].used>=unit){
                    prm.release_resource(2,unit);
                    std::cout<<"        release resources succeed"<<endl;
                    std::cout<<"        Current_running_process:    "+prm.pcb[prm.current_running].pid<<" "<<endl;
                }else{
                    std::cout<<"        error (invalid release: the relese number of resources is more than used)"<<" "<<endl;
                }
            }else if((name=="r4") && (unit>0 && unit<=4)){
                if(prm.pcb[prm.current_running].other_resources[3].used>=unit){
                    prm.release_resource(3,unit);
                    std::cout<<"        release resources succeed"<<endl;
                    std::cout<<"        Current_running_process:    "+prm.pcb[prm.current_running].pid<<" "<<endl;
                }else{
                    std::cout<<"        error (invalid release: the relese number of resources is more than used)"<<" "<<endl;
                }
            }else
            {
                std::cout<<"          error (invalid release: less opreator or more opreator)"<<" "<<endl;
            }
        }
        else if (in=="help")//输入为help，显示shell帮助信息
        {
            std::cout<<"-------------------Computer-OS-Test-Shell-Help-Information-------------------"<<endl;
            std::cout<<"          -init //init resources and process"<<endl;
            std::cout<<"          -cr <name> <priority>(=1 or 2) //create process"<<endl;
            std::cout<<"          -de <name> //delete process"<<endl;
            std::cout<<"          -req <resource name>(=R1 or R2 or R3 or R4) <# of units> //request resources"<<endl;
            std::cout<<"          -rel <resource name>(=R1 or R2 or R3 or R4) <# of units> //release resources"<<endl;
            std::cout<<"          -to //timeout"<<endl;
            std::cout<<"          -history //display all history input"<<endl;
            std::cout<<"          -list allp //dispaly all process and their status"<<endl;
            std::cout<<"          -list allr //display all resources and their status"<<endl;
            std::cout<<"          -list pcb  //display all processing control blocks"<<endl;
            std::cout<<"          -show <name> //display the process what is given provess`s Name "<<endl;
            std::cout<<"          -quit //close the test shell and exit"<<endl;
        }
        else if(in=="history")
        {
            std::cout<<"          ----history-input        "<<endl;
            for(int i=0;i<history.size();i++){
                std::cout<<"          "+history[i]<<endl;
            }
        }
        else if(in=="list allp"){
            std::cout<<"          ------All Process Include: "<<endl;
            prm.list_all_process();
        }
        else if (in=="list allr")
        {
            std::cout<<"          ------All Resources Include: "<<endl;
            prm.list_all_resources();
        }
        else if (show_ornot(in,a_show))
        {
            if(prm.contain(a_show[1])!=-1){
            std:cout<<"           Process <"+a_show[1]+"> Informations And Its Status:"<<endl;
            prm.list_all_process(a_show[1]);
            }
            else
            {
                std::cout<<"      Process <"+a_show[1]+"> Not Existed"<<std::endl;
            }
            
        }
        else if(in=="list pcb"){
            std::cout<<"        -------PCB--------"<<endl;
            prm.list_PCB();
        }
        else
        { 
            std::cout<<"          Wrong input!\n            Please input 'help' to get more information about the test shell"<<endl;
        }
        }
}
