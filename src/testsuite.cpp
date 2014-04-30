#include<iostream>
#include<algorithm>
#include<cstdio>
#include<unistd.h>
#include<sstream>
#include<iomanip>
#include"submission.h"
#include"compile.h"
#include"sandbox.h"
#include"config.h"
#include<map>
#include<sys/types.h>
#include<sys/wait.h>

using namespace std;

string eval(int problem_id, int td)
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
      return string("WA");
   else
      return string("AC");
}

int testsuite(submission &sub, problem &pro, result &res)
{
   sandbox box(10);
   box.init();

   compile(box, sub);

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
            command << "./isolate/isolate --box-id=" << 20 + head;
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
         proc[pid] = head+1;
         cout << pid << endl;
         ++head;
         ++procnum;
      }

      int status;
      pid_t cid = waitpid(-1, &status, WUNTRACED);
      cout << ' ' << cid << endl;
      if(cid == -1){
         perror("[ERROR] in testsuite :");
         return -1;
      }if(proc[cid] == 0){
         continue;
      }else{
         --proc[cid];
         res.verdict[proc[cid]] = eval(problem_id, proc[cid]);
         ostringstream sout;
         sout << "./isolate/isolate --box-id=" << 20+proc[cid] 
              << " --cleanup";
         system(sout.str().c_str());
         --procnum;
      }

      if(head == pro.testdata_count && procnum == 0) break;
   }
   box.cleanup();
   return 0;
}
