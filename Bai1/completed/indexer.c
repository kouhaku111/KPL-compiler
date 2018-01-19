#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct data {
    char word[20];
    int num;    // so lan xuat hien
    char lnum[100][3];  // cac dong xuat hien, toi da co 100 dong
}list;

list stop_list[30]; // toi da co 30 tu cam
int stop_range = 0;
list mnf_list[50];  // toi da co 50 tu co nghia
int mnf_range = 0;

list hash_list[23][20]; // toi da 20 tu co nghia ung voi cac chu cai dau tu a->z
int num[23] = {0};  // so luong tu ung voi cac chu cai dau tu a->z

void init() {
    for(int i = 0; i < 23; i++) {   
        for (int j = 0; j < 20; j++)
        {
            hash_list[i][j].num = 0;
        }
    }
}

int get_num_line(int i) {   // lay so luong cac dong xuat hien 
    for(int j = 0; j < 100; j++) {
        if(mnf_list[i].lnum[j][0] == '\0') return j;
    }
}

void to_lower(char buf[]) {
    int i = 0;
     
    while(buf[i] != '\0') { 
        if(buf[i] >= 'A' && buf[i] <= 'Z') buf[i] += 32;
        i++;
    }    
}

int check_stop_list(char buf[]) {   // ton tai thi tra lai 1, 0 trong th nguoc lai
    int i = 0;

    to_lower(buf);
    while(i < stop_range) {
        if(strcmp(stop_list[i++].word, buf) == 0) return 1;
    }
    return 0;
}

int check_mnf_list(char buf[]) {    //  tra ve vi tri tu duoc them vao hoac update trong mang cac ki tu dau 
    int i = 0;
    int k = buf[0] - 97;
    int range = num[k];
    //printf("range = %d\n", range);
    //printf("%d\n", k);
    //strcpy(hash_list[k][num[k]].word, buf);
    //printf("%s\n", hash_list[k][num[k]].word);
    //printf("%d\n", range);
    while(i < range) {
        //printf("1\n");
        if(strcmp(hash_list[k][i].word, buf) == 0) {    // neu da ton tai trong day
            hash_list[k][i].num += 1;
            //printf("%s %d\n", hash_list[k][i].word, hash_list[k][i].num);
            return i;
        }
        i++;
    }
    strcpy(hash_list[k][i].word, buf);  // neu chua ton tai
    num[k] += 1;
    return i;
}

void push(FILE *f, int op) {
    char line[255];
    char buffer[20];
    int index = 0;
    int j = 0;
    int line_no = 1;    
    while(fgets(line, 255, f)) {
        for(int i = 0; i < strlen(line); i++) {
            if((line[i] >= 'a' && line[i] <= 'z')) {    // neu gap ki tu a->z, copy vao buffer
                buffer[j++] = line[i];
            }
            else if(line[i] >= 'A' && line[i] <= 'Z') { // neu gap ki tu A->Z
                if(i != 0) {    // neu khong phai ki tu dau tien cua 1 dong
                    if(line[i-2] != '.') {  // la danh tu rieng
                        while(line[i+1] >= 'a' && line[i+1] <= 'z') i++;    //  i tang toi khi het mot tu
                    } else buffer[j++] = line[i];   // khong phai la danh tu rieng 
                }
                else {  //  neu la ki tu dau tien cua 1 dong
                    buffer[j++] = line[i];    
                }
            }
            else {
                buffer[j] = '\0'; 
                if(*buffer != '\0') {
                    if(op == 1) {   // xay dung stop list
                        strcpy(stop_list[index++].word, buffer);
                        stop_range += 1;
                    }
                    else if(op == 2) {
                        if(!check_stop_list(buffer)) {  // neu khong ton tai trong stop_list
                            // int k = check_mnf_list(buffer); // lay ra vi tri tu dang xet trong list
                            // char s[3];
                            // sprintf(s, "%d", line_no);  // chuyen so dong sang string
                            // int w = 0;
                            // int n = get_num_line(k);    
                            // while(w < n) {
                            //     if(strcmp(mnf_list[k].lnum[w++], s) == 0) break;
                            // }
                            // if(w == n) strcpy(mnf_list[k].lnum[w], s);
                            check_mnf_list(buffer);
                        }
                    }
                    j = 0;
                }        
            }    
        }
        line_no += 1;      // tang chi so dong len 1        
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
    FILE *f_stopw = fopen("stopw.txt", "r"); 
    push(f_stopw, 1);
    fclose(f_stopw);
           
    FILE *f_src = fopen("vanban.txt", "r");
    init();
    push(f_src, 2);
    
    // for(int i = 0; i < mnf_range; i++) {
    //     printf("%s %d", mnf_list[i].word, mnf_list[i].num);
    //     for(int j = 0; j < get_num_line(i); j++)
    //         printf(",%s", mnf_list[i].lnum[j]);
    //     printf("\n");
    // }
    
    fclose(f_src);
}
