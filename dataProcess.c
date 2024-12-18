#include <header.h>

// Define the maximum number of tokens (9) and the maximum line length
#define MAX_TOKENS 9
#define MAX_LINE_LENGTH 256

// Process the data of CDR file for Customer billing.
void *customerDataProcess(void *cargs)
{
    long int n = 0;
    struct User *CS = (struct User*)cargs;
    FILE *fp;

    fp = fopen("data/data.cdr", "r");

    if (fp == NULL)
    {
        return NULL;
    }
    else
    {
        char line[MAX_LINE_LENGTH] = {0};
        int i = 0, j = 0, token_count = 0;
        char tokens[MAX_TOKENS][MAX_LINE_LENGTH] = {0};

        while (fgets(line, MAX_LINE_LENGTH, fp) != NULL)
        {
            token_count = 0;
            j = 0;

            // Tokenize the line by splitting using '|'
            for (i = 0; line[i] != '\0'; i++)
            {
                if (line[i] == '|')
                {
                    tokens[token_count][j] = '\0'; // End of current token
                    token_count++;
                    j = 0;
                    continue;
                }
                tokens[token_count][j] = line[i];
                j++;
            }

            // Assign values from tokens array to CS array
            strcpy(CS[n].msisdn, tokens[0]);
            strcpy(CS[n].opbrandname, tokens[1]);
            strcpy(CS[n].opmmc, tokens[2]);
            strcpy(CS[n].caltype, tokens[3]);
            strcpy(CS[n].dur, tokens[4]);
            strcpy(CS[n].down, tokens[5]);
            strcpy(CS[n].upload, tokens[6]);
            strcpy(CS[n].thirdparmsisdn, tokens[7]);
            strcpy(CS[n].thirdparmmc, tokens[8]);

            n++;
        }

        fclose(fp);
    }

    return NULL;
}

// Process the data of CDR file for Inter operator billing.
void *operatorDataProcess(void *oargs)
{
    IP *op = (IP*)oargs;
    FILE *fp;

    fp = fopen("data/data.cdr", "r");

    if (fp == NULL)
    {
        printf("\nfile error\n");
        return NULL;
    }
    else
    {
        char line[MAX_LINE_LENGTH] = {0};
        int i = 0, j = 0, token_count = 0;
        long int IN = 0;
        char tokens[MAX_TOKENS][MAX_LINE_LENGTH] = {0};
        struct User *us = (struct User *)malloc(sizeof(struct User));

        while (fgets(line, MAX_LINE_LENGTH, fp) != NULL)
        {
            us = (struct User *)realloc(us, (IN + 1) * sizeof(struct User));

            token_count = 0;
            j = 0;

            // Tokenize the line by splitting using '|'
            for (i = 0; line[i] != '\0'; i++)
            {
                                                              if (line[i] == '|')
                {
                    tokens[token_count][j] = '\0'; // End of current token
                    token_count++;
                    j = 0;
                    continue;
                }
                tokens[token_count][j] = line[i];
                j++;
            }

            // Assign values from tokens array to user structure
            strcpy(us[IN].msisdn, tokens[0]);
            strcpy(us[IN].opbrandname, tokens[1]);
            strcpy(us[IN].opmmc, tokens[2]);
            strcpy(us[IN].caltype, tokens[3]);
            strcpy(us[IN].dur, tokens[4]);
            strcpy(us[IN].down, tokens[5]);
            strcpy(us[IN].upload, tokens[6]);
            strcpy(us[IN].thirdparmsisdn, tokens[7]);
            strcpy(us[IN].thirdparmmc, tokens[8]);

            IN++;
        }

        // Initialize operator data
        strcpy(op[0].opmmc, "42500");
        strcpy(op[1].opmmc, "42501");
        strcpy(op[2].opmmc, "42502");
        strcpy(op[3].opmmc, "42503");
        strcpy(op[4].opmmc, "42504");

        int k = 0;
        while (k < 5)
        {
            bzero(op[k].opname, 20);
            op[k].incomingcall = 0;
            op[k].outgoingcall = 0;
            op[k].incomingsms = 0;
            op[k].outgoingsms = 0;
            op[k].downloaded = 0;
            op[k].uploaded = 0;
            k++;
        }

        // Process user data and update operator billing details
        char *mtc = "MTC", *moc = "MOC", *smsin = "SMS-MT", *smsout = "SMS-MO", *gprs = "GPRS";
                                                                                                       k = 0;
        while (k < IN)
        {
            if (atol(us[k].opmmc) == atol(op[0].opmmc))
            {
                if (strcmp(us[k].caltype, mtc) == 0)
                {
                    op[0].incomingcall += atol(us[k].dur);
                }
                else if (strcmp(us[k].caltype, moc) == 0)
                {
                    op[0].outgoingcall += atol(us[k].dur);
                }
                else if (strcmp(us[k].caltype, smsin) == 0)
                {
                    op[0].incomingsms++;
                }
                else if (strcmp(us[k].caltype, smsout) == 0)
                {
                    op[0].outgoingsms++;
                }
                else if (strcmp(us[k].caltype, gprs) == 0)
                {
                    op[0].downloaded += atol(us[k].down);
                    op[0].uploaded += atol(us[k].upload);
                }
                strcpy(op[0].opname, us[k].opbrandname);
                k++;
            }
            else if (atol(us[k].opmmc) == atol(op[1].opmmc))
            {
                if (strcmp(us[k].caltype, mtc) == 0)
                {
                    op[1].incomingcall += atol(us[k].dur);
                }
                else if (strcmp(us[k].caltype, moc) == 0)
                {
                    op[1].outgoingcall += atol(us[k].dur);
                }
                else if (strcmp(us[k].caltype, smsin) == 0)
                {
                    op[1].incomingsms++;
                }
                else if (strcmp(us[k].caltype, smsout) == 0)
                {
                    op[1].outgoingsms++;
                                                                                      }
                else if (strcmp(us[k].caltype, gprs) == 0)
                {
                    op[1].downloaded += atol(us[k].down);
                    op[1].uploaded += atol(us[k].upload);
                }
                strcpy(op[1].opname, us[k].opbrandname);
                k++;
            }
            // Repeat the same for other operators (op[2] to op[4]) in a similar manner
        }

        free(us);
    }

    fclose(fp);
    return NULL;
}

// Perform the data processing for both customer and inter operator using multi-threading.
void dataProcess(struct User *CS, IP *IOS)
{
    pthread_t CDthread, IDthread;
    pthread_create(&CDthread, NULL, customerDataProcess, (void *)CS);
    pthread_join(CDthread, NULL);
    pthread_cancel(CDthread);

    pthread_create(&IDthread, NULL, operatorDataProcess, (void *)IOS);
    pthread_join(IDthread, NULL);
    pthread_cancel(IDthread);
}

                                        