#include <iostream> 
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <errno.h>  
#include <cstdlib> 
using namespace std;

//void do_one_thing(int *);
void produce();
void do_another_thing(int *);
void do_wrap_up(int, int);
void sems();

  int prodNum;
  int consumNum;
  int processNum;
  int semid;

  int *consumerSet;

struct sembuf incProd = {0, 1, 0};  
struct sembuf decProd = {0, -1, 0};   
struct sembuf incCons = {1, 1, 0};  
struct sembuf decCons = {1, -1, 0};    

struct sembuf arg;
key_t key = 6969;
pthread_t* prods;
pthread_t* cons;
pthread_mutex_t mut;

int hhh = 6;


class Monitor {
private:
    int width, height;
    int Qarray;
    int sharedQ[15];
    int start;
    int end;
    int totalproduced;
    int totalconsumed;
    int bucketsFull;
  public:

       Monitor() // default constructor
    {   
        start = 0;
        end = 0;
        totalproduced = 0;
        totalconsumed = 0;
    }
 
    //void set_values (int,int);
    void insert(int, int);
    void remove(int);
    
    
    void isfull();
    void isempty();
int size (){
  int count = 0;
  for(int i = 0; i < 15; i++){
    if(sharedQ[i] > 0){
      count++;
    }
  }
  return count;
}
//CRITICAL SECTION WHERE YOU INSERT OR REMOVE
void Critical(int isProducer) {

  pthread_mutex_lock (&mut);

  if(isProducer){
    
   
    
    


    insert(pthread_self(), 3);
    //increment consumer semaphore
    
    
    

    
    //increment critical section    
  } else {
    int sem1 = semctl(semid, 0, GETVAL, arg.sem_num);
    int sem2 = semctl(semid, 1, GETVAL, arg.sem_num);

    remove(pthread_self());

  }
  pthread_mutex_unlock (&mut);
}

};




void Monitor::insert(int pid, int i) {
//thdid  item   loc action  tot prod  tot con
  int inrand = rand() * pid % 10000 + 10001 + i;
   
  
  sharedQ[end] = inrand;
  (totalproduced)++;
  printf("%u\t\t%d\t%d\t Produce \t %d\t\t%d \n", pid, inrand, end, totalproduced, totalconsumed);
    if(end == 9){
    end = 0;
  } else {(end)++;}
}


void Monitor::remove(int pid) {

  
  (totalconsumed)++;
  printf("%u\t\t%d\t%d\t Consume \t %d\t\t%d \n", pid, sharedQ[start], start, totalproduced, totalconsumed);
  sharedQ[start] = 0;
    if(start == 9){
    start = 0;
  } else {
    (start)++;
  }
}

Monitor M;

void* getProducer(void* getin){
  //semdec
  for(int i = 0; i < processNum; i++){
  semop(semid, &decProd, 1);

    M.Critical(1);
    //seminc
  semop(semid, &incCons, 1);
}

}

void* getConsumer(void* i){
  int *myvalue = (int *) i;
  for(int v = 0; v < *myvalue; v++){
  semop(semid, &decCons, 1);
    //cout << "I am a consumer!!: " << *myvalue << endl;

    M.Critical(0);
  semop(semid, &incProd, 1);
}
}

int main(int argc, char* argv[])
{
  //Monitor rect;
  //rect.set_values (3,4);
  //cout << "area: " << rect.area();
  //thdid  item   loc action  tot prod  tot con
cout << "pid\t\t\titem\tloc\t action\t\t tot prod\ttot con";

//  pid_t  child1_pid, child2_pid;
 int  status;
 prodNum = atoi(argv[1]);
 consumNum = atoi(argv[2]);
 processNum = atoi(argv[3]);
 

  


    /* create a semaphore set with 3 semaphore: */
    if ((semid = semget(key, 3, 0777 | IPC_CREAT)) == -1) {
        printf("error in semget");
        exit(1);
    } else {
      printf(" \n");
    }

    /* initialize semaphore #0 to 1: */

   //producers 
    
   arg.sem_num = 10;
    if (semctl(semid, 0, SETVAL, arg.sem_num) == -1) { 
      printf("error in semctl");
      exit(1);
    }else {
      //printf("initialized\n");
    }

    //consumers
    arg.sem_num = 0;
    if (semctl(semid, 1, SETVAL, arg.sem_num) == -1) { 
      printf("error in semctl");
      exit(1);
    }else {
      //printf("initialized\n");
    }

    //critical osprey
    arg.sem_num = 1;
    if (semctl(semid, 2, SETVAL, arg.sem_num) == -1) { 
      printf("error in semctl");
      exit(1);
    }else {
     // printf("initialized\n");
    }

    //+,-producers, +,- consumers, +,- critical section





prods = new pthread_t[prodNum];
cons = new pthread_t[consumNum];

int sharedWork = prodNum * processNum / consumNum;

int sharedWorkplusone = sharedWork + 1;

int leftoverWork = prodNum * processNum % consumNum;

consumerSet = new int [ consumNum ]; 

for(int z = 0; z < consumNum; z++){
  consumerSet[z] = sharedWork;
}

int lc = 0;
for (int i = 0; i < leftoverWork; i++){
  consumerSet[lc] = consumerSet[lc] + 1;

  if(lc == consumNum - 1){
    lc = 0;
  } else {
    lc++;
  }
}

int sem1, sem2, sem3;


for(int i = 0; i< prodNum; i++){
    pthread_create (&(prods[i]), NULL, getProducer, NULL);
}

for(int i = 0; i< consumNum; i++){
  if(i < leftoverWork){
    pthread_create (&(cons[i]), NULL, getConsumer, (void *) &sharedWorkplusone);
  } else{
    pthread_create (&(cons[i]), NULL, getConsumer, (void *) &sharedWork);
 
  }
}


///////////////////////////////////////////////////////////////////////////////

for(int i = 0; i< prodNum; i++){
   pthread_join (prods[i], NULL);
}


for(int i = 0; i< consumNum; i++){
   pthread_join (cons[i], NULL);
}


    if (semctl(semid, 0, IPC_RMID, arg) == -1) {
        printf("error in semctl");
        exit(1);
    } else {
      //printf("removed\n");
    }



}