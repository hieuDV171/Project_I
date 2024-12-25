#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

int timeToSeconds(const string &time) {
    int h, m, s;
    sscanf(time.c_str(), "%d:%d:%d", &h, &m, &s);
    return h * 3600 + m * 60 + s;
}

struct Order {
    string customerID;
    string productID;
    int price;
    string shopID;
    int timeInSeconds;

    Order(string cID, string pID, int p, string sID, int t)
        : customerID(cID), productID(pID), price(p), shopID(sID), timeInSeconds(t) {}
};

bool compareOrders(const Order &a, const Order &b) {
    return a.timeInSeconds < b.timeInSeconds;
}

int main() {
    vector<Order> orders;
    unordered_map<string, int> shopRevenue;
    unordered_map<string, unordered_map<string, int>> customerShopRevenue;
    int totalOrders = 0, totalRevenue = 0;

    string line;

    while (getline(cin, line)) {
        if (line == "#") {
            break;
        }

        istringstream iss(line);
        string customerID, productID, shopID, timePoint;
        int price;
        iss >> customerID >> productID >> price >> shopID >> timePoint;

        int timeInSeconds = timeToSeconds(timePoint);

        orders.emplace_back(customerID, productID, price, shopID, timeInSeconds);
        shopRevenue[shopID] += price;
        customerShopRevenue[customerID][shopID] += price;
        totalOrders++;
        totalRevenue += price;
    }

    sort(orders.begin(), orders.end(), compareOrders);

    vector<string> queries;

    while (getline(cin, line)) {
        if (line == "#") {
            break;
        }
        queries.push_back(line);
    }

    for (const string &query : queries) {
        istringstream iss(query);
        string command;
        iss >> command;

        if (command == "?total_number_orders") {
            cout << totalOrders << endl;
        } else if (command == "?total_revenue") {
            cout << totalRevenue << endl;
        } else if (command == "?revenue_of_shop") {
            string shopID;
            iss >> shopID;
            cout << shopRevenue[shopID] << endl;
        } else if (command == "?total_consume_of_customer_shop") {
            string customerID, shopID;
            iss >> customerID >> shopID;
            cout << customerShopRevenue[customerID][shopID] << endl;
        } else if (command == "?total_revenue_in_period") {
            string fromTime, toTime;
            iss >> fromTime >> toTime;

            int fromSeconds = timeToSeconds(fromTime);
            int toSeconds = timeToSeconds(toTime);

            int startIdx = lower_bound(orders.begin(), orders.end(), Order("", "", 0, "", fromSeconds),
                                       [](const Order &a, const Order &b) {
                                           return a.timeInSeconds < b.timeInSeconds;
                                       }) -
                           orders.begin();

            int endIdx = upper_bound(orders.begin(), orders.end(), Order("", "", 0, "", toSeconds),
                                     [](const Order &a, const Order &b) {
                                         return a.timeInSeconds < b.timeInSeconds;
                                     }) -
                         orders.begin();

            int periodRevenue = 0;
            for (int i = startIdx; i < endIdx; ++i) {
                periodRevenue += orders[i].price;
            }

            cout << periodRevenue << endl;
        }
    }

    return 0;
}
