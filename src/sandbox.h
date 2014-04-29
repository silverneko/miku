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
         sout << "./isolate/isolate --wall-time=30 --dir=/etc/ "
              << "-e --processes=50 --stderr=compile_error --box-id="
              << id << " ";
         comm = sout.str();
      }
      int id()
      {
         return box_id;
      }
      void init()
      {
         system((comm + " --init 2>/dev/null >/dev/null").c_str());
         cerr << "[debug] box-10 inited" << endl;
      }
      void cleanup()
      {
         system((comm + " --cleanup 2>/dev/null >/dev/null").c_str());
         cerr << "[debug] box-10 cleaned" << endl;
      }
      void execute(const string &ex)
      {
         system((comm + " --run -- " + ex + " 2>/dev/null >/dev/null").c_str());
      }
};

#endif
