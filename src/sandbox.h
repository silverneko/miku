#ifndef SANDBOX
#define SANDBOX

#include <string>
#include <vector>

using namespace std;

class sandboxOptions{
   public:
      bool cgroup;         //--cg
      bool preserve_env;   //--full-env
      vector<string> dirs; //--dir
      string input;        //--stdin
      string output;       //--stdout
      string errout;       //--stderr
      string meta;         //--meta
      int mem;             //--mem in kilobytes
      int procs;           //--processes
      int timeout;         //--time in ms
      sandboxOptions() : cgroup(false), preserve_env(false), mem(0), procs(1), timeout(0) {}
};

int sandboxInit(int boxid);

int sandboxExec(int boxid, const sandboxOptions &option, const string &command);

int sandboxDele(int boxid);

#endif
