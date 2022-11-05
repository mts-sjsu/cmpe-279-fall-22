// Server side C/C++ program to demonstrate Socket programming

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pwd.h>
#include <errno.h>

#define PORT 8000

void forkDropExec(int server_fd);
void execedChild(char const *argv[]);
 
int main(int argc, char const *argv[])
{
    printf("Start of server...\n");
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int opt = 1;

    printf("execve=0x%p\n", execve);

    if (argc > 1) {
        execedChild(argv);
        return 0;
    } 

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

    // Here we create a child process and drop its privilege to prevent from 
    // potential threats from connections and subsequent request(s).
    forkDropExec(server_fd);

    return 0;
}

void forkDropExec(int server_fd) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        printf("\n*****\tPARENT: Waiting for child %d to exit ...\n\n", pid);
        
        int status;

        waitpid(pid, &status, 0);
        printf("\n*****\tPARENT: Child process %d exited with status %d.", pid, status);
        
        exit(0);
    }

    printf("\n*****\tCHILD: child process %d initiated.\n\n", getpid());
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


    // converting server_fd to char *
    char server_fd_chars[11];
    sprintf(server_fd_chars,"%d", server_fd);


    int execl_result;
    printf("Before exec with server_fd: %d, %s\n", server_fd, server_fd_chars);
    execl_result = execl("./server", "server", server_fd_chars, (char *) NULL);

    if (execl_result == -1) {
        printf("Exec errored with errno: %d\n", errno);
        printf("%s\n", strerror(errno));
    }
}

void execedChild(char const *argv[]) {
    printf("\n*****\tEXECED: Starting execed child process\n\n");

    int new_socket, server_fd, valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[102] = {0};
    char *hello = "Hello from server";

    sscanf(argv[1], "%d", &server_fd);
    printf("Received server_fd: %d\n", server_fd);    

    printf("Waiting for client message ...\n"); 
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                    (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    printf("New connection accepted.\n");

    valread = read( new_socket , buffer, 1024);
    printf("%s\n",buffer );
    send(new_socket , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");

    printf("End EXECED\n");
}