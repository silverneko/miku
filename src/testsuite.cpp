#include<iostream>
#include<cstdio>
#include<sstream>
#include<fstream>
#include<iomanip>
#include<unistd.h>
#include<map>
#include<sys/types.h>
#include<sys/wait.h>
#include"utils.h"
#include"sandbox.h"
#include"config.h"
#include"testsuite.h"
#include"server_io.h"

using namespace std;

int compile(const submission& target, int boxid, int spBoxid);
void eval(submission &sub, int td, int boxid, int spBoxid);
void getExitStatus(submission &sub, int td);

int MAXPARNUM = 1;
int BOXOFFSET = 10;
bool AGGUPDATE = false;

int testsuite(submission &sub)
{
   system("rm -f ./testzone/*");
   const int testBoxid = BOXOFFSET + 0, spBoxid = BOXOFFSET + 1;
   sandboxInit(testBoxid);
   sandboxInit(spBoxid);
   int status = compile(sub, testBoxid, spBoxid);
   if(status != OK) return status;
   
   //anyway, only have batch judge right now
   map<pid_t, int> proc;
   int procnum = 0;
   int problem_id = sub.problem_id;
   int* time_limit = sub.time_limit;
   int* mem_limit = sub.mem_limit;

   for(int i = 0; i < sub.testdata_count; ++i){
      while(procnum >= MAXPARNUM){
         int status;
         pid_t cid = waitpid(-1, &status, 0);
         if(cid == -1){
            perror("[ERROR] in testsuite,  `waitpid()` failed :");
            return ER;
         }
         int td = proc[cid];
         eval(sub, td, BOXOFFSET + 10 + td, spBoxid);
         if(AGGUPDATE){
            sendResult(sub, OK, false);
         }
         //cerr << "td" << td << " : " << sub.verdict[td] << endl;
         sandboxDele(BOXOFFSET + 10 + td);
         --procnum;
      }

      if(procnum < MAXPARNUM){
         //batch judge
         ostringstream command;
         command << "batchjudge " << problem_id;
         command << " " << i;
         command << " " << BOXOFFSET + 10 + i;
         command << " " << time_limit[i];
         command << " " << mem_limit[i];
         command << " " << testBoxid;
         //
         pid_t pid = fork();
         if(pid == -1){
            perror("[ERROR] in testsuite, `fork()` failed :");
            return ER;
         }
         if(pid == 0){
            //child proc
            execl("/bin/sh", "sh", "-c", command.str().c_str(), NULL);
            perror("[ERROR] in testsuite, `execl()` failed :");
            exit(0);
         }
         proc[pid] = i;
         ++procnum;
      }
   }
   while(procnum >= 1){
      int status;
      pid_t cid = waitpid(-1, &status, 0);
      if(cid == -1){
         perror("[ERROR] in testsuite,  `waitpid()` failed :");
         return ER;
      }
      const int td = proc[cid];
      //sub.verdict[td] = eval(problem_id, td);
      eval(sub, td, BOXOFFSET + 10 + td, spBoxid);
      if(AGGUPDATE){
         sendResult(sub, OK, false);
      }
      //cerr << "td" << td << " : " << sub.verdict[td] << endl;
      sandboxDele(BOXOFFSET + 10 + td);
      --procnum;
   }
   //clear box-10
   sandboxDele(testBoxid);
   sandboxDele(spBoxid);
   return OK;
}

void setExitStatus(submission &sub, int td)
{
   ostringstream sout;
   sout << "./testzone/META" << td;
   ifstream fin(sout.str());
   string line;
   map<string,string> META;
   while(!fin.eof() && getline(fin, line)){
      for(int i = 0; i < line.size(); ++i)
         if(line[i] == ':')
            line[i] = ' ';
      istringstream in(line);
      string a, b;
      in >> a >> b;
      META[a] = b;
   }

   //mem_used
   sub.mem[td] = cast(META["max-rss"]).to<int>();
   //time_used
   sub.time[td] = 1000 * cast(META["time"]).to<double>();
   //verdict
   if(META["status"] == ""){
      sub.verdict[td] = OK;
   }else if(META["status"] == "TO"){
      sub.verdict[td] = TLE;
   }else if(META["status"] == "SG"){
      switch(cast(META["exitsig"]).to<int>()){
         case 11:
            sub.verdict[td] = MLE;
            break;
         default :
            sub.verdict[td] = RE;
      }
   }else if(META["status"] == "RE"){
      sub.verdict[td] = RE;
   }else{
      // "XX"
      sub.verdict[td] = ER;
   }
   return ;
}

void eval(submission &sub, int td, int boxid, int spBoxid)
{
   int problem_id = sub.problem_id;
   setExitStatus(sub, td);
   if(sub.verdict[td] != OK){
      return ;
   }
   
   if(sub.problem_type == 1){
      ostringstream sout;
      sout << "/tmp/box/" << spBoxid << "/box/sj.out ";
      string sjpath(sout.str());
      sout.str("");
      sout << "./testdata" << setfill('0') << setw(4) << problem_id << "/input" << setw(3) << td << ' ';
      string tdin(sout.str());
      sout.str("");
      sout << "./testdata" << setfill('0') << setw(4) << problem_id << "/output" << setw(3) << td << ' ';
      string tdout(sout.str());
      sout.str("");
      sout << "/tmp/box/" << boxid << "/box/output ";
      string ttout(sout.str());
      FILE* Pipe = popen((sjpath+ttout+tdin+tdout).c_str(), "r");
      int result = 1;
      fscanf(Pipe, "%d", &result);
      pclose(Pipe);
      if(result == 0)
         sub.verdict[td] = AC;
      else
         sub.verdict[td] = WA;
      return ;
   }

   int status = AC;
   //solution output
   ostringstream sout;
   sout << "./testdata/" << setfill('0') << setw(4) << problem_id
      << "/output" << setw(3) << td;
   fstream tsol(sout.str());
   //user output
   sout.str("");
   sout << "/tmp/box/" << boxid << "/box/output";
   fstream mout(sout.str());
   while(true){
      if(tsol.eof() != mout.eof()){
         status = WA;
         break;
      }
      if(tsol.eof() && mout.eof()){
         break;
      }
      string s, t;
      getline(tsol,s);
      getline(mout,t);
      s.erase(s.find_last_not_of(" \n\r\t") + 1);
      t.erase(t.find_last_not_of(" \n\r\t") + 1);
      if(s != t){
         status = WA;
         break;
      }
   }
   sub.verdict[td] = status;
   return ;
}

int compile(const submission& target, int boxid, int spBoxid)
{
   ostringstream sout;
   sout << "/tmp/box/" << boxid << "/box/";
   string boxdir(sout.str());
   
   ofstream fout;
   if(target.lang == "c++"){
      fout.open(boxdir + "main.cpp");
   }else{
      fout.open(boxdir + "main.c");
   }
   fout << target.code << flush;
   fout.close();
   
   if(target.problem_type == 2){
      sout.str("");
      sout << boxdir << "lib" << setw(4) << setfill('0') << target.problem_id << ".h";
      fout.open(sout.str());
      fout << target.interlib << flush;
      fout.close();
   }

   sout.str("");
   if(target.lang == "c++"){
      sout << "/usr/bin/g++ ./main.cpp -o ./main.out -O2 ";
   }else{
      sout << "/usr/bin/gcc ./main.c -o ./main.out -O2 -ansi ";
   }
   if(!target.std.empty()){
      sout << "-std=" << target.std << " ";
   }
   string comm(sout.str());
   sandboxOptions opt;
   opt.dirs.push_back("/etc/");
   opt.procs = 50;
   opt.preserve_env = true;
   opt.errout = "compile_error";
   opt.timeout = 60 * 1000;
   opt.meta = "./testzone/metacomp";

   sandboxExec(boxid, opt, comm);
   if(access((boxdir+"main.out").c_str(), F_OK) == -1){
      return CE;
   }
   
   if(target.problem_type == 1){
      sout.str("");
      sout << "/tmp/box/" << spBoxid << "/box/";
      string spBoxdir = sout.str();
      
      fout.open(spBoxdir + "sj.cpp");
      fout << target.sjcode << flush;
      fout.close();
      
      sout.str("");
      sout << "/usr/bin/g++ ./sj.cpp -o ./sj.out -O2 -std=c++11 ";
      
      opt.meta = "./testzone/metacompsj";

      sandboxExec(spBoxid, opt, sout.str());
      if(access((spBoxdir+"sj.out").c_str(), F_OK) == -1){
         return ER;
      }
   }
   
   return OK;
}
