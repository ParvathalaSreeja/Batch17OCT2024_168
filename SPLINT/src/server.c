/*#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>*/
#include <header.h> // Include your header

//#define MAXBUFF 1024
//#define PORTNO 3004

pthread_mutex_t client_count_lock;
int active_clients = 0;
FILE *log_file;
void log_message(const char *level, const char *message) {
    log_file = fopen("client_log.txt", "a");
    if (log_file == NULL) {
        printf("Error opening log file\n");
        return;
    }

   time_t now;
    time(&now);
    fprintf(log_file, "[%s] [%s] %s\n", ctime(&now), level, message);
    fclose(log_file);
}
void* handleClient(void* csfdp);

int main() {
    log_message("INFO", "Creating Socket");
    int sfd = 0, retValue = 0, *csfd = 0;
    struct sockaddr_in serv_address;

    sfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sfd < 0) {
        log_message("FATAL", "Socket creation failed");
        perror("socket() ");
        exit(EXIT_FAILURE);
    }
    printf("\nSocket created with sockfd: %d\n", sfd);
    log_message("INFO", "Socket created");
 memset(&serv_address, '\0', sizeof(serv_address));
    serv_address.sin_family = AF_INET;
    serv_address.sin_port = htons(PORTNO);
    serv_address.sin_addr.s_addr = INADDR_ANY; // inet_addr("127.0.0.1");

    retValue = bind(sfd, (struct sockaddr *)&serv_address, sizeof(serv_address));
    if (retValue < 0) {
        log_message("FATAL", "Bind failed");
        perror("bind()");
        exit(EXIT_FAILURE);
    }

    printf("\nBinded the server to the ip address with portno\n");
    log_message("INFO", "Listening to clients");
    retValue = listen(sfd, 5);
    if (retValue < 0) {
        log_message("FATAL", "Listen Failed");
        perror("listen() ");
        exit(EXIT_FAILURE);
    }
    printf("\nListening to the clients now\n");
    pthread_mutex_init(&client_count_lock, NULL);

    while (1) {
        pthread_mutex_lock(&client_count_lock);
        if (active_clients >= 5) {
            pthread_mutex_unlock(&client_count_lock);
            log_message("WARN", "Max clients reached, in waiting state");
            sleep(1);
            continue;
        }
        pthread_mutex_unlock(&client_count_lock);

        csfd = malloc(sizeof(int));
        if (csfd == NULL) {
            log_message("FATAL", "Memory Allocation Failed");
            continue;
        }

        *csfd = accept(sfd, NULL, NULL);
        printf("\nClient connected");
        if (*csfd == -1) {
            log_message("FATAL", "Accept Failed");
            free(csfd);
            continue;
        }

        log_message("INFO", "Client connected");
        pthread_mutex_lock(&client_count_lock);
        active_clients++;
        pthread_mutex_unlock(&client_count_lock);

        pthread_t clientThread;
        if (pthread_create(&clientThread, NULL, handleClient, csfd) != 0) {
            close(*csfd);
            free(csfd);
            log_message("FATAL", "Failed to create thread");
            pthread_mutex_lock(&client_count_lock);
            active_clients--;
            pthread_mutex_unlock(&client_count_lock);
        } else {
            pthread_detach(clientThread);
        }
    }

    log_message("INFO", "Closing server socket");
    close(sfd);
    pthread_mutex_destroy(&client_count_lock);
    return 0;
}

void* handleClient(void* csfdp) {
    int csfd = *(int *)csfdp;
    free(csfdp);
    int choice;
    char *msisdn[20]; // Fixed: Use a single msisdn string instead of array of pointers
    static char buffer[MAXBUFF] = {0,};
    char username[20] = {0,};
    char password[20] = {0,};

    while (1) {
        recv(csfd, &choice, sizeof(choice), 0);

        switch (choice) {
            case 1: // Sign Up
                recv(csfd, username, sizeof(username), 0);
                recv(csfd, password, sizeof(password), 0);
                int res = signUp(username, password);
                if (res == 0) {
                    send(csfd, "SignUp Successful, Please Login", 50, 0);
                } else {
                    send(csfd, "SignUp Failed", 30, 0);
                }
                break;

            case 2: // Login
                memset(username, 0, sizeof(username));
                recv(csfd, username, sizeof(username), 0);
                memset(password, 0, sizeof(password));
                recv(csfd, password, sizeof(password), 0);
                res = logIn(username, password);
                if (res == 1) {
                    send(csfd, "Login Successful", 50, 0);
                    recv(csfd, &choice, sizeof(choice), 0);
                    switch (choice) {
                        case 1: // Process CDR
                             dataProcess(&a, &b);
                           /* if (res != '\0') {
                                send(csfd, "CDR Processed Successfully", 50, 0);
                            } else {
                                send(csfd, "CDR Processing Failed", 30, 0);
                            }*/
                            break;

                        case 2: // Print/search
                            recv(csfd, &choice, sizeof(choice), 0);
                            switch (choice) {
                                case 1: // customer billing
                                    recv(csfd, msisdn, sizeof(msisdn), 0); // Receive msisdn as a string
                                                                                                                                                   char *res1 = customerBilling(&a, msisdn);
                                    if (strcmp(res1, "User not found!")) {
                                        send(csfd, "User Not Found!", 50, 0);
                                    } else {
                                        send(csfd, "User Found Started Customer Billing", 50, 0);
                                        char *res2 = customerBillingFile(&a, msisdn);
                                        if (strcmp(res2, "success")) {
                                            send(csfd, "Data dumped successfully in CB.txt", 50, 0);
                                        } else {
                                            send(csfd, "Data dumping is not successful in CB.txt", 50, 0);
                                        }
                                    }
                                    break;

                                case 2: // operator billing
                                    recv(csfd, buffer, sizeof(buffer), 0); // Receive buffer data
                                    char *res3 = interOperatorBilling(&b, buffer);
                                    if (strcmp(res3, "Operator not found!")) {
                                        send(csfd, "Operator Not Found!", 50, 0);
                                    } else {
                                        send(csfd, "Operator Found, Started IP Billing", 50, 0);
                                        char *res4 = interOperatorBillingFile(&b);
                                        if (strcmp(res4, "yes")) {
                                            send(csfd, "Data dumped successfully in IOSB.txt", 50, 0);
                                        } else {
                                            send(csfd, "Data dumping is not successful in IOSB.txt", 50, 0);
                                        }
                                    }
                                    break;

                                case 3:
                                    log_message("INFO", "Client disconnected");
                                    close(csfd);
                                    pthread_mutex_lock(&client_count_lock);
                                    active_clients--;
                                    pthread_mutex_unlock(&client_count_lock);
                                    pthread_exit(NULL);
                            }
                    }
                }
                break;
        }
    }
    log_message("INFO", "Client disconnected");
    close(csfd);
    pthread_mutex_lock(&client_count_lock);
    active_clients--;
    pthread_mutex_unlock(&client_count_lock);
                                                                pthread_exit(NULL);
}

        