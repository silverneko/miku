#include"submission.h"
#include<iostream>
#include<cstdlib>
#include<sstream>
#include<cstdio>
#include<unistd.h>

int compile(const submission& target)
{
   ostringstream sout;
   if(target.lang == "c++"){
      sout << "g++ ./testzone/main.cpp -o ./testzone/main.out -O2 ";
      sout << "-std=" << target.std << " 2>/dev/null ";
   }
   system(sout.str().c_str());
   if(access("./testzone/main.out", F_OK) == -1){
      return -1;
   }
   return 0;
}

