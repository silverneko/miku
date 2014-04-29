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
   sout << "./src/fetch_problem_meta.py " << problem_id;
   FILE *Pipe = popen(sout.str().c_str(), "r");
   sout.str("");
   sout << "./testdata/";
   sout << setfill('0') << setw(4) << problem_id;
   string dir(sout.str());
   ofstream fout(dir + "/meta");
   int testdata_count;
   fscanf(Pipe, "%d", &testdata_count);
   fout << testdata_count << endl;
   for(int i = 0; i < testdata_count; ++i){
      int testdata_id;
      long long timestamp;
      fscanf(Pipe, "%d %lld", &testdata_id, &timestamp);
      fout << testdata_id << endl;
      sout.str("");
      sout << "./src/fetch_testdata_meta.py " << testdata_id;
      FILE *Pipe2 = popen(sout.str().c_str(), "r");
      bool flag = true;
      sout.str("");
      sout << dir << "input" << setfill('0') << setw(3) << i;
      string td(sout.str());
      ifstream fin(td + ".meta");
      long long ts;
      if(fin >> ts){
         if(ts == timestamp){
            flag = false;
         }
      }
      if(flag){
         //need to renew td
      }
   }
   pclose(Pipe);
   fout.close();
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
   
   while(true){
      submission sub;
      problem pro;
      result res;
      if(fetch_submission(sub) == -1){
         usleep(3000000);
         continue;
      }
      /*
      cout << sub.problem_id << ' ';
      cout << sub.submission_id << ' ';
      cout << sub.source << ' ';
      cout << sub.lang << ' ';
      cout << sub.std << ' ';
      cout << sub.submitter_id << endl;
      */
      cerr << "Recieveed submission [" << sub.submission_id << "]" << endl;
      if(fetch_problem(sub.problem_id, pro) == -1){
         usleep(3000000);
         cerr << "[ERROR] Can't fetch problem" << endl;
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

