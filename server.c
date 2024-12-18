#include<my_header.h>
pthread_mutex_t client_count_lock;
int active_clients = 0;

// Thread function to handle client requests
void* handleClient(void *csfdp) {
    int csfd = *((int*)csfdp);
    free(csfdp);

    char msg[MAXBUFF] = {0};
    char uid[MAXBUFF] = {0};
    char upass[MAXBUFF] = {0};
    int dataFlag = 0;
    struct User *CS = NULL;
    IP *IOS = NULL;

    while (1) {
        bzero(msg, MAXBUFF);
        read(csfd, msg, MAXBUFF);
        printf("[DEBUG] Message received from client: %s\n", msg);

        if (atoi(msg) == 3) {
            free(CS);
            free(IOS);
            CS = NULL;
            IOS = NULL;
            break;
        } else if (atoi(msg) == 1) {
            bzero(msg, MAXBUFF);
            strcpy(msg, "Enter username:");
            write(csfd, msg, strlen(msg));

            bzero(msg, MAXBUFF);
            read(csfd, uid, MAXBUFF);

            bzero(msg, MAXBUFF);
            strcpy(msg, "Enter password:");
            write(csfd, msg, strlen(msg));

            bzero(msg, MAXBUFF);
            read(csfd, upass, MAXBUFF);

            FILE *fp = fopen("../data/user_details.txt", "a+");
            if (fp == NULL) {
                fprintf(stderr, "[FATAL] Unable to open user_details.txt\n");
                write(csfd, "Error, please try again later.", 30);
                continue;
            }

            if (signUp(uid, upass)) {
                fprintf(fp, "%s %s\n", uid, upass);
                fclose(fp);
                write(csfd, "1", 1);  // Success
            } else {
                fclose(fp);
                write(csfd, "0", 1);  // Failure
            }
        } else if (atoi(msg) == 2) {
            strcpy(msg, "Enter username:");
            write(csfd, msg, strlen(msg));

            bzero(msg, MAXBUFF);
            read(csfd, uid, MAXBUFF);

            bzero(msg, MAXBUFF);
            strcpy(msg, "Enter password:");
            write(csfd, msg, strlen(msg));

            read(csfd, upass, MAXBUFF);

            if (logIn(uid, upass)) {
                bzero(msg, MAXBUFF);
                strcpy(msg, "1");
                write(csfd, msg, strlen(msg)); // Login success

                while (1) {
                    bzero(msg, MAXBUFF);
                    read(csfd, msg, MAXBUFF);

                    // Replaced logic for data flag check and data processing
                    if (atoi(msg) == 1) {
                        if (dataFlag == 0) {
                            CS = (struct User*)malloc(100000 * sizeof(struct User));
                            IOS = (IP *)malloc(5 * sizeof(IP));  // Memory allocation for data processing
                            dataProcess(CS, IOS);  // Processing the data

                            // Checking if both CS and IOS are successfully allocated
                            if (CS != NULL && IOS != NULL) {
                                dataFlag = 1;
                                strcpy(msg, "1");  // Data processed successfully
                                write(csfd, msg, strlen(msg));
                            } else {
                                strcpy(msg, "2");  // Failure to process data
                                write(csfd, msg, strlen(msg));
                            }
                        } else {
                            strcpy(msg, "3");  // Data already processed
                            write(csfd, msg, strlen(msg));
                        }
                    } else if (atoi(msg) == 2) {
                        // Billing menu logic
                        bzero(msg, MAXBUFF);
                        strcpy(msg, "\n1. Customer Billing\n2. Inter Operator Billing\nChoice: ");
                        write(csfd, msg, strlen(msg));

                        bzero(msg, MAXBUFF);
                        read(csfd, msg, MAXBUFF);
                        if (atoi(msg) == 1) {
                            bzero(msg, MAXBUFF);
                            strcpy(msg, "\n---------------> CUSTOMER BILLING <--------------\n\n");
                            strcat(msg, "1. Display on screen\n2. Download the CB.txt\nChoice: ");
                            write(csfd, msg, strlen(msg));

                            bzero(msg, MAXBUFF);
                            read(csfd, msg, MAXBUFF);
                            if (atoi(msg) == 1) {
                                bzero(msg, MAXBUFF);
                                strcpy(msg, "\n\nEnter MSISDN:");
                                write(csfd, msg, strlen(msg));

                                bzero(msg, MAXBUFF);
                                read(csfd, msg, MAXBUFF);
                                char msisdn[MAXBUFF] = {0};
                                strcpy(msisdn, msg);

                                bzero(msg, MAXBUFF);
                                char *customerAck = customerbilling(CS, msisdn);  // Customer billing process
                                bzero(msisdn, MAXBUFF);
                                strcpy(msg, customerAck);
                                free(customerAck);
                                write(csfd, msg, strlen(msg));
                            } else if (atoi(msg) == 2) {
                                bzero(msg, MAXBUFF);
                                                                                  strcpy(msg, "\n1. Customer Billing\n2. Inter Operator Billing\nChoice: ");
                        write(csfd, msg, strlen(msg));

                        bzero(msg, MAXBUFF);
                        read(csfd, msg, MAXBUFF);
                        if (atoi(msg) == 1) {
                            bzero(msg, MAXBUFF);
                            strcpy(msg, "\n---------------> CUSTOMER BILLING <--------------\n\n");
                            strcat(msg, "1. Display on screen\n2. Download the CB.txt\nChoice: ");
                            write(csfd, msg, strlen(msg));

                            bzero(msg, MAXBUFF);
                            read(csfd, msg, MAXBUFF);
                            if (atoi(msg) == 1) {
                                bzero(msg, MAXBUFF);
                                strcpy(msg, "\n\nEnter MSISDN:");
                                write(csfd, msg, strlen(msg));

                                bzero(msg, MAXBUFF);
                                read(csfd, msg, MAXBUFF);
                                char msisdn[MAXBUFF] = {0};
                                strcpy(msisdn, msg);

                                bzero(msg, MAXBUFF);
                                char *customerAck = customerbilling(CS, msisdn);  // Customer billing process
                                bzero(msisdn, MAXBUFF);
                                strcpy(msg, customerAck);
                                free(customerAck);
                                write(csfd, msg, strlen(msg));
                            } else if (atoi(msg) == 2) {
                                bzero(msg, MAXBUFF);
                                strcpy(msg, "\n\nEnter MSISDN:");
                                write(csfd, msg, strlen(msg));

                                bzero(msg, MAXBUFF);
                                read(csfd, msg, MAXBUFF);
                                char msisdn[MAXBUFF] = {0};
                                strcpy(msisdn, msg);
                                bzero(msg, MAXBUFF);
                                char *customerAck = customerbillingfile(CS, msisdn);  // Customer billing file process
                                bzero(msisdn, MAXBUFF);
                                strcpy(msg, customerAck);
                                free(customerAck);
                                write(csfd, msg, strlen(msg));
                            }
                        } else if (atoi(msg) == 2) {
                            // Inter Operator Billing menu
                            bzero(msg, MAXBUFF);
                            strcpy(msg, "\n----------------> INTER OPERATOR BILLING <---------------");
                            strcat(msg, "\n\n1. Display result on the user screen\n2. Download IOSB.txt\nChoice : ");
                            write(csfd, msg, strlen(msg));

                            bzero(msg, MAXBUFF);
                            read(csfd, msg, MAXBUFF);
                            if (atoi(msg) == 1) {
                                bzero(msg, MAXBUFF);
                                strcpy(msg, "\n\nEnter Operator Code :");
                                write(csfd, msg, strlen(msg));

                                bzero(msg, MAXBUFF);
                                read(csfd, msg, MAXBUFF);
                                char operatorCode[MAXBUFF] = {0};
                                strcpy(operatorCode, msg);

                                bzero(msg, MAXBUFF);
                                char *customerAck = interoperatorbilling(IOS, operatorCode);  // Inter operator billing process
                                strcpy(msg, customerAck);
                                free(customerAck);
                                write(csfd, msg, strlen(msg));
                            } else if (atoi(msg) == 2) {
                                bzero(msg, MAXBUFF);
                                char *customerAck = interoperatorbillingfile(IOS);  // Inter operator billing file process
                                strcpy(msg, customerAck);
                                free(customerAck);
                                write(csfd, msg, strlen(msg));
                            }
                        }
                    } else if (atoi(msg) == 3) {
                        // Clean up and exit
                        free(CS);
                        free(IOS);
                        CS = NULL;
                        IOS = NULL;
                        break;
                         }
                }
            } else {
                bzero(msg, MAXBUFF);
                strcpy(msg, "2");  // Login failure
                write(csfd, msg, strlen(msg));
            }
        }
    }

    pthread_mutex_lock(&client_count_lock);
    active_clients--;
    pthread_mutex_unlock(&client_count_lock);

    return NULL;
}

int main() {
    int sfd = 0, retValue = 0, *csfd = 0;
    struct sockaddr_in serv_address;

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd < 0) {
        fprintf(stderr, "[FATAL] Socket creation failed\n");
        perror("socket() ");
        exit(EXIT_FAILURE);
    }

    printf("[INFO] Socket created\n");

    memset(&serv_address, '\0', sizeof(serv_address));
    serv_address.sin_family = AF_INET;
    serv_address.sin_port = htons(PORTNO);
    serv_address.sin_addr.s_addr = INADDR_ANY; // Local address

    retValue = bind(sfd, (struct sockaddr *)&serv_address, sizeof(serv_address));
    if (retValue < 0) {
        fprintf(stderr, "[FATAL] Bind failed\n");
        perror("bind() ");
        exit(EXIT_FAILURE);
    }

    printf("[INFO] Listening to clients\n");
    retValue = listen(sfd, 5);
    if (retValue < 0) {
        fprintf(stderr, "[FATAL] Listen Failed\n");
                                                                       perror("listen() ");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_init(&client_count_lock, NULL);

    while (1) {
        pthread_mutex_lock(&client_count_lock);
        if (active_clients >= 5) {
            pthread_mutex_unlock(&client_count_lock);
            printf("[INFO] Max clients reached, waiting\n");
            sleep(1);
            continue;
        }
        pthread_mutex_unlock(&client_count_lock);

        csfd = malloc(sizeof(int));
        if (csfd == NULL) {
            fprintf(stderr, "[FATAL] Memory allocation failed\n");
            continue;
        }

        *csfd = accept(sfd, NULL, NULL);
        if (*csfd == -1) {
            fprintf(stderr, "[FATAL] Accept failed\n");
            free(csfd);
            continue;
        }

        printf("[INFO] Client connected\n");

        pthread_mutex_lock(&client_count_lock);
        active_clients++;
        pthread_mutex_unlock(&client_count_lock);

        pthread_t client_thread;
        if (pthread_create(&client_thread, NULL, handleClient, csfd) != 0) {
            close(*csfd);
            free(csfd);
            fprintf(stderr, "[FATAL] Failed to create thread\n");
            pthread_mutex_lock(&client_count_lock);
            active_clients--;
            pthread_mutex_unlock(&client_count_lock);
        } else {
                 pthread_detach(client_thread);
        }
    }

    printf("[INFO] Closing server socket\n");
    close(sfd);
    pthread_mutex_destroy(&client_count_lock);

    return 0;
}

