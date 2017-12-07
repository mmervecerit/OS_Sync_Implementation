#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <queue>
#include <limits>
#include <cstring>

using namespace std;
long TIME=0; //the global time, it is the time in the output file
long QUANTUM=100; //quantum can be changed
int howmanyprocesses=0; //it is the track of the processes we are dealing with
int completed=0; //if a process is completed we will increment this by one
struct Process{
    int id;
    string name;
    string code_file;
    long arrival_time;
    int last_executed_line;

};
//A Process includes its id, name, codefile,arrival time and last executed line. ID, Name, codefile and arrival time is taken from definition file, last executed line is
//related with the content of codefiles, it is changed during the scheduling.
struct Semaphor{
    int id;
    queue<Process> Wait_Queue;
    bool lock; //locked=1, unlocked=0
    ofstream outfile;
    string semaphor_output_filestring="";
    string id_s;
    Process currently_used_process;
};
struct Semaphor Semaphors[10];
queue<Process> Processes; //the queue of all processes, arrivals
queue<Process> Ready_Queue; //this is the queue of ready processes
void Initialize_Semaphors(){
    for(int i = 0 ; i<10 ; i++){
        Semaphors[i].id=i;
        Semaphors[i].id_s=i + '0';
        while(!Semaphors[i].Wait_Queue.empty()){
            Semaphors[i].Wait_Queue.pop();
        }
        Semaphors[i].lock=0;
        Semaphors[i].semaphor_output_filestring = "output_" +Semaphors[i].id_s +".txt";
        Semaphors[i].outfile.open(Semaphors[i].semaphor_output_filestring);
    }
}

void update_outputfile_wait(ofstream* out,int semaphor_id,long durationtemp,int mul){
    string tire = "-";
    string waitqueue;
    if(!Semaphors[semaphor_id].Wait_Queue.empty()){
        queue<Process> copy_queue = Semaphors[semaphor_id].Wait_Queue;
        waitqueue= copy_queue.front().name;
        copy_queue.pop();
        while(!copy_queue.empty()){
            waitqueue.append(tire);
            waitqueue.append(copy_queue.front().name);
            copy_queue.pop();
        }
        *out<<TIME+durationtemp*mul<<"::HEAD-"<<waitqueue<<"-TAIL"<<endl;}
    else{
        *out<<TIME+durationtemp*mul<<"::HEAD--TAIL"<<endl;
    }
}
//This function outputs the TIME and ready queue. When a change occurs in the ready queue, we are calling this function.
void update_outputfile(ofstream* out,long durationtemp,int mul){
    string tire = "-";
    string ready_queue;
    if(!Ready_Queue.empty()){
        queue<Process> copy_queue = Ready_Queue;
        ready_queue= copy_queue.front().name;
        copy_queue.pop();
        while(!copy_queue.empty()){
            ready_queue.append(tire);
            ready_queue.append(copy_queue.front().name);
            copy_queue.pop();
        }
        *out<<TIME+durationtemp*mul<<"::HEAD-"<<ready_queue<<"-TAIL"<<endl;}
    else{
        *out<<TIME+durationtemp*mul<<"::HEAD--TAIL"<<endl;
    }
}
bool WaitS(int semaphor_id, Process* currentprocess,ofstream* out,long duration_temp){
    if(Semaphors[semaphor_id].lock==1){
        Semaphors[semaphor_id].Wait_Queue.push(*currentprocess);
        update_outputfile_wait(&Semaphors[semaphor_id].outfile,semaphor_id,duration_temp,1);
        //update_outputfile(out,duration_temp,1);
        return false;
    }
    else{
        Semaphors[semaphor_id].lock=1;
        return true;
    }
}
bool SignS(int semaphor_id,Process* currentprocess,ofstream* out,long duration_temp){
    Semaphors[semaphor_id].lock=0; //unlock the corresponding semaphor
    //Ready_Queue.push(*currentprocess);
    //update_outputfile(out,duration_temp,0);
    if(!Semaphors[semaphor_id].Wait_Queue.empty()){
        Semaphors[semaphor_id].lock=1;
        Process p = Semaphors[semaphor_id].Wait_Queue.front();
        Ready_Queue.push(p);
        //update_outputfile(out,duration_temp,0);
        Semaphors[semaphor_id].Wait_Queue.pop();
        update_outputfile_wait(&Semaphors[semaphor_id].outfile,semaphor_id,duration_temp,1);
        return false;
    }
    return true;
    //semaphoreun wait queuesunu check et, varsa al. burda biteni de ready queue ya at.
}
//This function takes the file and the line number as number and goes to that line of file. I took this function from stackoverflow.
fstream& GotoLine(fstream& file, unsigned int num){
    file.seekg(ios::beg);
    for(int i=0; i < num - 1; ++i){
        file.ignore(numeric_limits<streamsize>::max(),'\n');
    }
    return file;
}
int main() {
    ifstream infile;
    ofstream outfile;
    infile.open("../definition.txt"); //the code assumes that the definition file will be named as definition.txt always and it will be placed to the same place with source code.
    outfile.open("../output.txt"); //output file is named as output.txt and it is placed to the same place with source code.
    string a,b;
    long c;
    if (infile.is_open()){
        int iterator=1;
        while(infile >> a >> b >> c){
            Process x;
            x.id=iterator;
            x.name = a;
            x.code_file=b;
            x.arrival_time=c;
            x.last_executed_line=0;
            iterator++;
            Processes.push(x);
            howmanyprocesses++;
        }
    } //from definition.txt file, we are reading the process details and creating the Process instances.
    else{cout<<"error while opening the file, make sure that you are in the right directory"<<endl;}//if smt off occurs with definition.txt file, this error will be printed.
    Initialize_Semaphors();
// assuming the first process(P1,first in the definition.txt) will arrive earliest
    TIME = Processes.front().arrival_time;
    Process front=Processes.front();
    Ready_Queue.push(front);
    update_outputfile(&outfile,0,0);
    Processes.pop();
//before going into while loop, we are initializing the ready queue by putting the first process into it. We are popping it from Processes queue since it will be either in the ready queue or completed.
    while(completed!=howmanyprocesses){
//this while loop also works when ready queue is empty.
        string instruction_name;
        long duration=0;
        long duration_temp=0;
        int line_offset=0;
        string codefile_directory="";
        Process current_process;

        if(Ready_Queue.empty()){
//if ready queue is empty, we are jumping to next arrival.
            TIME = Processes.front().arrival_time;
            Process front= Processes.front();
            Ready_Queue.push(front);
            Processes.pop();
            current_process = Ready_Queue.front();
            codefile_directory="../"+current_process.code_file;
            Ready_Queue.pop();

        }
//else we are going to current process's codefile and we pop the process from ready queue.
//If it will be completed, we are done with it we wont put it back. But if it is not completed, we will put it to ready queue again.
        else{
            current_process = Ready_Queue.front();
            codefile_directory="../"+current_process.code_file;
            Ready_Queue.pop();
        }
        fstream codefile;
        codefile.open(codefile_directory,fstream::in);
        GotoLine(codefile, static_cast<unsigned int>((current_process.last_executed_line) + 1));
//if quantum is not exceeded we go into while loop, since the instructions are atomic we dont halt the process if quantum is exceeded. If the previous instruction is exit, we are not go into while loop.
        string wait = "wait";
        string sign = "sign";
        char myArray[8];
        char wait_arr[wait.length()+1];;
        strcpy(wait_arr, wait.c_str());
        char sign_arr[sign.length()+1];;
        strcpy(sign_arr, sign.c_str());
        while(duration_temp<QUANTUM && instruction_name!="exit"){
            codefile>>instruction_name>>duration;
            strcpy(myArray, instruction_name.c_str());
            line_offset++;
            duration_temp+=duration;
            if(strstr(myArray,wait_arr)){
                unsigned long last= instruction_name.length()-1;
                char which_semaphor_char = instruction_name[last];
                int which_semaphor_int = which_semaphor_char - '0';
                bool check = WaitS(which_semaphor_int,&current_process,&outfile,duration_temp);
                if(!check){
                    break;
                }
            }
            else if(strstr(myArray,sign_arr)){
                unsigned long last= instruction_name.length()-1;
                char which_semaphor_char = instruction_name[last];
                int which_semaphor_int = which_semaphor_char - '0';
                SignS(which_semaphor_int,&current_process,&outfile,duration_temp);}
        }
        current_process.last_executed_line += line_offset;//we should record where we left.
        TIME+=duration_temp;//TIME is updated.
        if(!Processes.empty() && TIME>=Processes.front().arrival_time){
//if there are processes not arrived and their arrival time is past while we are processing the previous process, we should put it into ready queue.
            Process front=Processes.front();
            Ready_Queue.push(front);
            Processes.pop();
        }
//if the previous instruction is not exit, the process is not completed.we should put it into ready queue and update the output file.
        if(instruction_name!="exit" && !strstr(myArray,wait_arr) &&  !strstr(myArray,sign_arr)){
            Ready_Queue.push(current_process);
            update_outputfile(&outfile,0,0);
        }
//if it is exit, then it is completed we are done with that process, we will increment completed by 1  and update the output file.
        else{update_outputfile(&outfile,0,0);
            completed++;}
    }
//until all processes are completed, we are continueing to do the same thing. if they are all completed, we're done.
    infile.close();
    outfile.close();
    return 0;
}
