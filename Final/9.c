#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_SIZE 256

int main() {
    int pipe_fd[2];
    pid_t child_pid;
    char message[] = "Howdy parent\n";
    char buffer[BUFFER_SIZE];

    // Create a pipe
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork a child process
    if ((child_pid = fork()) == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        // Child process: write to the pipe
        close(pipe_fd[0]);  // Close the read end of the pipe in the child
        write(pipe_fd[1], message, strlen(message));
        close(pipe_fd[1]);  // Close the write end of the pipe in the child
        exit(EXIT_SUCCESS);
    } else {
        // Parent process: read from the pipe
        close(pipe_fd[1]);  // Close the write end of the pipe in the parent
        read(pipe_fd[0], buffer, sizeof(buffer));
        close(pipe_fd[0]);  // Close the read end of the pipe in the parent

        // Display the data to the terminal
        printf("Parent received: %s", buffer);

        // Wait for the child to finish
        wait(NULL);
    }

    return 0;
}

