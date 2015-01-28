#include<iostream>
#include<sstream>
#include<fstream>
#include<iomanip>
#include<algorithm>
#include<cstdio>
#include<unistd.h>
#include<sys/types.h>
#include<getopt.h>
#include"utils.h"
#include"config.h"
#include"testsuite.h"
#include"server_io.h"

using namespace std;

void usage()
{
   cout << "\
Usage:\n\
\n\
-v(, or --verbose) for extra verbosity\n\
-p(, or --parallel) [NUMBER] to have maxium [NUMBER] of parallel\n\
processes to evaluate usercode. However this may cause verbosity\n\
messages unreadable.\n\
-b [NUMBER] to set sandbox indexing offset. Need to be set to an\n\
appropriate number if running multiple judgse on one computer.\n\
-a(, or --aggressive-update) add this to aggressivly update\n\
verdict and result."
   << endl;
   exit(2);
}

const char optstring[] = "vp:b:a";

const struct option longopts[] = {
   {"verbose",             no_argument,         NULL,    'v'},
   {"parallel",            required_argument,   NULL,    'p'},
   {"boxoffset",           required_argument,   NULL,    'b'},
   {"aggressive-update",   no_argument,         NULL,    'a'},
   {NULL,                  0,                   NULL,    0}
};

int main(int argc, char *argv[])
{
   //initialize
   bool verbose = false;
   int ac;
   while((ac = getopt_long(argc, argv, optstring, longopts, NULL)) != -1){
      switch(ac){
         case 'v':
            verbose = true;
            break;
         case 'p':
            MAXPARNUM = cast(optarg).to<int>();
            break;
         case 'b':
            BOXOFFSET = cast(optarg).to<int>();
            break;
         case 'a':
            AGGUPDATE = true;
            break;
         default:
            usage();
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
      
      int verdict = testsuite(sub);
      sendResult(sub, verdict, true);
   }

   return 0;
}

