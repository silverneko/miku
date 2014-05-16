#ifndef SERVER_IO
#define SERVER_IO

#include"utils.h"
#include"config.h"

int fetchSubmission(submission &);

int downloadTestdata(submission &);

int fetchProblem(submission &);

int sendResult(submission &, int verdict);

#endif
