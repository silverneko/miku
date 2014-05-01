#ifndef libCOMPILE
#define libCOMPILE

#include<iostream>
#include<cstdlib>
#include<sstream>
#include<cstdio>
#include<unistd.h>
#include<fstream>
#include"utils.h"
#include"sandbox.h"

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
      return -1;
   }
   
   return 0;
}

#endif
