#include<string>
#include<iostream>
#include<cstdio>
#include<sstream>
#include<iomanip>
#include<fstream>
#include<cstdlib>
#include<unistd.h>
#include"server_io.h"

int fetch_submission(submission &sub)
{
   FILE *Pipe = popen("fetch_submission.py", "r");
   fscanf(Pipe, "%d", &sub.submission_id);
   if(sub.submission_id == -1){
      pclose(Pipe);
      return -1;
   }
   fscanf(Pipe, "%d", &sub.problem_id);
   //fscanf(Pipe, "%d", &sub.submitter_id);
   char *buff = new char[5000000];
   fscanf(Pipe, "%s", buff);
   sub.lang = buff;
   sub.std = "c++11";
   ostringstream sout;
   while(fgets(buff, 5000000, Pipe) != NULL)
      sout << buff;
   sub.source = sout.str();
   pclose(Pipe);
   delete [] buff;
   return 0;
}

int download_testdata(int problem_id, problem &pro)
{
   ostringstream sout;
   sout << "./testdata/" << setfill('0') << setw(4) << problem_id;
   sout << "/input";
   string dir(sout.str());

   sout.str("");
   sout << "fetch_testdata_meta.py " << problem_id;
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
         system(("fetch_testdata.py " + sout.str()).c_str());
         ofstream fout(td + ".meta");
         fout << timestamp << endl;
      }
   }
   pclose(Pipe);
   return 0;
}

int fetch_problem(int problem_id, problem &pro)
{
   //get memlimit, timelimit
   ostringstream sout;
   sout << "fetch_limits.py " << problem_id;
   FILE *Pipe = popen(sout.str().c_str(), "r");
   fscanf(Pipe, "%d %d", &pro.time_limit, &pro.mem_limit);
   pclose(Pipe);

   //check if testdata dir exists
   sout.str("");
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

int send_result(int id, result &res, int verdict)
{
   for(int i = 0; i < res.testdata_count; ++i)
      verdict = max(verdict, res.verdict[i]);

   ostringstream sout;
   sout << "update_verdict.py " << id << ' ' << fromVerdict(verdict).to_str();
   system(sout.str().c_str());
   return 0;
}

