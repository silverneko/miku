#ifndef SANDBOX
#define SANDBOX
#include <sstream>
#include <cstdlib>

using namespace std;

class sandbox{
   private:
      int box_id;
      string comm;
   public:
      sandbox(int id) : box_id(id)
      {
         ostringstream sout;
         sout << "./isolate/isolate --stderr=compile_error --dir=./:rw --box-id=" << id << " ";
         comm = sout.str();
      }
      int id()
      {
         return box_id;
      }
      void init()
      {
         system((comm + " --init >/dev/null").c_str());
         cerr << "[debug] box-10 inited" << endl;
      }
      void cleanup()
      {
         system((comm + " --cleanup >/dev/null").c_str());
         cerr << "[debug] box-10 cleaned" << endl;
      }
      void execute(const string &ex)
      {
         system((comm + ex).c_str());
      }
};

#endif
