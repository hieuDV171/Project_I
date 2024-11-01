#include <iostream>
using namespace std;

void swap(int &a, int &b) {
    int tmp = a;
    a = b;
    b = tmp;
}

void heapify(int A[], int i, int size) {
    int L = 2 * i + 1;
    int R = 2 * i + 2;
    int max = i;
    if (L < size && A[L] > A[i])
        max = L;
    if (R < size && A[R] > A[max])
        max = R;
    if (max != i) {
        swap(A[max], A[i]);
        heapify(A, max, size);
    }
}

void buildHeap(int A[], int size) {
    for (int i = size / 2 - 1; i >= 0; i--) {
        heapify(A, i, size);
    }
}

void heapSort(int A[], int size) {
    buildHeap(A, size);
    for (int i = size - 1; i > 0; i--) {
        swap(A[0], A[i]);
        heapify(A, 0, i);
    }
}

int main() {
    int n, M;
    cin >> n >> M;
    int A[n];
    for (int i = 0; i < n; i++) {
        cin >> A[i];
    }
    heapSort(A, n);
    int L = 0;
    int R = n - 1;
    int cnt = 0;
    while (L < R) {
        int sum = A[L] + A[R];
        if (sum == M) {
            cnt++;
            L++;
            R--;
        } else if (sum < M) {
            L++;
        } else if (sum > M) {
            R--;
        }
    }
    cout << cnt << endl;
    return 0;
}