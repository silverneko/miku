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
#include"config.h"

using namespace std;

char buff[5000000];

int fetch_submission(submission &sub)
{
   FILE *Pipe = popen("./src/fetch_submission.py", "r");
   fscanf(Pipe, "%d", &sub.submission_id);
   if(sub.submission_id == -1)
      return -1;
   fscanf(Pipe, "%d", &sub.problem_id);
   //fscanf(Pipe, "%d", &sub.submitter_id);
   fscanf(Pipe, "%s", buff);
   sub.lang = buff;
   sub.std = "c++11";
   ostringstream sout;
   while(fgets(buff, 5000000, Pipe) != NULL)
      sout << buff;
   sub.source = sout.str();
   pclose(Pipe);
   return 0;
}

int download_testdata(int problem_id, problem &pro)
{
   ostringstream sout;
   sout << "./testdata/" << setfill('0') << setw(4) << problem_id;
   sout << "/input";
   string dir(sout.str());

   sout.str("");
   sout << "./src/fetch_testdata_meta.py " << problem_id;
   FILE *Pipe = popen(sout.str().c_str(), "r");
   int testdata_count;
   fscanf(Pipe, "%d", &testdata_count);
   pro.testdata_count = testdata_count;
   for(int i = 0; i < testdata_count; ++i){
      int testdata_id;
      long long timestamp;
      fscanf(Pipe, "%d %lld", &testdata_id, &timestamp);
      bool flag = true;
      sout.str("");
      sout << dir << setfill('0') << setw(3) << i;
      string td(sout.str());
      ifstream fin(td + ".meta");
      long long ts;
      if(fin >> ts){
         if(ts == timestamp){
            flag = false;
         }
      }
      fin.close();
      if(flag){
         //need to renew td
         sout.str("");
         sout << testdata_id << ' ' << problem_id << ' ' << i;
         system(("./src/fetch_testdata.py " + sout.str()).c_str());
         ofstream fout(td + ".meta");
         fout << timestamp << endl;
      }
   }
   pclose(Pipe);
   return 0;
}

int fetch_problem(int problem_id, problem &pro)
{
   //check if testdata dir exists
   ostringstream sout;
   sout << "./testdata/";
   sout << setfill('0') << setw(4) << problem_id;
   string testdata_dir(sout.str());
   if(access(testdata_dir.c_str(), F_OK)){
      system(("mkdir " + testdata_dir + " 2>/dev/null").c_str());
   }
   
   //download testdata
   if(download_testdata(problem_id, pro) == -1){
      return -1;
   }
   
   //Batch judge only, haven't done anything for `special`
   //`interactive`, `output only` yet
   
   
   return 0;
}

int send_result(int id, result &res)
{
   string verdict("AC");
   for(int i = 0; i < res.testdata_count; ++i)
      if(res.verdict[i] == "WA"){
         verdict = "WA";
         break;
      }
   ostringstream sout;
   sout << "./src/update_verdict.py " << id << ' ' << verdict;
   system(sout.str().c_str());
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

   FILE *Pipe = popen("pwd", "r");
   fgets(buff, 5000000, Pipe);
   string pwd(buff);
   
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
      testsuite(sub, pro, res);
      send_result(sub.submission_id, res);
   }


   return 0;
}

