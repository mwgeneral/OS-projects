

#include <iostream> 
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
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



int   shared_mem_id;
int   *shared_mem_ptr;
int   *r1p;
int   *r2p;
int   *r3p;
int* sharedQ;
int hhh = 6;


class Queue {
private:
    int width, height;
    int Qarray;
    int* sharedQ;
    int* start;
    int* end;
    int* totalproduced;
    int* totalconsumed;
    int* bucketsFull;
  public:

       Queue(int* address) // default constructor
    {
        sharedQ = address;
        
        //start
         sharedQ[10] = 0;
         //end
         sharedQ[11] = 0;
         //total produced
         sharedQ[12] = 0;
         //totalconsumed
         sharedQ[13] = 0;
         //bucketsFull
         sharedQ[14] = 0;
         start = sharedQ + 10;
         end = sharedQ + 11;
         totalproduced = sharedQ + 12;
         totalconsumed = sharedQ + 13;


         //end = sharedQ[11];

         //this makes the second spot on the shared mem pointer store the value "70"
         //sharedQ[1] = 70;
         //printf("%u\n", sharedQ[0]);
    }
 
    //void set_values (int,int);
    void insert(int, int, int, int, int);
    void remove(int, int, int, int);
    void isfull();
    void isempty();

};

void Queue::insert(int pid, int i, int sem1, int sem2, int sem3) {
//pid    item   loc sem1 sem2  sem3   action  tot prod  tot con
  int inrand = rand() * pid % 100 + 101 + i;
   
  
  sharedQ[*end] = inrand;
  (*totalproduced)++;
  printf("%d \t %d \t %d \t %d \t %d \t %d \t Produce \t %d \t\t %d \n", pid, inrand, *end, sem1, sem2, sem3, *totalproduced, *totalconsumed);
    if(*end == 9){
    *end = 0;
  } else {(*end)++;}
}


void Queue::remove(int pid, int sem1, int sem2, int sem3) {
//pid    item   loc sem1 sem2  sem3 ...   action  tot prod  tot con

  
  (*totalconsumed)++;
  printf("%d \t %d \t %d \t %d \t %d \t %d \t Consume \t %d \t\t %d \n", pid, sharedQ[*start], *start, sem1,sem2,sem3, *totalproduced, *totalconsumed);
  sharedQ[*start] = 0;
    if(*start == 9){
    *start = 0;
  } else {
    (*start)++;
  }
}


int main(int argc, char* argv[])
{
  //Queue rect;
  //rect.set_values (3,4);
  //cout << "area: " << rect.area();
cout << "pid \t item \t loc \t sem1 \t sem2 \t sem3 \t action \t tot prod \t tot con";

//  pid_t  child1_pid, child2_pid;
  int  pid;
  int  status;
  int prodNum = atoi(argv[1]);
  int consumNum = atoi(argv[2]);
  int processNum = atoi(argv[3]);


int* prods = new int[prodNum];

  /* initialize shared memory segment */
  if ((shared_mem_id = shmget(IPC_PRIVATE, 15*sizeof(int), 0777)) == -1)
    printf("error in shmget"), exit(1);
  if ((shared_mem_ptr = (int *)shmat(shared_mem_id, (void *)0, 0)) == 
                                                 (void *)-1)
    printf("shmat failed"), exit(1);



      key_t key = 6969;
    int semid;
/*     union semun arg; */
    struct sembuf arg;

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

    struct sembuf incProd = {0, 1, 0};  
    struct sembuf decProd = {0, -1, 0};   
    struct sembuf incCons = {1, 1, 0};  
    struct sembuf decCons = {1, -1, 0};   
    struct sembuf incCrit = {2, 1, 0};  
    struct sembuf decCrit = {2, -1, 0};   


//chanfbwhidabh

  //first spot
  r1p = shared_mem_ptr;
  //second spot;  
  r2p = (shared_mem_ptr + 1);
  //third spot 
  r3p = shared_mem_ptr + 2;
  //puts "23" into the first spot in shared memory
  //*r1p = 23;
  //*r3p = 55;

/*

cout << "this is &r1p, the address" << endl;
printf("%d\n", &r1p);
cout << "this is *r1p, the value" << endl;
cout << *r1p << endl;

cout << "this is &r2p, the address" << endl;
printf("%d\n", &r2p);
cout << "this is *r2p, the value" << endl;
cout << *r2p << endl;
*/
//printf("the addrs, shared_mem_ptr=%u, r1p=%u, r2p=%u, r3p=%u\n",shared_mem_ptr, r1p, r2p, r3p);

Queue Q(shared_mem_ptr);


  //*r1p = 0;
  //*r2p = 0;


  //int input = 55;

  //shared_mem_ptr[0]=input;

//make an array of producers
//fork each producer


//producers

/*
    struct sembuf incProd = {0, 1, 0};  
    struct sembuf decProd = {0, -1, 0};   
    struct sembuf incCons = {1, 1, 0};  
    struct sembuf decCons = {1, -1, 0};   
    struct sembuf incCrit = {2, 1, 0};  
    struct sembuf decCrit = {2, -1, 0};   
*/

int sem1, sem2, sem3;
for(int i = 0; i< prodNum; i++){

  if ((pid = fork()) == 0) {
    for(int j = 0; j < processNum; j++){
      //semop check if there are ones to produce (space free)
    semop(semid, &decProd, 1);
      //semop check if critical space is free
    semop(semid, &decCrit, 1);

     sem1 = semctl(semid, 0, GETVAL, arg.sem_num);
     sem2 = semctl(semid, 1, GETVAL, arg.sem_num);
     sem3 = semctl(semid, 2, GETVAL, arg.sem_num);


    Q.insert(getpid(), i, sem1, sem2, sem3);
    //increment consumer semaphore
    semop(semid, &incCons, 1);
    //increment critical section
    semop(semid, &incCrit, 1);
    

  }
    //kill child
    return 0;
  } else if (pid == -1) {
    printf("error in fork"), exit(1);
  }else{
   // printf("producer parent pid = %d\n",pid);
  }
}


int sharedWork = prodNum * processNum / consumNum;

int leftoverWork = prodNum * processNum % consumNum;

int* consumerSet = new int [ consumNum ]; 

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

  //consumerSet[consumNum - 1] = consumerSet[consumNum - 1] + leftoverWork;
  //cout << leftoverWork << endl;
//for(int z = 0; z < consumNum; z++){
//  cout << consumerSet[z] << endl;
//}

/*
    struct sembuf incProd = {0, 1, 0};  
    struct sembuf decProd = {0, -1, 0};   
    struct sembuf incCons = {1, 1, 0};  
    struct sembuf decCons = {1, -1, 0};   
    struct sembuf incCrit = {2, 1, 0};  
    struct sembuf decCrit = {2, -1, 0};   
*/

for(int i = 0; i< consumNum; i++){
  if ((pid = fork()) == 0) {

    for(int j = 0; j < consumerSet[i]; j++){
      //semop check if there are items to consume
      
      semop(semid, &decCons, 1);
      //semop check if there critical space is free
      
      semop(semid, &decCrit, 1);

     sem1 = semctl(semid, 0, GETVAL, arg.sem_num);
     sem2 = semctl(semid, 1, GETVAL, arg.sem_num);
     sem3 = semctl(semid, 2, GETVAL, arg.sem_num);


      Q.remove(getpid(), sem1, sem2, sem3);
      //increment producer semophore
      semop(semid, &incProd, 1);
      //increment critical section
      semop(semid, &incCrit, 1);
    }
    //kill child
    return 0;
  } else if (pid == -1) {
   // printf("error in fork"), exit(1);
  }else{
  //  printf("consumer parent pid = %d\n",pid);
  }

}

  /* parent */
  
    //printf("child1_pid1 = %d\n",pid);
    
  if ((waitpid(pid, &status, 0) == -1))
  {printf("error in waitpid"), exit(1);}

   
//wait(NULL);
//int ppp = shared_mem_ptr[0];

  //sems();  

  do_wrap_up(*r1p, *r2p);

/* the following removes the allocated shared memory */
/* comment it out to see the shared memory   */      
  
  for(int d = 0; d < (prodNum + consumNum); d++){
   wait(NULL);  
}
    /* grab the semaphore set created by seminit.c: */
    if ((semid = semget(key, 1, 0)) == -1) {
        printf("error in semget");
        exit(1);
    }else {
      //printf("grabbed\n");
    }

    /* remove it: */
    if (semctl(semid, 0, IPC_RMID, arg) == -1) {
        printf("error in semctl");
        exit(1);
    } else {
      //printf("removed\n");
    }




    if (shmctl(shared_mem_id, IPC_RMID, 0) == -1) {
        printf("error in shmctl");
        exit(1);
   } 
  
  return 0; 
}

void do_wrap_up(int one_times, int another_times)
{
  int total;

  total = one_times + another_times;

}


