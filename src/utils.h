#ifndef UTILS
#define UTILS

#include<string>

using namespace std;

enum RESULTS{
	OK = 0,
	AC = 10,
	WA,
	TLE,
	MLE,
	OLE,
	RE,
	SIG,
	CE,
	CO,
	ER
};

class fromVerdict{
	public:
		const int verdict;
		fromVerdict(int verdict) : verdict(verdict) {}
		const char* to_str()
		{
			switch(verdict){
				case AC:
					return "Accepted";
				case WA:
					return "Wrong Answer";
				case TLE:
					return "Time Limit Exceeded";
				case MLE:
					return "Memory Limit Exceeded";
				case OLE:
					return "Output Limit Exceeded";
				case RE:
					return "Runtime Error";
				case SIG:
					return "Exited with Signal";
				case CE:
					return "Compile Error";
				case CO:
					return "Compiling Timed Out";
				case ER:
				default:
					return "Congradulations! You just panicked the judge.";
			}
		}
};

class submission{
   public:
      //meta
      int problem_id;
      int submission_id;
      string source;
      string lang;
      string std;
      string submitter;
      int submitter_id;
      //problem
      int problem_type;
      string special_judge;
      int mem_limit;
      int time_limit;
      int testdata_count;
      //test result
      int verdict[200];
      double score[200];
      int mem[200], time[200];
      
      submission()
      {
         problem_id = 0;
         submission_id = 0;
         source = "";
         lang = "";
         std = "";
         submitter = "";
         submitter_id = 0;

         problem_type = 0;
         special_judge = "";
         mem_limit = 0;
         time_limit = 0;
         testdata_count = 0;
      }
};

#endif
