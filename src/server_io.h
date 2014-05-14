#ifndef SERVER_IO
#define SERVER_IO

#include"utils.h"
#include"config.h"

int fetch_submission(submission &);

int download_testdata(int problem_id, problem &);

int fetch_problem(int problem_id, problem &);

int send_result(int submission_id, result &, int verdict);

#endif
