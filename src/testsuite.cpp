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

RESULTS eval(int problem_id, int td)
{
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
      if(s!=t){/*
                  printf("%s\n",s[i]);
                  printf("%s\n",t);
                  printf("%d %d\n",strlen(s),strlen(t));
                  printf("%d %d\n",s[strlen(s)-1],t[strlen(t)-1]);*/
         error=1;
         break;
      }
   }
   
   if(error)
      return WA;
   else
      return AC;
}

int testsuite(submission &sub, problem &pro, result &res)
{
   sandboxInit(10);
   if(compile(10, sub) == CE){
      return CE;
   }

   //anyway, only have batch judge right now
   map<pid_t, int> proc;
   int head = 0, procnum = 0;
   int problem_id = sub.problem_id;
   while(true){
      while(procnum < MAXPARNUM && head < pro.testdata_count){
         pid_t pid = fork();
         if(pid == -1){
            perror("[ERROR] in testsuite :");
            return -1;
         }
         if(pid == 0){
            //child proc
            ostringstream command;
            command << "./bin/isolate --box-id=" << 20 + head;
            command << " --mem=" << pro.mem_limit;
            command << " --time=" << fixed << pro.time_limit / 1000.0;
            command << " --wall-time=" << fixed << pro.time_limit / 200.0;
            command << " --extra-time=" << fixed << 0.1;
            command << " --meta=" << "./testzone/META" << head;
            command << " --processes=" << 1;
            system((command.str() + " --init").c_str());
            ostringstream sout;
            sout << "./testdata/" << setfill('0') << setw(4) << problem_id;
            sout << "/input" << setfill('0') << setw(3) << head;
            string tddir(sout.str());
            sout.str("");
            sout << " /tmp/box/" << 20 + head << "/box/";
            system(("cp " + tddir + sout.str() + "input").c_str());
            system(("cp /tmp/box/10/box/main.out" + sout.str() + "main").c_str());
            command << " --stdin=" << "input";
            command << " --stdout=" << "output";
            command << " --stderr=" << "/dev/null";
            command << " --dir=/tmp/box/10/box/";
            command << " --run -- " << "main"; //"/tmp/box/10/box/main.out";
            system(command.str().c_str());
            exit(0);
            //execl("");
         }
         proc[pid] = head;
         cout << pid << endl;
         ++head;
         ++procnum;
      }
      int status;
      pid_t cid = waitpid(-1, &status, 0);
      cout << ' ' << cid << endl;
      if(cid == -1){
         perror("[ERROR] in testsuite :");
         return ERR;
      }else{
         res.verdict[proc[cid]] = eval(problem_id, proc[cid]);
         ostringstream sout;
         sout << "./bin/isolate --box-id=" << 20+proc[cid] 
              << " --cleanup";
         system(sout.str().c_str());
         --procnum;
      }

      if(head == pro.testdata_count && procnum == 0) break;
   }
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
   
   sandboxExec(boxid, opt, comm);
   if(access((boxdir+"main.out").c_str(), F_OK) == -1){
      return CE;
   }
   
   return 0;
}
