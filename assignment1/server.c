// Server side C/C++ program to demonstrate Socket programming

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pwd.h>
#include <errno.h>

#define PORT 80

void forkAndDrop();

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[102] = {0};
    char *hello = "Hello from server";

    printf("execve=0x%p\n", execve);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Attaching socket to port 8000
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );


    // Forcefully attaching socket to the port 8000
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    forkAndDrop();

    printf("Waiting for client message ...\n"); 
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    valread = read( new_socket , buffer, 1024);
    printf("%s\n",buffer );
    send(new_socket , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    return 0;
}

void forkAndDrop() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        printf("*****     In parent process waiting for child to exit ...\n\n");
        
        int status;

        waitpid(pid, &status, 0);
        printf("Child process %d exit with status %d.", pid, status);
        
        exit(0);
    }

    printf("*****     In child process %d\n\n", getpid());
    printf("Current uid: %d\n", getuid());


    struct passwd* pw;
    pw = getpwnam("nobody");
    if (pw == NULL) {
        perror("getpwnam");
        exit(EXIT_FAILURE);
    }

    int nobody_uid = pw -> pw_uid;
    printf("Got pw name with uid: %d\n", pw -> pw_uid);

    int setuid_result = setuid(nobody_uid);
    if (setuid_result != 0) {
        perror("setuid");
        exit(EXIT_FAILURE);
    }

    printf("Successfully set user to nobody.\n");
    printf("Updated uid: %d\n\n", getuid());

}
