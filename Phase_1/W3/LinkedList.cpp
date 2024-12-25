#include <iostream>
using namespace std;
struct Node {
    int value;
    Node *next;
};
Node *first;

Node *makeNode(int v) {
    Node *p = new Node;
    p->value = v;
    p->next = NULL;
    return p;
}

int Appear(int v, Node *r) {
    while (r != nullptr) {
        if (r->value == v)
            return 1;
        r = r->next;
    }
    return 0;
}

Node *addLast(int v, Node *h) {
    if (!Appear(v, h)) {
        Node *q = makeNode(v);
        if (first == NULL) {
            return q;
        }
        Node *p = h;
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = q;
    }
    return h;
}

void printList(Node *h) {
    Node *p = h;
    while (p != NULL) {
        cout << p->value << " ";
        p = p->next;
    }
    cout << endl;
}

Node *remove(int v, Node *&h) {
    Node *p = h;
    if (h == NULL) {
        return nullptr;
    }
    if (h->value == v) {
        Node *temp = h;
        h = h->next;
        delete temp;
        return h;
    }
    while (p->next != NULL && p->next->value != v) {
        p = p->next;
    }
    if (p->next != nullptr) {
        Node *temp = p->next;
        p->next = temp->next;
        delete temp;
    }
    return h;
}

void freeList(Node *&h) {
    while (h != NULL) {
        Node *tmp = h;
        h = h->next;
        delete tmp;
    }
}

Node *insertBefore(int v, int u, Node *h) {
    if (!Appear(v, h)) {
        if (h == NULL) {
            return h;
        }
        if (h->value == u) {
            Node *q = makeNode(v);
            q->next = h;
            return q;
        }
        Node *p = h;
        while (p->next->value != u && p->next->next != NULL) {
            p = p->next;
        }
        if (p->next->value == u) {
            Node *q = makeNode(v);
            Node *tmp = p->next;
            p->next = q;
            q->next = tmp;
        }
    }
    return h;
}

Node *insertAfter(int v, int u, Node *h) {
    if (!Appear(v, h)) {
        if (h == NULL) {
            return h;
        }
        Node *p = h;
        while (p->value != u && p->next != NULL) {
            p = p->next;
        }
        if (p->value == u) {
            Node *q = makeNode(v);
            Node *tmp = p->next;
            p->next = q;
            q->next = tmp;
        }
    }
    return h;
}

Node *insertFirst(int v, Node *h) {
    if (!Appear(v, h)) {
        Node *q = makeNode(v);
        q->next = h;
        return q;
    }
    return h;
}

Node *reverse(Node *h) {
    if (h == NULL)
        return h;
    Node *pre = NULL;
    Node *cur = h;
    Node *pos = h->next;
    while (cur != NULL) {
        pos = cur->next;
        cur->next = pre;
        pre = cur;
        cur = pos;
    }
    return pre;
}

int main() {
    string s;
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        first = addLast(x, first);
    }
    while (s.compare("#") != 0) {
        cin >> s;
        if (s.compare("addlast") == 0) {
            int x;
            cin >> x;
            first = addLast(x, first);
        } else if (s.compare("addfirst") == 0) {
            int x;
            cin >> x;
            first = insertFirst(x, first);
        } else if (s.compare("addafter") == 0) {
            int u, v;
            cin >> u >> v;
            first = insertAfter(u, v, first);
        } else if (s.compare("addbefore") == 0) {
            int u, v;
            cin >> u >> v;
            first = insertBefore(u, v, first);
        } else if (s.compare("remove") == 0) {
            int x;
            cin >> x;
            first = remove(x, first);
        } else if (s.compare("reverse") == 0) {
            first = reverse(first);
        }
    }
    printList(first);
    freeList(first);
    return 0;
}