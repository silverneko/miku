#ifndef libCOMPILE
#define libCOMPILE
#include<iostream>
#include<cstdlib>
#include<sstream>
#include<cstdio>
#include<unistd.h>
#include<fstream>
#include"submission.h"
#include"sandbox.h"

int compile(sandbox &box, const submission& target)
{
   ostringstream sout;
   sout << "/tmp/box/" << box.id() << "/box/";
   string boxdir(sout.str());
   ofstream fout(boxdir + "main.cpp");
   fout << target.source << flush;
   fout.close();

   sout.str() = "";
   if(target.lang == "c++"){
      sout << "/usr/bin/g++ ./main.cpp -o ./main.out -O2 -static";
      sout << "-std=" << target.std;// << " 2>/dev/null ";
   }
   //system(sout.str().c_str());
   box.execute(sout.str());
   if(access((boxdir+"main.out").c_str(), F_OK) == -1){
      return -1;
   }
   return 0;
}

#endif
