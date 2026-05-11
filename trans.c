#include <stdio.h>
#include <stdlib.h>

// structure
struct clientData
{
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
};

// function prototypes
unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
void displayAll(FILE *fPtr);
void searchRecord(FILE *fPtr);

int main()
{
    FILE *cfPtr;
    unsigned int choice;

    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL)
    {
        printf("File could not be opened.\n");
        exit(1);
    }

    while ((choice = enterChoice()) != 7)
    {
        switch (choice)
        {
        case 1:
            textFile(cfPtr);
            break;

        case 2:
            updateRecord(cfPtr);
            break;

        case 3:
            newRecord(cfPtr);
            break;

        case 4:
            deleteRecord(cfPtr);
            break;

        case 5:
            displayAll(cfPtr);
            break;

        case 6:
            searchRecord(cfPtr);
            break;

        default:
            printf("Invalid choice\n");
        }
    }

    fclose(cfPtr);
    return 0;
}

// MENU
unsigned int enterChoice(void)
{
    unsigned int choice;

    printf("\n===== BANK MENU =====\n");
    printf("1. Save accounts to text file\n");
    printf("2. Update account\n");
    printf("3. Add new account\n");
    printf("4. Delete account\n");
    printf("5. Display all accounts\n");
    printf("6. Search account\n");
    printf("7. Exit\n");
    printf("Enter choice: ");

    scanf("%u", &choice);

    return choice;
}

// WRITE TO TEXT FILE
void textFile(FILE *readPtr)
{
    FILE *writePtr;
    struct clientData client = {0, "", "", 0.0};

    if ((writePtr = fopen("accounts.txt", "w")) == NULL)
    {
        printf("File could not be opened.\n");
        return;
    }

    rewind(readPtr);

    fprintf(writePtr, "%-6s%-16s%-11s%10s\n",
            "Acct", "Last Name", "First Name", "Balance");

    while (fread(&client, sizeof(struct clientData), 1, readPtr) == 1)
    {
        if (client.acctNum != 0)
        {
            fprintf(writePtr, "%-6u%-16s%-11s%10.2f\n",
                    client.acctNum,
                    client.lastName,
                    client.firstName,
                    client.balance);
        }
    }

    fclose(writePtr);

    printf("Data saved to accounts.txt\n");
}

// UPDATE ACCOUNT
void updateRecord(FILE *fPtr)
{
    unsigned int account;
    double transaction;
    struct clientData client;

    printf("Enter account number: ");
    scanf("%u", &account);

    if (account < 1 || account > 100)
    {
        printf("Invalid account number!\n");
        return;
    }

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found!\n");
        return;
    }

    printf("Current Balance: %.2f\n", client.balance);

    printf("Enter amount (+deposit / -withdraw): ");
    scanf("%lf", &transaction);

    client.balance += transaction;

    if (client.balance < 100)
    {
        printf("Warning: Low balance!\n");
    }

    // corrected line
    fseek(fPtr, -(long)sizeof(struct clientData), SEEK_CUR);

    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Updated successfully!\n");
}

// ADD NEW ACCOUNT
void newRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};
    unsigned int accountNum;

    printf("Enter account number (1-100): ");
    scanf("%u", &accountNum);

    if (accountNum < 1 || accountNum > 100)
    {
        printf("Invalid account number!\n");
        return;
    }

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0)
    {
        printf("Account already exists!\n");
        return;
    }

    printf("Enter Lastname Firstname Balance: ");

    scanf("%14s%9s%lf",
          client.lastName,
          client.firstName,
          &client.balance);

    client.acctNum = accountNum;

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Account created successfully!\n");
}

// DELETE ACCOUNT
void deleteRecord(FILE *fPtr)
{
    struct clientData blank = {0, "", "", 0.0};
    struct clientData client;

    unsigned int accountNum;

    printf("Enter account number to delete: ");
    scanf("%u", &accountNum);

    if (accountNum < 1 || accountNum > 100)
    {
        printf("Invalid account number!\n");
        return;
    }

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found!\n");
        return;
    }

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fwrite(&blank, sizeof(struct clientData), 1, fPtr);

    printf("Account deleted!\n");
}

// DISPLAY ALL RECORDS
void displayAll(FILE *fPtr)
{
    struct clientData client;

    rewind(fPtr);

    printf("\n%-6s%-16s%-11s%10s\n",
           "Acct",
           "Last Name",
           "First Name",
           "Balance");

    while (fread(&client, sizeof(struct clientData), 1, fPtr) == 1)
    {
        if (client.acctNum != 0)
        {
            printf("%-6u%-16s%-11s%10.2f\n",
                   client.acctNum,
                   client.lastName,
                   client.firstName,
                   client.balance);
        }
    }
}

// SEARCH ACCOUNT
void searchRecord(FILE *fPtr)
{
    struct clientData client;
    unsigned int account;

    printf("Enter account number to search: ");
    scanf("%u", &account);

    if (account < 1 || account > 100)
    {
        printf("Invalid account number!\n");
        return;
    }

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found!\n");
    }
    else
    {
        printf("\nAccount Found:\n");
        printf("Account No: %u\n", client.acctNum);
        printf("Name: %s %s\n",
               client.firstName,
               client.lastName);

        printf("Balance: %.2f\n",
               client.balance);
    }
}