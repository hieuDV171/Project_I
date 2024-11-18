#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;
struct TP {
    int H;
    int M;
    int S;
};
struct Submission {
    int UserID;
    int ProblemID;
    TP TimePoint;
    bool status;
    int Points;

    Submission(int uID, int pID, TP Time,
               bool status, int points) : UserID(uID), ProblemID(pID), TimePoint(Time),
                                          status(status), Points(points) {}
};
const int N = 100000;
int numSubmit = 0;
int numErrSubmit = 0;
vector<Submission> TPSort;
vector<Submission> UserSort;

TP convertTP(string StrTP) {
    TP T;
    T.H = (StrTP[0] - 48) * 10 + (StrTP[1] - 48);
    T.M = (StrTP[3] - 48) * 10 + (StrTP[4] - 48);
    T.S = (StrTP[6] - 48) * 10 + (StrTP[7] - 48);
    return T;
}

int convertID(string StrID) {
    int l = StrID.length();
    int ID = 0, m = 1;
    for (int i = l - 1; i > 0; i--) {
        ID += (StrID[i] - 48) * m;
        m *= 10;
    }
    return ID;
}

bool compare(const Submission &a, const Submission &b) {
    if (a.TimePoint.H != b.TimePoint.H) {
        return a.TimePoint.H < b.TimePoint.H;
    }
    if (a.TimePoint.M != b.TimePoint.M) {
        return a.TimePoint.M < b.TimePoint.M;
    }
    return a.TimePoint.S < b.TimePoint.S;
}

bool userCompare(const Submission &a, const Submission &b) {
    return a.UserID < b.UserID;
}

void input() {
    string line;
    while (getline(cin, line)) {
        if (line == "#") {
            break;
        }
        istringstream iss(line);
        string StrUserID, StrProblemID, StrTimePoint, StrStatus;
        int UserID, ProblemID, Points;
        bool Status;
        TP TimePoint;
        iss >> StrUserID >> StrProblemID >> StrTimePoint >> StrStatus >> Points;
        TimePoint = convertTP(StrTimePoint);
        UserID = convertID(StrUserID);
        ProblemID = convertID(StrProblemID);
        if (StrStatus.compare("ERR") == 0)
            Status = false;
        else
            Status = true;
        Submission newSubmission(UserID, ProblemID, TimePoint, Status, Points);
        TPSort.emplace_back(newSubmission);
        UserSort.emplace_back(newSubmission);

        if (Status == false)
            numErrSubmit++;
        numSubmit++;
    }
    sort(TPSort.begin(), TPSort.end(), compare);
    sort(UserSort.begin(), UserSort.end(), userCompare);
    // for (auto it : UserSort) {
    //     cout << it.UserID << ":" << it.ProblemID << ":" << it.Points << endl;
    // }
}

int Lower_bound(int user) {
    int L = 0, R = numSubmit - 1;
    int mid;
    while (L < R) {
        mid = L + (R - L) / 2;
        if (UserSort[mid].UserID >= user) {
            R = mid;
        } else if (UserSort[mid].UserID < user) {
            L = mid + 1;
        }
    }
    return L;
}

int number_error_submission_of_user(string StrUID) {
    int cnt = 0;
    int user = convertID(StrUID);
    int startIndex = lower_bound(UserSort.begin(), UserSort.end(), user, [](const Submission &a, int user) {
                      return a.UserID < user;
                  }) -
                  UserSort.begin();
    // int startIndex = Lower_bound(user);
    while (UserSort[startIndex].UserID == user) {
        if (UserSort[startIndex].status == false)
            cnt++;
        startIndex++;
    }
    return cnt;
}

int total_point_of_user(string sUID) {
    int sum = 0;
    int user = convertID(sUID);
    int startIndex = lower_bound(UserSort.begin(), UserSort.end(), user, [](const Submission &a, int user) {
                         return a.UserID < user;
                     }) -
                     UserSort.begin();
    // int startIndex = Lower_bound(user);
    int i = 0;
    int p4P[1000] = {0};
    while (UserSort[startIndex].UserID == user) {
        int pID = UserSort[startIndex].ProblemID;
        int pts = UserSort[startIndex].Points;
        p4P[pID] = (p4P[pID] > pts) ? p4P[pID] : pts;
        startIndex++;
    }
    for (int i : p4P) {
        if (i != 0) {
            sum += i;
        }
    }
    return sum;
}

int number_submission_period(string fromStr, string toStr) {
    TP from = convertTP(fromStr);
    TP to = convertTP(toStr);

    int startIndex = lower_bound(TPSort.begin(), TPSort.end(), from, [](const Submission &a, TP from) {
                         if (a.TimePoint.H != from.H) {
                             return a.TimePoint.H < from.H;
                         }
                         if (a.TimePoint.M != from.M) {
                             return a.TimePoint.M < from.M;
                         }
                         return a.TimePoint.S < from.S;
                     }) -
                     TPSort.begin();
    int endIndex = upper_bound(TPSort.begin(), TPSort.end(), to, [](TP to, const Submission &b) {
                       if (to.H != b.TimePoint.H) {
                           return to.H < b.TimePoint.H;
                       }
                       if (to.M != b.TimePoint.M) {
                           return to.M < b.TimePoint.M;
                       }
                       return to.S < b.TimePoint.S;
                   }) -
                   TPSort.begin();
    return endIndex - startIndex;
}

void query() {
    string line;
    while (getline(cin, line)) {
        if (line == "#") {
            break;
        }
        istringstream iss(line);
        string command;
        iss >> command;
        if (command.compare("?total_number_submissions") == 0) {
            cout << numSubmit << endl;
        } else if (command.compare("?number_error_submision") == 0) {
            cout << numErrSubmit << endl;
        } else if (command.compare("?number_error_submision_of_user") == 0) {
            string StrUID;
            iss >> StrUID;
            cout << number_error_submission_of_user(StrUID) << endl;
        } else if (command.compare("?total_point_of_user") == 0) {
            string sUID;
            iss >> sUID;
            cout << total_point_of_user(sUID) << endl;
        } else if (command.compare("?number_submission_period") == 0) {
            string fromStr, toStr;
            iss >> fromStr >> toStr;
            cout << number_submission_period(fromStr, toStr) << endl;
        }
    }
}

int main() {
    input();
    query();
    return 0;
}