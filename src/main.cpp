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
         if(argv[i][1] == '-'){
            string opt(argv[i]+2);
            if(opt == "verbose"){
               verbose = true;
            }else{
            
            }
         }else{
            switch(argv[i][1]){
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
      problem pro;
      result res;
      if(fetch_submission(sub) == -1){
         usleep(3000000);
         continue;
      }
      cerr << "Recieveed submission [" << sub.submission_id << "]" << endl;
      if(fetch_problem(sub.problem_id, pro) == -1){
         usleep(3000000);
         cerr << "[ERROR] Can't fetch problem" << endl;
         continue;
      }
      res.testdata_count = pro.testdata_count;
      int verdict = testsuite(sub, pro, res);
      send_result(sub.submission_id, res, verdict);
   }


   return 0;
}

