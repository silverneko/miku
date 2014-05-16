#include<iostream>
#include<algorithm>
#include<cstdio>
#include<sstream>
#include<iomanip>
#include"sandbox.h"

using namespace std;

int main(int argc, char *argv[])
{
   if(argc < 5) return -1;
   int problem_id, judgeid, time_limit, mem_limit;
   stringstream stoi;
   for(int i = 1; i <= 4; ++i)
      stoi << argv[i] << ' ';
   stoi >> problem_id >> judgeid >> time_limit >> mem_limit;
   
   //init box
   sandboxInit(judgeid);
   ostringstream sout;
   sout << "./testdata/" << setfill('0') << setw(4) << problem_id;
   sout << "/input" << setfill('0') << setw(3) << judgeid-20;
   string tddir(sout.str());
   sout.str("");
   sout << " /tmp/box/" << judgeid << "/box/";
   system(("cp " + tddir + sout.str() + "input").c_str());
   system(("cp /tmp/box/10/box/main.out" + sout.str() + "main.out").c_str());
   
   //set options
   sandboxOptions opt;
      //opt.dirs.push_back("/tmp/box/10/box");
   opt.cgroup = true;
   opt.procs = 1;
   opt.input = "input";
   opt.output = "output";
   opt.errout = "/dev/null";
   sout.str("");
   sout << judgeid - 20;
   opt.meta = "./testzone/META" + sout.str();
   opt.timeout = time_limit;
   opt.mem = mem_limit;
   
   //invoke box command
   sandboxExec(judgeid, opt, "main.out");

   return 0;
}

