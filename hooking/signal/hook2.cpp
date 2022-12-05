#include<bits/stdc++.h>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>
#define endl '\n'
using namespace std;

static void signal_handler(int signum){
    cout<<"Signal "<<signum<<" received"<<endl;
    switch (signum)
    {
    case SIGINT:
        cout<<"SIGINT received"<<endl;
        cout<<"PID is "<<getpid()<<endl;
        cout<<"PPID is "<<getppid()<<endl;
        break;
    case SIGTERM:
        cout<<"SIGTERM received"<<endl;
        cout<<"PID is "<<getpid()<<endl;
        cout<<"PPID is "<<getppid()<<endl;
        break;
    }
}

int main(){
    try{
        signal(SIGINT,signal_handler);
        signal(SIGTERM,signal_handler);
    }
    catch(exception &e){
        cout<<e.what()<<endl;
    }

    while(1){
        cout<<"Waiting for signal... PID is "<<getpid()<<endl;
        sleep(1);
    }
}