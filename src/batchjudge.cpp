#include<iostream>
#include<algorithm>
#include<cstdio>
#include"sandbox.h"
#include<sstream>

using namespace std;

int main(int argc, char *argv[])
{
   if(argc < 4) return -1;
   int judgeid, time_limit, mem_limit;
   stringstream stoi;
   for(int i = 1; i <= 3; ++i)
      stoi << argv[i] << ' ';
   stoi >> judgeid >> time_limit >> mem_limit;
   
   //init box
   sandboxInit(judgeid);
   
   //set options
   sandboxOptions opt;
      //opt.dirs.push_back("/tmp/box/10/box");
   opt.input = "input";
   opt.output = "output";
   opt.errout = "/dev/null";
   opt.meta = "./testzone/META";
   opt.timeout = time_limit;
   opt.mem = mem_limit;
   
   //invoke box command
   sandboxExec(judgeid, opt, "main");

   return 0;
}

