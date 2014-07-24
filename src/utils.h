#ifndef UTILS
#define UTILS

#include<sstream>
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
		const int verdict;
	public:
      explicit fromVerdict();
		explicit fromVerdict(int verdict) : verdict(verdict) {}
		const char* toStr() const
		{
			switch(verdict){
				case AC:
					return "Accepted";
				case WA:
					return "Wrong Answer";
				case TLE:
					return "Time Limit Exceeded";
				case MLE:
					return "Segmentation Fault";
				case OLE:
					return "Output Limit Exceeded";
				case RE:
					return "Runtime Error (exited with nonzero status)";
				case SIG:
					return "Runtime Error (exited with signal)";
				case CE:
					return "Compile Error";
				case CO:
					return "Compilation Timed Out";
				case ER:
					return "WTF!";
                                default:
                                        return "nil";
			}
		}
		const char* toAbr() const
		{
			switch(verdict){
				case AC:
					return "AC";
				case WA:
					return "WA";
				case TLE:
					return "TLE";
				case MLE:
					return "MLE";
				case OLE:
					return "OLE";
				case RE:
					return "RE";
				case SIG:
					return "SIG";
				case CE:
					return "CE";
				case CO:
					return "CO";
				case ER:
					return "ER";
                                default:
                                        return "";
			}
		}
};

class cast{
      const string val;
   public:
      explicit cast();
      cast(const string& c) : val(c) {}
      template<typename T> T to() const
      {
         T res;
         istringstream in(val);
         in >> res;
         return res;
      }
};

class submission{
   public:
      //meta
      int problem_id;
      int submission_id;
      string code;
      string sjcode;
      string interlib;
      string lang;
      string std;
      string submitter;
      int submitter_id;
      //problem
      int problem_type;
      string special_judge;
      //test result
      int testdata_count;
      int mem_limit[200];
      int time_limit[200];
      int verdict[200];
      int mem[200], time[200];
      
      submission()
      {
         problem_id = 0;
         submission_id = 0;
         code = "";
         sjcode = "";
         interlib = "";
         lang = "";
         std = "";
         submitter = "";
         submitter_id = 0;
         problem_type = 0;
         special_judge = "";
         testdata_count = 0;
         fill(mem_limit, mem_limit + 200, 0);
         fill(time_limit, time_limit + 200, 0);
         fill(verdict, verdict + 200, 0);
         fill(mem, mem + 200, 0);
         fill(time, time + 200, 0);
      }
};

#endif
