#ifndef SUBMISSION
#define SUBMISSION
#include<string>

using namespace std;

class result{
   public:
      int testdata_count;
      string verdict[200];
      double score[200];
      int mem[200], time[200];
      result()
      {
         testdata_count = 0;
      }
};

class problem{
   public:
      int problem_type;
      string special_judge;
      int mem_limit;
      int time_limit;
      int testdata_count;
      problem()
      {
         problem_type = 0;
         special_judge = "";
         mem_limit = 0;
         time_limit = 0;
         testdata_count = 0;
      }
};

class submission{
   public:
      int problem_id;
      int submission_id;
      string source;
      string lang;
      string std;
      string submitter;
      int submitter_id;
      submission()
      {
         problem_id = 0;
         submission_id = 0;
         source = "";
         lang = "";
         std = "";
         submitter = "";
         submitter_id = 0;
      }
};
#endif
