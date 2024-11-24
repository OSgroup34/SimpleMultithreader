#include <iostream>
#include <list>
#include <functional>
#include <stdlib.h>
#include <cstring>

int user_main(int argc, char **argv);

/* Demonstration on how to pass lambda as parameter.
 * "&&" means r-value reference. You may read about it online.
 */
void demonstration(std::function<void()> && lambda) {
  lambda();
}
struct thread1{
  std::function<void(int)> lambda;
  int high,low;
  
};
struct thread2{
  std::function<void(int,int)> lambda;
  int low1,low2,high1,high2;
};
void *thread_func1(void* ptr){
  thread1 * thr=((thread1*) ptr); 
  for (int i=(*thr).low;i<(*thr).high;i++){
    (*thr).lambda(i);
  }
  return NULL;
}
void *thread_func2(void* ptr){
  thread2 * thr=((thread2*) ptr); 
  for (int i=(*thr).low1;i<(*thr).high1;i++){
    for (int j=(*thr).low2;j<(*thr).high2;j++){
      (*thr).lambda(i,j);
    }
  }
  return NULL;
}

//for vector
void parallel_for(int low, int high, std::function<void(int)> &&lambda, int numThreads){
  if (low>high || numThreads<=0){
    perror("Invalid Arguments");
    exit(1);
  }
  std::chrono::time_point<std::chrono::steady_clock> startTime,endTime;
  startTime=std::chrono::steady_clock::now();
  int total=high-low;
  int perThread=total/numThreads;
  pthread_t threadIDs[numThreads];
  thread1 threadArgs[numThreads];
  if (high-low<numThreads){
    for (int i=0;i<numThreads;i++){
      threadArgs[i].low=i;
      threadArgs[i].high=i+1;
      threadArgs[i].lambda=lambda;
    
    if (i>high-low){
        threadArgs[i].low = 0;
        threadArgs[i].high = 0;
        threadArgs[i].lambda = lambda;
    }
    if (pthread_create(&threadIDs[i], NULL,thread_func1,(&threadArgs[i])) != 0){
        perror("Error: pthread create");
      }
    }
    
  }
  else{  
    for (int i=0;i<numThreads;i++){
      threadArgs[i].low=i*perThread;
      threadArgs[i].high=threadArgs[i].low+perThread;
      threadArgs[i].lambda=lambda;
      if (i==numThreads-1){
        threadArgs[i].high=high;
      }
      if (pthread_create(&threadIDs[i], NULL,thread_func1,(&threadArgs[i])) != 0){
        perror("Error: pthread create");
      }
  }}
  for (int i=0; i<numThreads; i++){
      if (pthread_join(threadIDs[i], NULL)!=0){
        perror("Error: pthread join");
      }
    }
  endTime=std::chrono::steady_clock::now();
  std::chrono::duration<double> execTime{endTime-startTime};
  std::cout << "parallel_for total execution time: ";
  std::cout<<execTime.count()<<"seconds \n";
}

//for matrix
void parallel_for(int low1, int high1, int low2, int high2,std::function<void(int, int)> &&lambda, int numThreads){
  if (low1>high1 ||low2>high2 || numThreads<=0){
    perror("Invalid Arguments");
    exit(1);
  }
  std::chrono::time_point<std::chrono::steady_clock> startTime,endTime;
  startTime=std::chrono::steady_clock::now();
  int total=high1-low1;
  int perThread=total/numThreads;
  pthread_t threadIDs[numThreads];
  thread2 threadArgs[numThreads];
  if (high1-low1<numThreads){
    for (int i=0;i<numThreads;i++){
      threadArgs[i].low1=i;
      threadArgs[i].high2=i+1;
      threadArgs[i].low2=low2;
      threadArgs[i].high2=high2;
      threadArgs[i].lambda=lambda;
    
    if (i>high1-low1){
        threadArgs[i].low1 = 0;
        threadArgs[i].high1 = 0;
        threadArgs[i].low2=0;
        threadArgs[i].high2=0;
        threadArgs[i].lambda = lambda;
    }
    if (pthread_create(&threadIDs[i], NULL,thread_func1,(&threadArgs[i])) != 0){
        perror("Error: pthread create");
      }
    }
    
  }
  else{  
    for (int i=0;i<numThreads;i++){
      threadArgs[i].low1=i*perThread;
      threadArgs[i].high1=threadArgs[i].low1+perThread;
      threadArgs[i].low2=low2;
      threadArgs[i].high2=high2;
      threadArgs[i].lambda=lambda;
      if (i==numThreads-1){
        threadArgs[i].high1=high1;
      }
      if (pthread_create(&threadIDs[i], NULL,thread_func2,(&threadArgs[i]))!=0){
        perror("Error: pthread create");
      }
  }}
  for (int i=0; i<numThreads; i++){
      if (pthread_join(threadIDs[i], NULL)!=0){
        perror("Error: pthread join");
      }
    }
  endTime=std::chrono::steady_clock::now();
  std::chrono::duration<double> execTime{endTime-startTime};
  std::cout << "parallel_for total execution time: ";
  std::cout<<execTime.count()<<"seconds \n";
}


int main(int argc, char **argv) {
  /* 
   * Declaration of a sample C++ lambda function
   * that captures variable 'x' by value and 'y'
   * by reference. Global variables are by default
   * captured by reference and are not to be supplied
   * in the capture list. Only local variables must be 
   * explicity captured if they are used inside lambda.
   */
  int x=5,y=1;
  // Declaring a lambda expression that accepts void type parameter
  auto /*name*/ lambda1 = /*capture list*/[/*by value*/ x, /*by reference*/ &y](void) {
    /* Any changes to 'x' will throw compilation error as x is captured by value */
    y = 5;
    std::cout<<"====== Welcome to Assignment-"<<y<<" of the CSE231(A) ======\n";
    /* you can have any number of statements inside this lambda body */
  };
  // Executing the lambda function
  demonstration(lambda1); // the value of x is still 5, but the value of y is now 5

  int rc = user_main(argc, argv);
 
  auto /*name*/ lambda2 = [/*nothing captured*/]() {
    std::cout<<"====== Hope you enjoyed CSE231(A) ======\n";
    /* you can have any number of statements inside this lambda body */
  };
  demonstration(lambda2);
  return rc;
}

#define main user_main


