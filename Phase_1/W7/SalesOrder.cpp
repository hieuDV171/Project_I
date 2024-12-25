#include <algorithm>
#include <stdio.h>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string.h>
using namespace std;
const int MAX_N = 100000;
struct TimePoint {
    int H;
    int M;
    int S;

    int to_seconds() const {
        return H * 3600 + M * 60 + S;
    }
};
struct Order {
    int CustomerID, ProductID, ShopID, Price;
    TimePoint timePoint;
};

vector<Order> orders;
unordered_map<int, long long> shopRevenue;
unordered_map<int, unordered_map<int, long long>> customerShopRevenue;
int numOrders = 0;
long long totalRevenue = 0;

TimePoint convert(const char *Time) {
    TimePoint T;
    T.H = (Time[0] - 48) * 10 + (Time[1] - 48);
    T.M = (Time[3] - 48) * 10 + (Time[4] - 48);
    T.S = (Time[6] - 48) * 10 + (Time[7] - 48);
    return T;
}

int convertID(string sID) {
    int l = sID.length();
    int m = 1, T = 0;
    for (int i = l - 1; i > 0; i--) {
        T += (sID[i] - '0') * m;
        m *= 10;
    }
    return T;
}

bool compare(const Order &a, const Order &b) {
    return a.timePoint.to_seconds() < b.timePoint.to_seconds();
}

void input() {
    char customerID[12], productID[12], shopID[12], timePoint[9];
    int price;
    char buffer[50];
    while (scanf("%s", buffer) == 1) {
        if (buffer[0] == '#') {
            break;
        }

        sscanf(buffer, "%s", customerID);
        scanf("%s %d %s %s", productID, &price, shopID, timePoint);

        TimePoint t = convert(timePoint);
        int cID = convertID(customerID);
        int pID = convertID(productID);
        int sID = convertID(shopID);

        orders.push_back({cID, pID, sID, price, t});

        shopRevenue[sID] += price;
        customerShopRevenue[cID][sID] += price;
        totalRevenue += price;
        numOrders++;
    }
    sort(orders.begin(), orders.end(), compare);
}

vector<long long> prefixSum(86400, 0);

void build_prefix_sum() {
    for (const Order &order : orders) {
        int orderSec = order.timePoint.to_seconds();
        prefixSum[orderSec] += order.Price;
    }

    for (int i = 1; i < 86400; ++i) {
        prefixSum[i] += prefixSum[i - 1];
    }
}

long long total_revenue_in_period(int fromSec, int toSec) {
    if (fromSec > 0) {
        return prefixSum[toSec] - prefixSum[fromSec - 1];
    } else {
        return prefixSum[toSec];
    }
}

void query() {
    char command[50];

    while (scanf("%s", command) == 1) {

        if (command[0] == '#')
            break;

        if (strcmp(command, "?total_number_orders") == 0) {
            printf("%lu\n", orders.size());
        } else if (strcmp(command, "?total_revenue") == 0) {
            printf("%lld\n", totalRevenue);
        } else if (strcmp(command, "?revenue_of_shop") == 0) {
            char shop[12];
            scanf("%s", shop);
            int sID = convertID(shop);
            printf("%lld\n", shopRevenue[sID]);
        } else if (strcmp(command, "?total_consume_of_customer_shop") == 0) {
            char customer[12], shop[12];
            scanf("%s %s", customer, shop);
            int cID = convertID(customer);
            int sID = convertID(shop);
            printf("%lld\n", customerShopRevenue[cID][sID]);
        } else if (strcmp(command, "?total_revenue_in_period") == 0) {
            char from[9], to[9];
            scanf("%s %s", from, to);
            TimePoint fromTime = convert(from);
            TimePoint toTime = convert(to);
            int fromSec = fromTime.to_seconds();
            int toSec = toTime.to_seconds();
            printf("%lld\n", total_revenue_in_period(fromSec, toSec));
        }
    }
}

main() {
    input();
    build_prefix_sum();
    query();
    return 0;
}