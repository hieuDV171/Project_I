#include <algorithm>
#include <iostream>
#include <sstream>
#include <stack>
#include <vector>
using namespace std;
struct DoB {
    int Y;
    int M;
    int D;
};
struct Citizen {
    int code;
    DoB dob;
    int father_code;
    int mother_code;
    char is_alive;
    int region_code;

    Citizen(int code, DoB dob, int fCode,
            int mCode, char iA, int rCode) : code(code), dob(dob), father_code(fCode), mother_code(mCode),
                                             is_alive(iA), region_code(rCode) {}
    Citizen() {}
};
struct State {
    int cur_index;
    int depth;
};
struct Node {
    int value;
    Node *next;

    Node(int val) : value(val), next(nullptr) {}
};
const int MAX = 100001;
Node *ChildList[MAX];
int iDeg[MAX];
bool visited[MAX];
vector<Citizen> citizens;
vector<Citizen> sortedPeople;
int numCitizens = 0;

DoB convertDoB(string dobStr) {
    DoB T;
    T.Y = (dobStr[0] - 48) * 1000 + (dobStr[1] - 48) * 100 + (dobStr[2] - 48) * 10 + (dobStr[3] - 48);
    T.M = (dobStr[5] - 48) * 10 + (dobStr[6] - 48);
    T.D = (dobStr[8] - 48) * 10 + (dobStr[9] - 48);
    return T;
}

int convertCode(string StrCode) {
    int l = StrCode.length();
    int code = 0, m = 1;
    for (int i = l - 1; i >= 0; i--) {
        code += (StrCode[i] - 48) * m;
        m *= 10;
    }
    return code;
}

bool compare(const Citizen &a, const Citizen &b) {
    if (a.dob.Y != b.dob.Y) {
        return a.dob.Y < b.dob.Y;
    }
    if (a.dob.M != b.dob.M) {
        return a.dob.M < b.dob.M;
    }
    return a.dob.D < b.dob.D;
}

void input() {
    string line;
    while (getline(cin, line)) {
        if (line == "*") {
            break;
        }
        istringstream iss(line);
        string StrCode, StrFCode, StrMCode, StrRCode, dobStr;
        int code, fCode, mCode, rCode;
        DoB dob;
        char iA;
        iss >> StrCode >> dobStr >> StrFCode >> StrMCode >> iA >> StrRCode;
        dob = convertDoB(dobStr);
        code = convertCode(StrCode);
        fCode = convertCode(StrFCode);
        mCode = convertCode(StrMCode);
        rCode = convertCode(StrRCode);

        Citizen newCitizen(code, dob, fCode, mCode, iA, rCode);
        citizens.emplace_back(newCitizen);
        sortedPeople.emplace_back(newCitizen);
        numCitizens++;
    }
    sort(sortedPeople.begin(), sortedPeople.end(), compare);
    // for (auto it : sortedPeople) {
    //     cout << it.dob.Y << ":" << it.dob.M << ":" << it.dob.D << endl;
    // }
}

int NUMBER_PEOPLE_BORN_AT(string date) {
    int cnt = 0;
    DoB at = convertDoB(date);
    int atIndex = lower_bound(sortedPeople.begin(), sortedPeople.end(), Citizen(0, at, 0, 0, ' ', 0), [](const Citizen &a, Citizen people) {
                      if (a.dob.Y != people.dob.Y) {
                          return a.dob.Y < people.dob.Y;
                      }
                      if (a.dob.M != people.dob.M) {
                          return a.dob.M < people.dob.M;
                      }
                      return a.dob.D < people.dob.D;
                  }) -
                  sortedPeople.begin();
    // cout << sortedPeople[atIndex].dob.Y << ":" << sortedPeople[atIndex].dob.M << ":" << sortedPeople[atIndex].dob.D << endl;
    while ((sortedPeople[atIndex].dob.Y == at.Y) && (sortedPeople[atIndex].dob.M == at.M) && (sortedPeople[atIndex].dob.D == at.D)) {
        cnt++;
        atIndex++;
    }
    return cnt;
}

// int bSearch(int code) {
//     int L = 0, R = numCitizens - 1;
//     if (L == R && citizens[L].code == code) {
//         return L;
//     }
//     int mid;
//     while (L <= R) {
//         mid = (L + R) / 2;
//         if (citizens[mid].code == code) {
//             return mid;
//         } else if (citizens[mid].code < code) {
//             L = mid + 1;
//         } else if (citizens[mid].code > code) {
//             R = mid - 1;
//         }
//     }
//     return -1;
// }

int MOST_ALIVE_ANCESTOR(string sCode) {
    int maxDepth = 0;
    int oCode = convertCode(sCode);
    int startIndex = oCode - 1;

    if (startIndex == -1) {
        return 0;
    }

    stack<State> AnsStack;
    AnsStack.push({startIndex, 0});
    while (!AnsStack.empty()) {
        auto [curIndex, depth] = AnsStack.top();
        AnsStack.pop();
        maxDepth = max(maxDepth, depth);

        int mCode = citizens[curIndex].mother_code;
        int mIndex = mCode - 1;
        if (mIndex != -1) {
            AnsStack.push({mIndex, depth + 1});
        }

        int fCode = citizens[curIndex].father_code;
        int fIndex = fCode - 1;
        if (fIndex != -1) {
            AnsStack.push({fIndex, depth + 1});
        }
    }
    return maxDepth;
}

int NUMBER_PEOPLE_BORN_BETWEEN(string fromStr, string toStr) {
    DoB from = convertDoB(fromStr);
    DoB to = convertDoB(toStr);

    int startIndex = lower_bound(sortedPeople.begin(), sortedPeople.end(), Citizen(0, from, 0, 0, ' ', 0), [](const Citizen &a, Citizen people) {
                         if (a.dob.Y != people.dob.Y) {
                             return a.dob.Y < people.dob.Y;
                         }
                         if (a.dob.M != people.dob.M) {
                             return a.dob.M < people.dob.M;
                         }
                         return a.dob.D < people.dob.D;
                     }) -
                     sortedPeople.begin();
    int endIndex = upper_bound(sortedPeople.begin(), sortedPeople.end(), Citizen(0, to, 0, 0, ' ', 0), [](Citizen people, const Citizen &b) {
                       if (people.dob.Y != b.dob.Y) {
                           return people.dob.Y < b.dob.Y;
                       }
                       if (people.dob.M != b.dob.M) {
                           return people.dob.M < b.dob.M;
                       }
                       return people.dob.D < b.dob.D;
                   }) -
                   sortedPeople.begin();
    //
    // cout << startIndex << ":" << endIndex << endl;
    return endIndex - startIndex;
}

// void makeSet() {
//     for (int i = 0; i < numCitizens; i++) {
//         Parent[i] = i;
//         Rank[i] = 1;
//     }
// }

// int findRoot(int x) {
//     if (Parent[x] != x) {
//         Parent[x] = findRoot(Parent[x]);
//     }
//     return Parent[x];
// }

// void unify(int r1, int r2) {
//     if (Rank[r1] > Rank[r2]) {
//         Parent[r2] = r1;
//     } else {
//         Parent[r1] = r2;
//         if (Rank[r2] == Rank[r1]) {
//             Rank[r2]++;
//         }
//     }
// }

// Node *search(Node *r, int u) {
//     if (r == nullptr)
//         return nullptr;
//     Node *q = r;
//     while (q != nullptr && q->value != u) {
//         q = q->next;
//     }
//     return q;
// }

Node *add(Node *r, int v) {
    Node *p = new Node(v);
    if (r == nullptr) {
        r = p;
    } else {
        p->next = r;
        r = p;
    }
    return r;
}

int MAX_UNRELATED_PEOPLE() {
    int cnt = 0;
    for (int i = 0; i <= numCitizens; i++) {
        iDeg[i] = 0;
        visited[i] = false;
    }
    for (int i = 1; i <= numCitizens; i++) {
        int dad = citizens[i - 1].father_code;
        int mom = citizens[i - 1].mother_code;
        if (dad != 0) {
            ChildList[dad] = add(ChildList[dad], i);
            iDeg[i]++;
        }
        if (mom != 0) {
            ChildList[mom] = add(ChildList[mom], i);
            iDeg[i]++;
        }
    }
    int nVisitedNode = 0;
    while (nVisitedNode != numCitizens) {
        for (int i = 1; i <= numCitizens; i++) {
            if (iDeg[i] == 0 && !visited[i]) {
                visited[i] = true;
                nVisitedNode++;
                cnt++;
                Node *p = ChildList[i];
                while (p != nullptr) {
                    if (!visited[p->value]) {
                        visited[p->value] = true;
                        nVisitedNode++;
                        Node *q = ChildList[p->value];
                        while (q != nullptr) {
                            iDeg[q->value]--;
                            q = q->next;
                        }
                    }
                    p = p->next;
                }
            }
        }
    }
    return cnt;
};

void query() {
    string line;
    while (getline(cin, line)) {
        if (line == "***") {
            break;
        }
        istringstream iss(line);
        string command;
        iss >> command;
        if (command.compare("NUMBER_PEOPLE") == 0) {
            cout << numCitizens << endl;
        } else if (command.compare("NUMBER_PEOPLE_BORN_AT") == 0) {
            string date;
            iss >> date;
            cout << NUMBER_PEOPLE_BORN_AT(date) << endl;
        } else if (command.compare("MAX_UNRELATED_PEOPLE") == 0) {
            cout << MAX_UNRELATED_PEOPLE() << endl;
        } else if (command.compare("MOST_ALIVE_ANCESTOR") == 0) {
            string sCode;
            iss >> sCode;
            cout << MOST_ALIVE_ANCESTOR(sCode) << endl;
        } else if (command.compare("NUMBER_PEOPLE_BORN_BETWEEN") == 0) {
            string fromStr, toStr;
            iss >> fromStr >> toStr;
            cout << NUMBER_PEOPLE_BORN_BETWEEN(fromStr, toStr) << endl;
        }
    }
}

int main() {
    input();
    query();
    return 0;
}