#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define BACKLOG 42

struct accept_loop_args_struct {
    int FDs[BACKLOG];
    int n_FDs;
    char * sock_path;
};

int accept_loop(void * args);
void write_to_socket(int * FSs, int n_FDs, char * message);

