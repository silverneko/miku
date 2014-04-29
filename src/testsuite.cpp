#include<iostream>
#include<algorithm>
#include<cstdio>
#include"submission.h"
#include"compile.h"
#include"sandbox.h"
#include"config.h"

using namespace std;

int testsuite(submission &sub, problem &pro, result &res)
{
   sandbox box(10);
   box.init();
   compile(box, sub);
   for(int i = 0; i < pro.testdata_count / MAXPARNUM; ++i){
   
   }
   //while(true);

   box.cleanup();
   return 0;
}
