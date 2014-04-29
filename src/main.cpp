#include<iostream>
#include<algorithm>
#include<cstdio>
#include"submission.h"
#include"compile.h"
#include"testsuite.cpp"
#include<unistd.h>
#include<sys/types.h>
#include<sstream>
#include<fstream>
#include<iomanip>

using namespace std;

int fetch_submission(submission &sub)
{
   cin >> sub.problem_id >> sub.submission_id;
   sub.source = "#include<iostream>\nusing namespace std; int main(){ int n; cin >> n; int res = 0; while(n--){int a; cin >> a; res += a;} cout << res << endl; return 0;}";
   cin >> sub.lang >> sub.std >> sub.submitter;
   return 0;
}

int fetch_problem(int id, problem &pro)
{
   ostringstream sout;
   sout << "./testdata/";
   sout << setfill('0') << setw(4) << id;
   sout << "/meta";
   ifstream fin(sout.str());
   int timestamp;
   fin >> timestamp;
   fin >> pro.problem_type;
   fin >> pro.testdata_count;
   switch(pro.problem_type){
      case 0:
         
         break;
      case 1:
         //fetch special judger
         break;
   }
   //fetch problem mem_limiy time_limit
   pro.mem_limit = 65536;
   pro.time_limit = 1000;
   return 0;
}

int send_result(result &res)
{
   
   return 0;
}

int main()
{
   if(geteuid() != 0){
      cerr << "Must be started as root !" << endl;
      return 0;
   }
   if(access("./testdata", F_OK))
      system("mkdir ./testdata");
   if(access("./testzone", F_OK))
      system("mkdir ./testzone");
   
   /*
   submission sub;
   problem pro;
   result res;
   testsuite(sub, pro, res);
   */

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
         cerr << "Can't fetch problem" << endl;
         continue;
      }
      //cout << sub.submitter << ' ' << pro.testdata_count << endl;
      //while(true);
      testsuite(sub, pro, res);
      send_result(res);
      while(true);
   }


   return 0;
}

