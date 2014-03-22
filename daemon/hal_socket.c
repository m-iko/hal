#include "hal_socket.h"

int accept_loop(void * args){
    struct accept_loop_args_struct *arguments = args;

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(struct sockaddr_un));

    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, arguments->sock_path);
    unlink(addr.sun_path);
    size_t len  = sizeof(addr.sun_family) + sizeof(addr.sun_len) + strlen(addr.sun_path);

    int sockfd = socket(PF_LOCAL, SOCK_STREAM, 0);
    if (sockfd < 0){
        perror("socket()");
        return -1;
    }

    if (bind(sockfd, &addr, len) != 0){
        perror("bind()");
        close(sockfd);
        return -1;
    }

    if (listen(sockfd, BACKLOG) != 0){
        perror("listen()");
        close(sockfd);
        return -1;
    }

    while (1){
        int fd = accept(sockfd, NULL, NULL);
        if (fd >= 0){
            arguments->FDs[arguments->n_FDs] = fd;
            arguments->n_FDs++;
            printf("ADD FD: %d\n", fd);
        }
    }

    return 0;
}

void write_to_socket(int * FDs, int n_FDs, char * message){
    for (int i=0; i<n_FDs; i++){
        printf("WRITE TO: %d\n", FDs[i]);
        int r = write(FDs[i], "BITE", 5);
        if (r == 0){
            printf("REMOVE FD: %d\n", FDs[i]);
            close(FDs[i]);
            if (i < n_FDs-1){
                FDs[i] = FDs[n_FDs-1];
                i--;
            }
            else
                FDs[i] = 0;
            n_FDs--;
        }
    }
}
