#include<iostream>
#include<algorithm>
#include<cstdio>
#include"submission.h"
#include"compile.h"
#include"testsuite.cpp"
#include<unistd.h>
#include<sys/types.h>

using namespace std;

int main()
{
   if(geteuid() != 0){
      cerr << "Must be started as root !" << endl;
      return 0;
   }
   /*
    * preprocess
    * 
    * 
    */
   submission sub;
   problem pro;
   result res;
   testsuite(sub, pro, res);
   
   while(true){
      /*
       * get_submission
       * fetch_submission_data
       * compile
       * get_result
       * send_result
       */  
     
   }
   
   
   return 0;
}

