#include<iostream>
#include<algorithm>
#include<cstdio>
#include"submission.h"
#include"compile.h"
#include"sandbox.h"

using namespace std;

int testsuite(submission &sub, problem &pro, result &res)
{
   sandbox box(10);
   box.init();
   compile(box, sub);

   box.cleanup();
   return 0;
}
