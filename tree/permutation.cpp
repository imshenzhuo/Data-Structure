#include <cstdio>

/**
 * 
 */ 

char *B = "SEACHXM";

void print_permutation(int n, int *A, int cur, char *B) {
    if (cur == n) {
        for(int i=0; i<n; i++)
            printf("%c, ", B[A[i]]);
            // printf("%d, ", A[i]);
        printf("\n");
    }
    else for(int i = 1; i <= n; i++) {
        int ok = 1;
        for(int j=0; j<cur;j++)
            if (A[j] == i)  
                ok = 0;
        if (ok) {
            A[cur] = i;
            print_permutation(n, A, cur+1, B);
        }
    }
}

int main(int argc, char const *argv[])
{
    int A[5];
    print_permutation(6, A, 0, B);
    return 0;
}
