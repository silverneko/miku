#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<sstream>
#include<iomanip>
#include"sandbox.h"

using namespace std;

int sandboxExec(int boxid, const sandboxOptions &opt, const string &comm)
{
   ostringstream sout;
   sout << "isolate -v -v --box-id=" << boxid;
   if(opt.cgroup) sout << " --cg";
   if(opt.preserve_env) sout << " --full-env";
   for(int i = 0; i < opt.dirs.size(); ++i)
      sout << " --dir=" << opt.dirs[i];
   if(!opt.input.empty()) sout << " --stdin=" << opt.input;
   if(!opt.output.empty()) sout << " --stdout=" << opt.output;
   if(!opt.errout.empty()) sout << " --stderr=" << opt.errout;
   if(!opt.meta.empty()) sout << " --meta=" << opt.meta;
   if(opt.mem != 0) sout << " --mem=" << opt.mem;
   sout << " --processes=" << opt.procs;
   double timeout = opt.timeout * 0.001;
   if(opt.timeout != 0){
      sout << " --time=" << fixed << setprecision(3) << timeout;
      //sout << " --wall-time=" << fixed << setprecision(3) << timeout * 3;
   }
   sout << " --extra-time=0.2";
   sout << " --run -- " << comm;
   system(sout.str().c_str());
   
   cerr << "[debug] sandbox" << boxid << " executed command : " << sout.str() << endl;
   return 0;
}

int sandboxInit(int boxid)
{
   ostringstream sout;
   sout << "isolate --box-id=" << boxid;
   sout << " --cg";
   sout << " --init 2>/dev/null >/dev/null";
   system(sout.str().c_str());
   cerr << "[debug] box-" << boxid << " inited" << endl;
   return 0;
}

int sandboxDele(int boxid)
{
   ostringstream sout;
   sout << "isolate --box-id=" << boxid;
   sout << " --cg";
   sout << " --cleanup 2>/dev/null >/dev/null";
   system(sout.str().c_str());
   cerr << "[debug] box-" << boxid << " cleaned" << endl;
   return 0;
}
