#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static int CMP_CNT, SWAP_CNT;

// Custom comparator (with sorting stats)
static int Cmp(int a, int b) {
    ++CMP_CNT;
    return a < b;
}

// Custom swap (with sorting stats)
static void Swap(int *a, int *b) {
    if (a != b) {
        ++SWAP_CNT;
        int tmp = *a;
        *a = *b;
        *b = tmp;
    }
}

// Less comparator (for qsort)
static int Less(const void *pa, const void *pb) {
    int a = *(int *)pa, b = *(int *)pb;
    return (b <= a) - (a <= b);
}

// Greater comparator (for qsort)
static int Greater(const void *pa, const void *pb) {
    int a = *(int *)pa, b = *(int *)pb;
    return (a <= b) - (b <= a);
}


// Generate pivot (median of three random array elements)
static int GetPivot(const int *a, int n) {
    int x = a[rand() % n], y = a[rand() % n], z = a[rand() % n];
    if (x < y) {
        return x < z ? (y < z ? y : z) : x;
    }
    return y < z ? (x < z ? x : z) : y;
}

// Generate random integer
static int GetRand(void) {
    return (rand() << 30) ^ (rand() << 15) ^ rand();
}

// Generate ascending array
static void GenAscending(int *a, int n) {
    for (int i = 0; i < n; ++i) {
        a[i] = GetRand();
    }
    qsort(a, n, sizeof(int), Less);
}

// Generate descending array
static void GenDescending(int *a, int n) {
    for (int i = 0; i < n; ++i) {
        a[i] = GetRand();
    }
    qsort(a, n, sizeof(int), Greater);
}

// Generate random array
static void GenRandom(int *a, int n) {
    for (int i = 0; i < n; ++i) {
        a[i] = GetRand();
    }
}

// SelectSort implementation
static void SelectSort(int *a, int n) {
    for (int idx, i = 0; i < n - 1; ++i) {
        idx = i;
        for (int j = i + 1; j < n; ++j) {
            if (Cmp(a[j], a[idx])) {
                idx = j;
            }
        }
        Swap(a + i, a + idx);
    }
}

// Recursive QuickSort implementation
static void QuickSort(int *a, int n) {
    int i = 0, j = n - 1, pivot = GetPivot(a, n);
    do {
        while (i < n - 1 && Cmp(a[i], pivot)) {
            ++i;
        }
        while (j > 0 && Cmp(pivot, a[j])) {
            --j;
        }
        if (i <= j) {
            Swap(a + i++, a + j--);
        }
    } while (i <= j);
    if (j > 0) {
        QuickSort(a, j + 1);
    }
    if (i < n - 1) {
        QuickSort(a + i, n - i);
    }
}

// Check sorting function
static int SortCheck(const int *a, const int *sorted, int n) {
    int *copy = malloc(n * sizeof(int));
    memcpy(copy, a, n * sizeof(int));
    qsort(copy, n, sizeof(int), Less);
    for (int i = 0; i < n; ++i) {
        if (copy[i] != sorted[i]) {
            free(copy);
            return 0;
        }
    }
    free(copy);
    return 1;
}

// Launch sorting function
static void LaunchSort(const char *name, void (*sort)(int *, int), const int *a, int n) {
    int *copy = malloc(n * sizeof(int));
    memcpy(copy, a, n * sizeof(int));
    CMP_CNT = 0, SWAP_CNT = 0;
    sort(copy, n);
    printf("%s stats: %d comparisons, %d swaps\n", name, CMP_CNT, SWAP_CNT);
    printf("%s check: %s\n", name, SortCheck(a, copy, n) ? "correct" : "incorrect");
    free(copy);
}

// Launch tests for a set array type
static void LaunchTests(const char *type, void (*generate)(int *, int), int n) {
    int *a = malloc(n * sizeof(int));
    generate(a, n);
    printf("\n%s\n", type);
    LaunchSort("SelectSort", SelectSort, a, n);
    LaunchSort("QuickSort", QuickSort, a, n);
    free(a);
}

int main(void) {
    srand(time(NULL));
    int n;
    if (scanf("%d", &n) != 1 || n < 1 || n > 10000) {
        printf("Enter a natural number from 1 to 10000 :(\n");
        return 1;
    }
    LaunchTests("Ascending array:", GenAscending, n);
    LaunchTests("Descending array:", GenDescending, n);
    LaunchTests("Random array 1:", GenRandom, n);
    LaunchTests("Random array 2:", GenRandom, n);
    return 0;
}
