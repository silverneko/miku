#ifndef SERVER_IO
#define SERVER_IO

#include"utils.h"
#include"config.h"

int fetch_submission(submission &);

int download_testdata(submission &);

int fetch_problem(submission &);

int send_result(submission &, int verdict);

#endif
