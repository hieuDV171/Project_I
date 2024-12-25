#include <cstdio>
#include <iostream>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <functional>
#include <string.h>
using namespace std;

struct Transaction {
    int from;
    int to;
    int money;
    int time;
    int atm;
};

int convertAcc(string acc) {
    int l = acc.length();
    int T = 0, m = 1;
    for (int i = l - 1; i > 0; i--) {
        T += (acc[i] - '0') * m;
        m *= 10;
    }
    return T;
}

int convertATM(string atm) {
    int l = atm.length();
    int T = 0, m = 1;
    for (int i = l - 1; i > 2; i--) {
        T += (atm[i] - '0') * m;
        m *= 10;
    }
    return T;
}

int convertTime(string time) {
    int H = (time[0] - 48) * 10 + (time[1] - 48);
    int M = (time[3] - 48) * 10 + (time[4] - 48);
    int S = (time[6] - 48) * 10 + (time[7] - 48);
    return H * 3600 + M * 60 + S;
}

int main() {
    vector<Transaction> transactions;
    unordered_map<int, long long> total_money_from;
    set<string> accounts;
    long long total_money = 0;
    int transaction_count = 0;

    char from[21], to[21], time[9], atm[11];
    int money;

    while (true) {
        scanf("%s", from);
        if (strcmp(from, "#") == 0)
            break;
        scanf("%s %d %s %s", to, &money, time, atm);
        int fAcc = convertAcc(from);
        int tAcc = convertAcc(to);
        int Time = convertTime(time);
        int ATM = convertATM(atm);
        transactions.push_back({fAcc, tAcc, money, Time, ATM});

        transaction_count++;
        total_money += money;
        total_money_from[fAcc] += money;

        accounts.insert(from);
        accounts.insert(to);
    }

    unordered_map<int, vector<int>> graph;
    for (const auto &transaction : transactions) {
        graph[transaction.from].push_back(transaction.to);
    }

    char query[50];
    while (true) {
        scanf("%s", query);
        if (query[0] == '#')
            break;

        if (strcmp(query, "?number_transactions") == 0) {
            printf("%d\n", transaction_count);
        } else if (strcmp(query, "?total_money_transaction") == 0) {
            printf("%lld\n", total_money);
        } else if (strcmp(query, "?list_sorted_accounts") == 0) {
            for (const auto &account : accounts) {
                printf("%s ", account.c_str());
            }
            printf("\n");
        } else if (strcmp(query, "?total_money_transaction_from") == 0) {
            char account[21];
            scanf("%s", account);
            int acc = convertAcc(account);
            printf("%lld\n", total_money_from[acc]);
        } else if (strcmp(query, "?inspect_cycle") == 0) {
            char start[21];
            int k;
            scanf("%s %d", start, &k);
            int begin = convertAcc(start);

            unordered_set<int> visited;
            function<bool(const int &, const int &, int)> dfs = [&](const int &start, const int &current, int depth) {
                if (depth == k)
                    return current == start;
                if (visited.count(current))
                    return false;

                visited.insert(current);
                for (const auto &neighbor : graph[current]) {
                    if (dfs(start, neighbor, depth + 1)) {
                        return true;
                    }
                }
                visited.erase(current);
                return false;
            };

            printf("%d\n", dfs(begin, begin, 0) ? 1 : 0);
        }
    }
    return 0;
}