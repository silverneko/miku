#include<iostream>
#include<algorithm>
#include<cstdio>
#include<unistd.h>
#include<sys/types.h>
#include<sstream>
#include<fstream>
#include<iomanip>
#include"utils.h"
#include"config.h"
#include"testsuite.h"
#include"server_io.h"

using namespace std;

int main(int argc, char *argv[])
{
   //initialize
   bool verbose = false;
   for(int i = 1; i < argc; ++i){
      if(argv[i][0] == '-'){
         string option(argv[i]+1);
         if(option[0] == '-'){
            if(option == "-verbose"){
               verbose = true;
            }else{
            
            }
         }else{
            switch(option[0]){
               case 'v':
                  verbose = true;
                  break;
            }
         }
      }
   }
   
   if(!verbose){
      freopen("/dev/null", "w", stderr);
   }
   
   //initialize done
   
   if(geteuid() != 0){
      cerr << "Must be started as root !" << endl;
      return 0;
   }
   if(access("./testdata", F_OK))
      system("mkdir ./testdata");
   if(access("./testzone", F_OK))
      system("mkdir ./testzone");

   while(true){
      submission sub;
      if(fetchSubmission(sub) == -1){
         usleep(3000000);
         continue;
      }
      cerr << "Recieveed submission [" << sub.submission_id << "]" << endl;
      if(fetchProblem(sub) == -1){
         usleep(3000000);
         cerr << "[ERROR] Can't fetch problem" << endl;
         continue;
      }
      
      int verdict = testsuite(sub);
      sendResult(sub, verdict);
   }


   return 0;
}

