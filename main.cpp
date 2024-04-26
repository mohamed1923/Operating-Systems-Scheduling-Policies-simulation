#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <queue>
#include <vector>
#include <bits/stdc++.h>
#include <math.h>


using namespace std;

char input [100][100];
int numOfSeconds;
int *arrival_time;
int *service_time;
int *finish_time;
int *initial_priority;
float *waiting_ratio;
int *priority;
char ** traceOutput;
float *turnaround_time;
float *normturn_time;
bool output_type;
int current_alog_id;

struct CompareService{

    bool operator()(int p1,int p2){
        if(current_alog_id == 5)
        {
            return waiting_ratio[p1] < waiting_ratio[p2];
        }
        else if(current_alog_id == 8)
        {
            if(priority[p2] == priority[p1])
            {
                return false;
            }
            return priority[p1] < priority[p2]; 
        }
        else
        {
            return p2 > p1;
        }
    }
};

char ** split (char * str,char *x){

    char **temp ;
    temp = (char**)malloc(10 * sizeof(char*));

    for (int i = 0; i < 10; i++)
        temp[i] = (char*)malloc(10 * sizeof(char));
    int i=0;
    
    char *token = strtok(str, x);
    while (token != NULL)
    {
        
        strcpy(temp[i],token);
        token = strtok(NULL, x);
        i++;
        
    }
    return temp;

}


void extract_times(char processes[][100],int num_of_processes){

    arrival_time=(int*)malloc(num_of_processes*sizeof(int));
    service_time=(int*)malloc(num_of_processes*sizeof(int));
    priority = (int*)malloc(num_of_processes*sizeof(int));
    initial_priority = (int*)malloc(num_of_processes*sizeof(int));

    char ***processes_separated =(char***)malloc(num_of_processes * sizeof(char*)) ;

    for (int i = 0; i < 10; i++){
        
        processes_separated[i] = (char**)malloc(10 * sizeof(char));
    
    }
    for(int i=0;i<num_of_processes;i++){
        
        processes_separated[i]=split(processes[i],",");
    
    }
    
    for(int i=0;i<num_of_processes;i++){
    
        arrival_time[i]=atoi(processes_separated[i][1]);
        service_time[i]=atoi(processes_separated[i][2]);
    
    }
   


}

void states_calc(int num_of_processes){
    //calculate turnaround time
    float sum=0.0;
    turnaround_time=(float*)malloc((num_of_processes+1)*sizeof(float));
    for(int i=0;i<num_of_processes;i++){
    
        turnaround_time[i]=finish_time[i]-arrival_time[i];
        sum+=turnaround_time[i];    
    
    }
    turnaround_time[num_of_processes]=sum/num_of_processes; //MEAN
    
    //calculate normal turnaround time
    sum=0.0;
    normturn_time=(float*)malloc((num_of_processes+1)*sizeof(float));
    for(int i=0;i<num_of_processes;i++){
    
        normturn_time[i]=turnaround_time[i]/service_time[i];
        sum+=normturn_time[i];    
    
    }
    
    normturn_time[num_of_processes]=sum/num_of_processes; //MEAN
}

void clearTraceOutput(int num_of_processes)
{
    int process,t;
    for(process=0;process<num_of_processes;process++)
    {
        for(t=0;t<numOfSeconds;t++)
        {
            traceOutput[process][t] = ' ';
        }
    }
}


void showPQ(priority_queue<int, vector<int>, CompareService> Q)
{
    priority_queue<int, vector<int>, CompareService> newQ = Q;
    cout << "PQ------------" << endl;
    int i;
    while(!newQ.empty())
    {
        i = newQ.top();
        cout <<"Process " <<i << " Priority " << priority[i] <<endl;
        newQ.pop();
    }
    //cout << newQ.top() << endl;
    cout <<"---------------"  <<endl;
}

void updateQ(priority_queue<int, vector<int>, CompareService> *Q)
{
    vector<int> v;
    int i = 0;
    //printf("------PQ---Before Update----\n");
    //showPQ(*Q);
    while(!Q->empty())
    {
        i = Q->top();
        //printf("Process %d, Priority: %d\n",i,priority[i]);
        Q->pop();
        v.push_back(i);
    }
    //printf("---------------\n");
    //printf("------PQ After Update---------\n");
    while(!v.empty())
    {
        Q->push(v[0]);
        v.erase(v.begin());
    }   
    //showPQ(*Q);
    //printf("-------------------------------\n");
}


//Function to arrange processes depend on service time
void rearrange(int* p,int num_of_processes){
    int min;
    int i=0;
    
    for (i = 0; i < num_of_processes-1; i++)
    {   
        min = i;
         if(p[i]==-1)
        continue;
        
        for (int j = i+1; j < num_of_processes; j++){
            //Check if procces has finished
            if(p[j]==-1)
            break;
            if (service_time[p[j]] < service_time[p[min]])
            min = j;
            
        }
        if(min!=i)
            std::swap(p[min],p[i]);
    }
}


//Function to print the tabel of stats
void states(char processes[][100],int num_of_processes){
  
    //Print name of process  
    printf("Process");
    printf("    |");
    for(int i=0;i<num_of_processes;i++){
        
        printf("  %c  |",processes[i][0]);
    }

    //Print arrival time
    printf("\nArrival");
    printf("    |");
    for(int i=0;i<num_of_processes;i++){
    
        printf("%3d  |",arrival_time[i]);
    }
    
    //Print service time
    printf("\nService");
    printf("    |");
    for(int i=0;i<num_of_processes;i++){
        
        printf("%3d  |",service_time[i]);
    }
    printf(" Mean|");
    
    //Print finish time
    printf("\nFinish");
    printf("     |");
    for(int i=0;i<num_of_processes;i++){
        
        printf("%3d  |",finish_time[i]);
    }
    printf("-----|");
    
    //Print turnaround time
    printf("\nTurnaround");
    printf(" |");
    for(int i=0;i<num_of_processes+1;i++){
        if(i==num_of_processes){
            //Print mean   
            printf("%5.2f|",turnaround_time[i]);
        }else{
            
            printf("%3.0f  |",turnaround_time[i]);
        }
    }
    
    //Print normturn
    printf("\nNormTurn");
    printf("   |");
    for(int i=0;i<num_of_processes+1;i++){
        
        printf("%5.2f|",normturn_time[i]);
    }
    printf("\n");
    printf("\n");
}

void trace(char processes[][100],int num_of_processes)
{
    int i,j;
    for(i=0;i<=numOfSeconds;i++)
    {
        printf("%d ",(i%10));
    }
    printf("\n");
    
    for(i=0;i<8;i++)
    {
        printf("-");
    }
    for(i=0;i<2*numOfSeconds;i++)
    {
        printf("-");

    }
    printf("\n");
    for(i=0;i<num_of_processes;i++)
    {
        printf("%c     ",processes[i][0]);
        for(j=0;j<numOfSeconds;j++)
        {
            printf("|%c",traceOutput[i][j]);
        }
        printf("| ");
        printf("\n");
    }
    for(i=0;i<8;i++)
    {
        printf("-");
    }
    for(i=0;i<2*numOfSeconds;i++)
    {
        printf("-");

    }
    printf("\n");
    printf("\n");
}


void FCFS(int num_of_processes){

    //calculate finish time
    finish_time=(int*)malloc(num_of_processes*sizeof(int));
    finish_time[0]=service_time[0]+arrival_time[0];
    for(int i=1;i<num_of_processes;i++){
        if(arrival_time[i] > finish_time[i-1])
        {
            finish_time[i]=finish_time[i-1]+service_time[i] + (arrival_time[i]-finish_time[i-1]);
        }
        else
        {
            finish_time[i]=finish_time[i-1]+service_time[i];
        }
    }
    

    //trace part
    if(output_type)
    {
        vector<int> V;
        int i,j;
        int processOrder = 0;
        int oldProcess;
        for(i=arrival_time[processOrder];i<numOfSeconds;i++)       //loop over timeline
        {
            //traceOutput[processOrder][i] = '*';
            for(j=0;j<num_of_processes;j++) //for each process check if it's waiting
            {
                if(j != processOrder && arrival_time[j]<i+1 && finish_time[j] >= i+1 && finish_time[processOrder] >= i+1)
                {
                    traceOutput[j][i] = '.';
                }
                if( j != processOrder && arrival_time[j] == i) //if a process came push it to vector
                {
                    V.push_back(j);
                }
            }
            if(finish_time[processOrder] >= i+1 ) // If current process not finished
            {
                traceOutput[processOrder][i] = '*'; //execute process
            }
            if(finish_time[processOrder] <= i+1)  //If current process has finished 
            {
                oldProcess = processOrder;
                if(!V.empty())
                {
                    processOrder = V.front();
                    V.erase(V.begin());
                    if(finish_time[oldProcess] < i+1) //If Scheduler was IDLE
                    {
                        traceOutput[processOrder][i] = '*';
                    }
                }
            }
        }

    }

    states_calc(num_of_processes);


    }



void RR(int num_of_processes,int q){
    std::queue<int> p;
    finish_time=(int*)malloc(num_of_processes*sizeof(int));
    int t=0,x=0,a=0;
    int i;

    //make copy service time 
    int service_temp[num_of_processes];
    for(i=0;i<num_of_processes;i++){
        
        service_temp[i]=service_time[i];
    }
    //loop unit get first procces
    while(t <=arrival_time[0]){
        
        t++;
        p.push(a);
        a++;
    }
    while (t <= numOfSeconds)
    {
        int temp;
        
        //add procces to queue when it arrive
        if((arrival_time[a])==t   ){
            p.push(a);
            a++;  //increment procces index
        }
        if(!p.empty() ){
            temp=p.front();
            
            if(output_type)
            {
               
                    traceOutput[temp][t-1] = '*';

                //check for other processes in queue
                for(i=0;i<num_of_processes;i++)
                {
                    //check whether other processes are waiting or not
                    if(i != temp && arrival_time[i] < t && service_temp[i] != 0)
                    {
                        traceOutput[i][t-1] = '.';
                    }
                }
            }
            
            //decrement service time of runnong procces
            service_temp[temp]-=1;
            
            // increment 'x' counter indicate how many time slice does the procces excute
            x++; 

            //if the quantem of procces finish or the procces service time finish pop from the queue
            if(x%q ==0 ||service_temp[temp]==0 ){
            
                p.pop();
                x=0;
                //if the procces doesnot finish push it again in the queue else assign current time to be finish time
                if(service_temp[temp]!=0){
                    p.push(temp);
                }else{
                    finish_time[temp]=t;
            
                }
            }            
        }
        t++;
    }
    states_calc(num_of_processes);
    
}


void SPN(int num_of_processes){
    finish_time=(int*)malloc(num_of_processes*sizeof(int));
    
    //make copy service time 
    int service_temp[num_of_processes];
    int j;
    for(int i=0;i<num_of_processes;i++){
        service_temp[i]=service_time[i];
    }

    int t=0,a=0,min,i=0;

    //creat array of procces and assign each one to -1 until it arrive
    int p[num_of_processes];
    for(int i=0;i<num_of_processes;i++){
        p[i]=-1;
    }
  
    while (t <= numOfSeconds)
    {
        //add procces to array when it arrive 
        if((arrival_time[a])==t   ){
            p[a]=a;
            a++; 
        }

        if(output_type)
        {
            if(p[i] != -1 && service_temp[p[i]] != 0)
            {
                traceOutput[p[i]][t] = '*';

                for(j=0;j<num_of_processes;j++)
                {
                    if(p[i] != j && arrival_time[j] < t+1 && service_temp[j] != 0)
                    {
                        traceOutput[j][t] = '.';
                    }
                }
            }
        }

        //decrement service time
        service_temp[p[i]]-=1;

        //if procces finished assign current time to be finish time and assign her index in the array to -1 and rearrange the procceses to get min service time
        if(service_temp[p[i]]==0){
            finish_time[p[i]]=t+1;
            p[i]=-1;
            rearrange(p,num_of_processes);
            i++;
            if(i >= num_of_processes)
            {
                break;
            }
        }
        t++;
       
    }
    states_calc(num_of_processes);
}

void SRT(int num_of_processes){
    
    finish_time=(int*)malloc(num_of_processes*sizeof(int));

    //make copy service time
    int service_temp[num_of_processes];
    for(int i=0;i<num_of_processes;i++){
        service_temp[i]=service_time[i];
    }
    int j;
    int t=0,a=0,i=0;
    //creat array of procces and assign each one to -1 until it arrive
    int p[num_of_processes];
    for(int i=0;i<num_of_processes;i++){
        p[i]=-1;
    }
    //printf("There\n");
    while (t <= numOfSeconds)
    {
        //add procces to array when it arrive and when it arrive rearrang them to get shortest remaining time
        if((arrival_time[a])==t   ){
            p[a]=a;
            rearrange(p,num_of_processes); 
            a++;  
        }
        //if there is two procces have the same remaining time serve which come first
        
        
        if(i+1<num_of_processes && p[i]>p[i+1] && service_temp[p[i]]==service_temp[p[i+1]]){
            std::swap(p[i],p[i+1]);            
        }  
        
        if(output_type)
        {
            if(p[i] != -1 && service_temp[p[i]] != 0)
            {
                traceOutput[p[i]][t] = '*';

                //check for ready slots
                for(j=0;j<num_of_processes;j++)
                {
                    if(p[i] != j && arrival_time[j] < t+1 && service_temp[j] != 0)
                    {
                        traceOutput[j][t] = '.';
                    }
                }
            }
        }

        //decrement service time
        service_temp[p[i]]-=1;   

        //if procces finished assign current time to be finish time and assign her index in the array to -1 
        
        if(service_temp[p[i]]==0){
            finish_time[p[i]]=t+1;
            p[i]=-1;
            i++;
            if( i >= num_of_processes)
            {
                break;
            }
        }
        t++;
    }
    states_calc(num_of_processes);    
}



void HRRN(int num_of_processes){
    finish_time=(int*)malloc(num_of_processes*sizeof(int));
    int service_temp[num_of_processes];
    for(int i=0;i<num_of_processes;i++){
        service_temp[i]=service_time[i];
    }

    int waiting[num_of_processes];
    for(int i=0;i<num_of_processes;i++){
        waiting[i]=0;

    }

    
    //define new Q
    priority_queue<int, vector<int>, CompareService> PQ;

    //initialize waiting ratio array
    waiting_ratio = (float*)(malloc(sizeof(float)*num_of_processes));

    int Process,t,otherProcess,oldProcess;
    Process = 0;
    t = arrival_time[0];
    for(t=arrival_time[0];t<numOfSeconds;t++)
    {

        for(otherProcess=0;otherProcess<num_of_processes;otherProcess++)
        {
            if(Process!=otherProcess && arrival_time[otherProcess] < t+1 && service_temp[otherProcess] > 0 && service_temp[Process] >= 0)
            {
                traceOutput[otherProcess][t] = '.';   //Process is waiting
                waiting[otherProcess] += 1;

                //update ratio of waiting
                waiting_ratio[otherProcess] = (float)((waiting[otherProcess] + service_time[otherProcess]) / (float)(service_time[otherProcess]));
            }
            if(Process!=otherProcess && arrival_time[otherProcess] == t)
            {
                PQ.push(otherProcess);                
            }
        }
        
        service_temp[Process]--;
        if(service_temp[Process] >= 0) //Process still not finished
        {
            traceOutput[Process][t] = '*';  //Execute Process
        }
        
        if(service_temp[Process] == 0) //Process just completed
        {
            finish_time[Process] = t+1; //Store finish time
        }
        if(service_temp[Process] <= 0) //Process Completed just now or earlier
        {
            oldProcess = Process;
            updateQ(&PQ);     //Check for new process in Q        
            if(!PQ.empty())  
            {
                Process = PQ.top();
                PQ.pop();
                if(service_temp[oldProcess] < 0) //Scheduler was idle
                {
                    traceOutput[Process][t] = '*'; //Execute Process
                    service_temp[Process]--;
                }
            }
        }

    }
    states_calc(num_of_processes);

      
}

void FB1(int num_of_processes){
    queue<int> p[numOfSeconds];
    int queue_num=0,q=1;
    finish_time=(int*)malloc(num_of_processes*sizeof(int));
    int t=arrival_time[0],a=0;

    //make copy service time 
    int service_temp[num_of_processes];
    for(int i=0;i<num_of_processes;i++){
       
        service_temp[i]=service_time[i];
    }

   int f=0,temp,f2=0;
    while (t < numOfSeconds)
    {
        //add procces to queue when it arrive and add it to queue level 0 'higest piroity'
        if((arrival_time[a])==t){
            p[0].push(a);
            a++;  
            queue_num=0;
        }
        if(p[queue_num].empty()){
            t++;
            queue_num++;
            continue;
        }
        //f=1 when only one  process is running so it remain in the same level 
        if(p[queue_num].size()==1 && arrival_time[a]!=t+1 && p[queue_num+1].size()==0 && p[queue_num+2].size()==0){
            
            f=1;
        }else{
            f=0;
        }
       
        //decrement service time
        service_temp[p[queue_num].front()]-=q;

        temp=p[queue_num].front();
        
        if(output_type)
        {
            if(service_temp[temp] >= 0)
            {
                traceOutput[temp][t] = '*';

                for(int i=0;i<num_of_processes;i++)
                {
                    if(i!=temp && arrival_time[i]<t+1 && service_temp[i] != 0)
                    {
                        traceOutput[i][t] = '.';
                    }
                }
            }
        }

        //pop thr procces when quantem  = 1 finish
        p[queue_num].pop();

        //if procces didnot finish push it to queue deoending on f else assign current time to be finish time
        if(service_temp[temp]!=0 ){
            
           if(f==1){

                p[queue_num].push(temp); 

           }else{

                p[queue_num+1].push(temp); 

           }
        }else{
             
            finish_time[temp]=t+1;
        }
        
        //if cuurne queue is empty go to next queue "next level"
        if (p[queue_num].empty()){
            queue_num++;
        }
        t++;

    }
    states_calc(num_of_processes); 
    
}

void FB2(int num_of_processes){
    std::queue<int> p[numOfSeconds];
    int queue_num=0,q;
    finish_time=(int*)malloc(num_of_processes*sizeof(int));
    int t=0,a=0;
    
    //make copy service time 
    int service_temp[num_of_processes];
    for(int i=0;i<num_of_processes;i++){
    
        service_temp[i]=service_time[i];
    }
   
    int f=0,temp,f2=0,x=0;
    while (t < numOfSeconds)
    {
        //add procces to queue when it arrive and add it to queue level 0 'higest piroity'
        if((arrival_time[a])==t){
        
            p[0].push(a);
            a++; 
            //if there is procces running donot cut it to serve new arrival 
            if(x==0) 
                queue_num=0;
        }
        if(p[queue_num].empty()){
            t++;
            queue_num++;
            continue;
        }
        q=pow(2,queue_num);

        //f=1 when only one  process is running so it remain in the same level 
        if(p[queue_num].size()==1 && arrival_time[a]!=t+1 && p[queue_num+1].size()==0 && p[queue_num+2].size()==0){
        
            f=1;
        }else{
            
            f=0;
        }

        //decrement service time
        service_temp[p[queue_num].front()]-=1;

        // increment 'x' counter indicate how many time slice does the procces excute
        x++;
        
        temp=p[queue_num].front();

        if(output_type)
        {
            traceOutput[temp][t] = '*';

            for(int i=0;i<num_of_processes;i++)
            {
                if(i!=temp && arrival_time[i]<t+1 && service_temp[i] != 0)
                {
                    traceOutput[i][t] = '.';
                }
            }
        }

        //if procces didnot finish and finish its quantem push it to queue deoending on f else assign current time to be finish time
        if(service_temp[p[queue_num].front()]!=0 && x%q==0 ){
           if(f==1){
                p[queue_num].push(p[queue_num].front());
           }else{
                p[queue_num+1].push(p[queue_num].front()); 

           }
        }else{
             
            finish_time[p[queue_num].front()]=t+1;
        }
        
        if(x%q==0 || queue_num==0 ||service_temp[p[queue_num].front()]==0){
            
            p[queue_num].pop();
        
            x=0;
            if(p[0].size()!=0){
                queue_num=0;
            }
        }
        
        //if cuurne queue is empty go to next queue "next level"
        if (p[queue_num].empty()){
            
            queue_num++;
        }
        t++;
    }
    states_calc(num_of_processes);
    
}

void updateVect(vector<int> &v)
{
    int i;
    for (i=v.size()-1;i>0;i--)
    {
        if(priority[v[i]] > priority[v[i-1]])
        {
            swap(v[i],v[i-1]);
        }
    }
}


void Aging(int num_of_processes,int q){

    //define priority queue
    priority_queue<int, vector<int>, CompareService> PQ;
    vector<int> v;
    int i,tempQ,t;
    tempQ = q;
    
    //initialize initial priority array with service array
    for(i=0;i<sizeof(service_time);i++)
    {
        initial_priority[i] = service_time[i];
    }
    
    //copy values from initial_priority array to priority array
    for(i=0;i<sizeof(initial_priority);i++)
    {
        priority[i] = initial_priority[i];
    }


    int Process = 0;
    for(t=arrival_time[Process];t<numOfSeconds;t++)
    {
        //return old priority value for the running process
        priority[Process] = initial_priority[Process];
        traceOutput[Process][t] = '*';

        //loop over processes to check for ready state and update their priority levels
        for(i=0;i<num_of_processes;i++)
        {
            if(i!=Process && arrival_time[i] < t+1 )
            {

                //set this process as ready
                traceOutput[i][t] = '.';
                //update priority
                priority[i] += 1;
            }
            if(i!= Process && arrival_time[i] == t+1)
            {
                //if process arrived at instance t ... add to Q
                v.push_back(i);   
            }
        }
        tempQ--;
        if(tempQ == 0)
        {
            //push current process to Q
            v.push_back(Process);
            //Update Q
            updateVect(v);
            //Take new process
            Process = v.front();
            v.erase(v.begin());
            tempQ = q;
        }
    }
}


int main(void){
    
    int i,j;
    
    for(int i=0 ;i< 4;i++ ){
        fscanf(stdin,"%s",input[i]);
    }
   

    
    //get num of seconds
    numOfSeconds = atoi(input[2]);

    //get time line
    int terminat_time=atoi(input[2]);

    //get num of processes
    int num_of_processes=atoi(input[3]);
    char processes[num_of_processes][100];
    
    //define 2d array for trace output
    traceOutput = (char **)(malloc(sizeof(char*) *num_of_processes));
    //init each process trace output as empty
    for(i=0;i<num_of_processes;i++)
    {
        traceOutput[i] = (char *)(malloc(sizeof(char) *numOfSeconds));
        for(j=0;j<numOfSeconds;j++)
        {
            traceOutput[i][j] = ' ';
        }
    }

    for(int i=0 ;i<num_of_processes;i++ ){
        fscanf(stdin,"%s",processes[i]);
        
    }
    
    
   
    if(!strcmp(input[0],"trace"))
    {
        output_type=true;
    }else{
        output_type=false;
    }
    extract_times(processes,num_of_processes);
    
    char ** algorithm_id=(char**)malloc(10 * sizeof(char*)) ;
    for (int i = 0; i < 10; i++)
        algorithm_id[i] = (char*)malloc(10 * sizeof(char));

    algorithm_id=split(input[1],",");
    int q;
    for(int i=0;i<=sizeof(algorithm_id);i++){
        current_alog_id = atoi(algorithm_id[i]);
        switch (current_alog_id)
        {
        case (1):
            clearTraceOutput(num_of_processes);
            FCFS(num_of_processes);
            if(output_type){
                printf("FCFS  ");
                trace(processes,num_of_processes);
            }else{
                printf("FCFS\n");
                states(processes,num_of_processes); 
            }
            break;
        case (2):
            clearTraceOutput(num_of_processes);
            q=int(algorithm_id[i][2]-'0');
            RR(num_of_processes,q);
            if(output_type){
            printf("RR-%d  ",q);
            trace(processes,num_of_processes);
            }else{
             printf("RR-%d\n",q);
             states(processes,num_of_processes);
            }
            
            
            break;
        case (3):
            clearTraceOutput(num_of_processes);
            SPN(num_of_processes);
            if(output_type){
            printf("SPN   ");
            trace(processes,num_of_processes);

            }else
            {
            printf("SPN\n");
            states(processes,num_of_processes);

            }
            break;
        case (4):
            clearTraceOutput(num_of_processes);
            SRT(num_of_processes);
            if(output_type){
            printf("SRT   ");
            trace(processes,num_of_processes);

            }else
            {
            printf("SRT\n");
            states(processes,num_of_processes);
                
            }
            break;
        case (5):
            clearTraceOutput(num_of_processes);
            HRRN(num_of_processes);
            if(output_type){
            printf("HRRN  ");
            trace(processes,num_of_processes);

            }else
            {
            printf("HRRN\n");
            states(processes,num_of_processes);
                
            }
            break;
        case (6):
            clearTraceOutput(num_of_processes);
            FB1(num_of_processes);
            if(output_type){
            printf("FB-1  ");
            trace(processes,num_of_processes);

            }else
            {
            printf("FB-1\n");
            states(processes,num_of_processes);
                
            }
            break;
        case (7):
            clearTraceOutput(num_of_processes);
            FB2(num_of_processes);
            if(output_type){
            printf("FB-2i ");
            trace(processes,num_of_processes);

            }else
            {
            printf("FB-2i\n");
            states(processes,num_of_processes);
                
            }
            break;
        case (8):
            clearTraceOutput(num_of_processes);
            q=int(algorithm_id[i][2]-'0');
            Aging(num_of_processes,q);
            if(output_type){
            printf("Aging ");
            trace(processes,num_of_processes);
            }
            break;
        
        default:
            break;
        }
        
    }
    
    
    return 0;
}
