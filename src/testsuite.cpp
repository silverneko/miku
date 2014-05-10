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

using namespace std;

int compile(int boxid, const submission& target);
int eval(int, int);

int getExitStatus(int td)
{
   ostringstream sout;
   sout << "./testzone/META" << td;
   ifstream fin(sout.str());
   string line;
   while(!fin.eof() && getline(fin, line)){
      for(int i = 0; i < line.size(); ++i)
         if(line[i] == ':')
            line[i] = ' ';
      istringstream in(line);
      string a, b;
      in >> a >> b;
      if(a == "status"){
         if(b == "XX"){
            return ER;
         }else if(b == "TO"){
            return TLE;
         }else{
            return RE;
         }
      }
   }
   return OK;
}

int testsuite(submission &sub, problem &pro, result &res)
{
   sandboxInit(10);
   int status = compile(10, sub);
   if(status != 0) return status;

   //anyway, only have batch judge right now
   map<pid_t, int> proc;
   int procnum = 0;
   int problem_id = sub.problem_id;
   int time_limit = pro.time_limit;
   int mem_limit = pro.mem_limit;

   for(int i = 0; i < pro.testdata_count; ++i){
      while(procnum >= MAXPARNUM){
         int status;
         pid_t cid = waitpid(-1, &status, 0);
         cout << ' ' << cid << endl;
         if(cid == -1){
            perror("[ERROR] in testsuite,  `waitpid()` failed :");
            return ER;
         }
         const int td = proc[cid];
         res.verdict[td] = eval(problem_id, td);
         cout << "td" << td << " : " << res.verdict[td] << endl;
         sandboxDele(20+td);
         --procnum;
      }
      
      if(procnum < MAXPARNUM){
         const int judgeid = 20 + i;
         ostringstream sout;
         sout << "./testdata/" << setfill('0') << setw(4) << problem_id;
         sout << "/input" << setfill('0') << setw(3) << i;
         string tddir(sout.str());
         sout.str("");
         sout << " /tmp/box/" << judgeid << "/box/";
         system(("cp " + tddir + sout.str() + "input").c_str());
         system(("cp /tmp/box/10/box/main.out" + sout.str() + "main").c_str());
         //
         ostringstream command;
         command << "./bin/batchjudge " << judgeid;
         command << " " << time_limit;
         command << " " << mem_limit;
         //
         pid_t pid = fork();
         if(pid == -1){
            perror("[ERROR] in testsuite, `fork()` failed :");
            return -1;
         }
         if(pid == 0){
            //child proc
            execl("/bin/sh", "-c", command.str().c_str());
            perror("[ERROR] in testsuite, `execl()` failed :");
            exit(0);
         }
         proc[pid] = i;
         cout << "td" << i << " : " << pid << endl;
         ++procnum;
      }
   }
   while(procnum >= 1){
      int status;
      pid_t cid = waitpid(-1, &status, 0);
      cout << ' ' << cid << endl;
      if(cid == -1){
         perror("[ERROR] in testsuite,  `waitpid()` failed :");
         return ER;
      }
      const int td = proc[cid];
      res.verdict[td] = eval(problem_id, td);
      cout << "td" << td << " : " << res.verdict[td] << endl;
      sandboxDele(20+td);
      --procnum;
   }
   //clear box-10
   sandboxDele(10);

   return 0;
}

int compile(int boxid, const submission& target)
{
   ostringstream sout;
   sout << "/tmp/box/" << boxid << "/box/";
   string boxdir(sout.str());
   
   ofstream fout(boxdir + "main.cpp");
   fout << target.source << flush;
   fout.close();
   
   sout.str("");
   if(target.lang == "c++"){
      sout << "/usr/bin/g++ ./main.cpp -o ./main.out -O2 -static ";
      sout << " -std=" << target.std << " ";// << " 2>/dev/null ";
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
   
   return 0;
}

int eval(int problem_id, int td)
{
   int status = getExitStatus(td);
   if(status != OK)
      return status;
   
   int error = 0;
   string s,t;
   ostringstream sout;
   sout << "./testdata/" << setfill('0') << setw(4) << problem_id
        << "/output" << setw(3) << td;
   fstream tsol(sout.str());
   sout.str("");
   sout << "/tmp/box/" << 20 + td << "/box/output";
   fstream mout(sout.str());
   while(1){
      s="";
      t="";
      getline(tsol,s);
      getline(mout,t);
      if(t==""&&s!=""){
         error=1;
         break;
      }
      if(t!=""&&s==""){
         error=1;
         break;
      }
      if(t==""&&s==""){
         break;
      }
      s.erase(s.find_last_not_of(" \n\r\t")+1);
      t.erase(t.find_last_not_of(" \n\r\t")+1);
      if(s!=t){
         error=1;
         break;
      }
   }
   
   if(error)
      return WA;
   else
      return AC;
}
