#include <cstdlib>
#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
#include <sstream>
#include <unistd.h>
#include <iomanip>

#include "server_io.h"
#include "utils.h"

using namespace std;

int main(int argc, char *argv[])
{
  if(argc < 2) return 0;
  int pid = cast(argv[1]).to<int>();
  submission sub;

  sub.problem_id = pid;

  ostringstream sout;
  sout << "./testdata/";
  sout << setfill('0') << setw(4) << sub.problem_id;
  string testdata_dir(sout.str());
  if(access(testdata_dir.c_str(), F_OK)){
    system(("mkdir " + testdata_dir + " 2>/dev/null").c_str());
  }

  downloadTestdata(sub);

  return 0;
}

