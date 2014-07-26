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
   int MAXPARNUM = 1, BOXOFFSET = 10;
   bool AGGUPDATE = false;
   for(int i = 1; i < argc; ++i){
      if(argv[i][0] == '-'){
         string option(argv[i]+1);
         if(option[0] == '-'){
            if(option == "-verbose"){
               verbose = true;
            }else if(option == "-parallel"){
               MAXPARNUM = cast(argv[i+1]).to<int>();
               ++i;
            }else if(option == "-boxoffset"){
               BOXOFFSET = cast(argv[i+1]).to<int>();
               ++i;
            }else if(option == "-aggressive-update"){
               AGGUPDATE = true;
            }
         }else{
            switch(option[0]){
               case 'v':
                  verbose = true;
                  break;
               case 'p':
                  MAXPARNUM = cast(argv[i+1]).to<int>();
                  ++i;
                  break;
               case 'b':
                  BOXOFFSET = cast(argv[i+1]).to<int>();
                  ++i;
                  break;
               case 'a':
                  AGGUPDATE = true;
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
   /*
   if(access("./testdata", F_OK))
      system("mkdir ./testdata");
   if(access("./testzone", F_OK))
      system("mkdir ./testzone");
   */
   
   while(true){
      submission sub;
      int status = fetchSubmission(sub);
      if(status != 0){
         if(status == -2){
            cerr << "[ERROR] Unable to connect to TIOJ url" << endl; 
         }
         usleep(1000000);
         continue;
      }
      cerr << "Recieved submission [" << sub.submission_id << "]" << endl;
      respondValidating(sub.submission_id);
      if(fetchProblem(sub) == -1){
         usleep(1000000);
         cerr << "[ERROR] Unable to fetch problem meta" << endl;
         continue;
      }
      
      int verdict = testsuite(sub, MAXPARNUM, BOXOFFSET, AGGUPDATE);
      sendResult(sub, verdict, true);
   }


   return 0;
}

