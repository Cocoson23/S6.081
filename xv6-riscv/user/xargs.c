#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char* argv[])
{
    if(argc < 2) {
        fprintf(2, "xargs: command is too short\n");
        exit(1);
    }
    if(argc + 1 > MAXARG) {
        fprintf(2, "xargs: command is too long\n");
        exit(1);
    }

    char* command[MAXARG];
    for(int i = 1; i < argc; i++) {
        command[i] = argv[i];
    }

    command[strlen(command)] = 0;
    char* buf;
    int end = strlen(command);
    while(read(0, buf, 1)) {
        if(!strcmp(buf, "\n"))
            break;
        command[end++] = buf;
    }
    if(end == strlen(command))
        exit(0);
    command[end] = 0;

    if(fork() == 0) {
        for(int i = 1; i < strlen(command); i++) {
            exec(command[0], command[i]);
        }
        exit(0);
    }
    else if(fork() > 0) {
        wait(0);
        exit(0);
    }
    return 0;
}
