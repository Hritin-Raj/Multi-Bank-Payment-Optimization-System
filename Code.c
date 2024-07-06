#include <stdio.h>
#include <limits.h>
#include <string.h>

#define MAX_BANKS 100

typedef struct {
    char bankName[50];
    int balance;
    char paymentMethods[5][20];
    int numPaymentMethods;
} Bank;

typedef struct {
    char bankName[50];
    int index;
} BankIndex;

int getMinIdx(Bank bankList[], int numBanks) {
    int minBalance = INT_MAX, minIdx = -1;
    for (int i = 0; i < numBanks; i++) {
        if (bankList[i].balance == 0) {
            continue;
        }
        if (bankList[i].balance < minBalance) {
            minIdx = i;
            minBalance = bankList[i].balance;
        }
    }
    return minIdx;
}

int getMaxIdx(Bank bankList[], int numBanks) {
    int maxBalance = INT_MIN, maxIdx = -1;
    for (int i = 0; i < numBanks; i++) {
        if (bankList[i].balance == 0) {
            continue;
        }
        if (bankList[i].balance > maxBalance) {
            maxIdx = i;
            maxBalance = bankList[i].balance;
        }
    }
    return maxIdx;
}

void optimizeCashFlow(int numBanks, Bank input[], int numTransactions, int graph[MAX_BANKS][MAX_BANKS], int maxNumMethods) {

    Bank bankList[MAX_BANKS];
    for (int b = 0; b < numBanks; b++) {
        strcpy(bankList[b].bankName, input[b].bankName);
        bankList[b].numPaymentMethods = input[b].numPaymentMethods;
        for (int m = 0; m < input[b].numPaymentMethods; m++) {
            strcpy(bankList[b].paymentMethods[m], input[b].paymentMethods[m]);
        }

        int balance = 0;

        for (int i = 0; i < numBanks; i++) {
            balance += graph[i][b];
        }

        for (int j = 0; j < numBanks; j++) {
            balance += (-1) * graph[b][j];
        }

        bankList[b].balance = balance;
    }

    int zeroBalances = 0;
    for (int i = 0; i < numBanks; i++) {
        if (bankList[i].balance == 0) {
            zeroBalances++;
        }
    }

    printf("\nThe transactions for optimized transaction flow among Indian banks are as follows:\n\n");

    while (zeroBalances != numBanks) {
        int minIdx = getMinIdx(bankList, numBanks);
        int maxBalance = INT_MIN;
        int largestIdx = -1;
        char matchingMethod[20] = "";

        // Finding largestIdx and matchingMethod directly here
        for (int i = 0; i < numBanks; i++) {
            if (bankList[i].balance == 0 || bankList[i].balance < 0) {
                continue;
            }

            char v[maxNumMethods][20];
            int count = 0;
            for (int j = 0; j < input[minIdx].numPaymentMethods; j++) {
                for (int k = 0; k < input[i].numPaymentMethods; k++) {
                    if (strcmp(bankList[minIdx].paymentMethods[j], bankList[i].paymentMethods[k]) == 0) {
                        strcpy(v[count], bankList[i].paymentMethods[k]);
                        count++;
                    }
                }
            }

            if (count != 0 && maxBalance < bankList[i].balance) {
                maxBalance = bankList[i].balance;
                largestIdx = i;
                strcpy(matchingMethod, v[0]);
            }
        }

        if (largestIdx == -1) {
            int maxIdx = getMaxIdx(bankList, numBanks);
            int transactionAmt = abs(bankList[minIdx].balance);
            
            printf("%s transfers Rs %d to %s using %s\n", bankList[minIdx].bankName, transactionAmt, bankList[maxIdx].bankName, bankList[minIdx].paymentMethods[0]);

            bankList[maxIdx].balance += bankList[minIdx].balance;
            bankList[minIdx].balance = 0;

            if (bankList[minIdx].balance == 0) zeroBalances++;

            if (bankList[maxIdx].balance == 0) zeroBalances++;
        } else {
            int transactionAmt = abs(bankList[minIdx].balance) < bankList[largestIdx].balance
                                    ? abs(bankList[minIdx].balance)
                                    : bankList[largestIdx].balance;

            printf("%s transfers Rs %d to %s using %s\n", bankList[minIdx].bankName, transactionAmt, bankList[largestIdx].bankName, matchingMethod);

            bankList[minIdx].balance += transactionAmt;
            bankList[largestIdx].balance -= transactionAmt;

            if (bankList[minIdx].balance == 0) zeroBalances++;
            if (bankList[largestIdx].balance == 0) zeroBalances++;
        }
    }

    printf("\n");
}

int findIndex(BankIndex bankIndices[], int numBanks, char bankName[]) {
    for (int i = 0; i < numBanks; i++) {
        if (strcmp(bankIndices[i].bankName, bankName) == 0) {
            return bankIndices[i].index;
        }
    }
    return -1; // not found
}

int main() {
    printf("\n\t\t\t\t******** Welcome to INDIAN BANK PAYMENT OPTIMIZATION SYSTEM ********\n\n\n");
    printf("This system optimizes the transaction flow among Indian banks, minimizing the number of transactions and reducing the overall balance among them. There is one central bank (with all payment methods) to act as an intermediary between banks that have no common payment method.\n\n");
    printf("Enter the number of Indian banks participating in the transactions.\n");
    int numBanks;
    scanf("%d", &numBanks);

    Bank input[MAX_BANKS];
    BankIndex bankIndices[MAX_BANKS]; // stores index of a bank

    printf("Enter the details of the banks and transactions as stated:\n");
    printf("Bank name, number of payment methods it has, and the payment methods.\n");
    printf("Bank name and payment methods should not contain spaces.\n");

    int maxNumMethods;
    for (int i = 0; i < numBanks; i++) {
        if (i == 0) {
            printf("Central Bank: ");
        } else {
            printf("Bank %d : ", i);
        }
        scanf("%s", input[i].bankName);
        bankIndices[i].index = i;
        strcpy(bankIndices[i].bankName, input[i].bankName);
        scanf("%d", &input[i].numPaymentMethods);

        if (i == 0) maxNumMethods = input[i].numPaymentMethods;

        for (int j = 0; j < input[i].numPaymentMethods; j++) {
            scanf("%s", input[i].paymentMethods[j]);
        }
    }

    printf("Enter number of transactions.\n");
    int numTransactions;
    scanf("%d", &numTransactions);

    int graph[MAX_BANKS][MAX_BANKS] = {0}; // adjacency matrix

    printf("Enter the details of each transaction as stated:\n");
    printf("Debtor Bank, creditor Bank, and amount\n");
    printf("The transactions can be in any order.\n");
    for (int i = 0; i < numTransactions; i++) {
        printf("%d th transaction: ", i);
        char s1[50], s2[50];
        int amount;
        scanf("%s %s %d", s1, s2, &amount);

        int idx1 = findIndex(bankIndices, numBanks, s1);
        int idx2 = findIndex(bankIndices, numBanks, s2);
        if (idx1 != -1 && idx2 != -1) {
            graph[idx1][idx2] = amount;
        }
    }

    optimizeCashFlow(numBanks, input, numTransactions, graph, maxNumMethods);
    return 0;
}
