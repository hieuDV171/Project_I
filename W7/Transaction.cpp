/*
The data about bank transactions consists of a sequence of transactions: the information of each transaction has the following format:
                                                                    <from_account>   <to_account>   <money>   <time_point>   <atm>
In which:
•	<from_account>: the account from which money is transferred (which is a string of length from 6 to 20 )
•	<to_account>: the account which receives money in the transaction (which is a string of length from 6 to 20)
•	<money>: amount of money transferred in the transaction (which is an integer from 1 to 10000)
•	<time_point>: the time point at which the transaction is performed, it is a string under the format HH:MM:SS  (hour: minute: second)
•	<atm>: the code of the ATM where the transaction is taken (a string of length from 3 to 10)
Example: T00112233445 T001234002 2000 08:36:25 BIDV (at the ATM BIDV, account T00112233445 transfers 2000$ to account T001234002 at time point 08:36:25 (08 hour, 36 minutes, 25 seconds)
A transaction cycle of length k starting from account a1 is defined to be a sequence of distinct account a1, a2, …, ak  in which there are transactions from account a1 to a2, from a2 to a3, …, from ak to a1.
Write a program that process the following queries:
?number_transactions: compute the total number of transactions of the data
?total_money_transaction: compute the total amount of money of transactions
?list_sorted_accounts: compute the sequence of bank accounts (including sending and receiving accounts) appearing in the transaction (sorted in an increasing (alphabetical) order)
?total_money_transaction_from <account>: compute the total amount of money transferred from the account <account>
?inspect_cycle <account> k : return 1 if there is a transaction cycle of length k, starting from <account>, and return 0, otherwise
Input (stdin)
The input consists of 2 blocks of information: the data block and the query block
•	The data block consists of lines:
o	Each line contains the information about a transaction described above
o	The data is terminated by a line containing #
•	The query block consists of lines:
o	Each line is a query described above
o	The query block is terminated by a line containing #

Output (stdout)
•	Print to stdout (in each line) the result of each query described above

Example
Input
T000010010 T000010020 1000 10:20:30 ATM1
T000010010 T000010030 2000 10:02:30 ATM2
T000010010 T000010040 1500 09:23:30 ATM1
T000010020 T000010030 3000 08:20:31 ATM1
T000010030 T000010010 4000 12:40:00 ATM2
T000010040 T000010010 2000 10:30:00 ATM1
T000010020 T000010040 3000 08:20:31 ATM1
T000010040 T000010030 2000 11:30:00 ATM1
T000010040 T000010030 1000 18:30:00 ATM1
#
?number_transactions
?total_money_transaction
?list_sorted_accounts
?total_money_transaction_from T000010010
?inspect_cycle T000010010 3
#
Output
9
19500
T000010010 T000010020 T000010030 T000010040
4500
1
*/
#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string.h>
using namespace std;
struct TP {
    int H;
    int M;
    int S;

    int to_seconds() const {
        return H * 3600 + M * 60 + S;
    }
};
struct Bill {
    unsigned long long fAcc;
    unsigned long long tAcc;
    int money;
    TP timePoint;
    int atm;
};
vector<Bill> TransData;
long long total_money_transaction = 0;
TP convertTP(string time) {
    TP T;
    T.H = (time[0] - '0') * 10 + (time[1] - '0');
    T.M = (time[3] - '0') * 10 + (time[4] - '0');
    T.S = (time[6] - '0') * 10 + (time[7] - '0');
    return T;
}

bool compare(const Bill &a, const Bill &b) {
    return a.timePoint.to_seconds() < b.timePoint.to_seconds();
}

void input() {
    char fromAccount[21], toAccount[21], time_point[9], atm[11];
    int Money;
    char buffer[30];
    while (scanf("%s", buffer) == 1) {
        if (buffer[0] == '#') {
            break;
        }

        sscanf(buffer, "%s", fromAccount);
        scanf("%s %d %s %s", toAccount, &Money, time_point, atm);
    }
    sort(TransData.begin(), TransData.end(), compare);
}

void query() {
    char command[50];
    while (scanf("%s", command) == 1) {
        if (command[0] == '#') {
            break;
        }
        if (strcmp(command, "?number_transactions") == 0) {
            printf("%lu\n", TransData.size());
        } else if (strcmp(command, "?total_money_transaction") == 0) {
            printf("%lld\n", total_money_transaction);
        } else if (strcmp(command, "?list_sorted_accounts") == 0) {
            printf("%lld\n");
        } else if (strcmp(command, "?total_money_transaction_from") == 0) {
            char fromAcc[21];
            scanf("%s", fromAcc);
            printf("%lld\n");
        } else if (strcmp(command, "?inspect_cycle") == 0) {
            char Acc[21];
            int k;
            scanf("%s %d", Acc, k);
            printf("%lld\n");
        }
    }
}

int main() {
    return 0;
}