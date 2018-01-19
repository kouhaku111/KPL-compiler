#include <stdio.h>
#include <string.h>

char hash_array[23][10][20];
int num[23] = {0};

int get_number_string(int k) {
    for(int i = 0; i < 10; i++) {
        if(hash_array[k][i][0] == '\0') return i;
    }
}

void swap(char *str1, char *str2)
{
  char *temp = (char *)malloc((strlen(str1) + 1) * sizeof(char));
  strcpy(temp, str1);
  strcpy(str1, str2);
  strcpy(str2, temp);
  free(temp);
}  

void sort(char arr[10][20], int k) {
    int i, j, min_idx;
    for (i = 0; i < k - 1; i++) {
        min_idx = i;
        for (j = i+1; j < k; j++) 
            if (strcmp(arr[min_idx], arr[j]) > 0)
                min_idx = j;
        swap(arr[min_idx], arr[i]);
    }
}

void main() {
    const char *arr[] = {"GeeksforGeeks", "GeeksQuiz", "CLanguage"};
    int n = sizeof(arr)/sizeof(arr[0]);
    
    for (int i = 0; i < n; i++) {
        int j = arr[i][0] - 65;
        strcpy(hash_array[j][num[j]], arr[i]);
        num[j] += 1;
    }

    // n = get_number_string(0);
    // sort(hash_array[0], n);
    // for (int j = 0; j < n; j++)
    //     printf("%d: %s \n", j, hash_array[0][j]);

    for(int i = 0; i < 23; i++) {
        int n = get_number_string(i);
        if (n != 0) {
            sort(hash_array[i], n);
            for (int j = 0; j < n; j++)
                 printf("%s \n", hash_array[i][j]);
        }
    }

}
