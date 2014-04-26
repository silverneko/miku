#ifndef SUBMISSION
#define SUBMISSION
#include<string>

using namespace std;

class submission{
   public:
      int problem_id;
      int submission_id;
      string source;
      string lang;
      string std;
      int mem_limit;
      int time_limit;
      string submitter;
      int problem_type;
      submission(int pid, int sid, string source, string lang, string std, 
               int ml, int tl, string subter, int problem_type = 0):
               problem_id(pid), submission_id(sid), source(source),
               lang(lang), std(std), mem_limit(ml), time_limit(tl),
               submitter(subter), problem_type(problem_type) {}
};
#endif
