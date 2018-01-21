#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct data {
    char word[20];
    int num;    // so lan xuat hien
    char lnum[100][3];  // cac dong xuat hien, toi da co 100 dong
}token;

token stop_token[30]; // toi da co 30 tu cam
token proNoun[30];
int proNoun_range = 0;
int stop_range = 0;

token hash_token[23][20]; // toi da 20 tu co nghia ung voi cac chu cai dau tu a->z
int num[23] = {0};  // so luong tu ung voi cac chu cai dau tu a->z

void init() {
    for(int i = 0; i < 23; i++) {   
        for (int j = 0; j < 20; j++)
        {
            hash_token[i][j].num = 0;
        }
    }
}

void to_lower(char buf[]) {
    int i = 0;
     
    while(buf[i] != '\0') { 
        if(buf[i] >= 'A' && buf[i] <= 'Z') buf[i] += 32;
        i++;
    }    
}

int check_stop_token(char buf[]) {   // ton tai thi tra lai 1, 0 trong th nguoc lai
    int i = 0;

    while(i < stop_range) {
        if(strcmp(stop_token[i++].word, buf) == 0) return 1;
    }
    return 0;
}

int check_proNoun(char buf[]) {
    int i = 0;

    while(i < proNoun_range) {
        if(strcmp(proNoun[i++].word, buf) == 0) return 1;
    }
    return 0;
}

int get_num_line(int p, int k) {
    for(int i = 0; i < 100; i++) 
        if(hash_token[p][k].lnum[i][0] == '\0') return i;
}

void push_hash_list(char buf[], int line_no) {    //  tra ve vi tri tu duoc them vao hoac update trong mang cac ki tu dau 
    int i = 0;
    int k = buf[0] - 97;
    int range = num[k];
    int w, n;

    char s[3];
    sprintf(s, "%d", line_no);  // chuyen so dong sang string
    
    while(i < num[k]) {
        if(strcmp(hash_token[k][i].word, buf) == 0) {    // neu da ton tai trong day
            hash_token[k][i].num += 1;
            break;
        }
        i++;
    }

    if(i == num[k]) {
        strcpy(hash_token[k][i].word, buf);  // neu chua ton tai
        hash_token[k][i].num = 1;
        num[k] += 1;
    }

    n = get_num_line(k, i); // lay ra so luong dong
    for(w = 0; w < n; w++)
        if(strcmp(hash_token[k][i].lnum[w], s) == 0) 
            break;
    if(w == n) strcpy(hash_token[k][i].lnum[w], s);

    return;
}

void push(int op) {
    char line[255];
    char buffer[20];
    int index = 0;
    int j = 0;
    int line_no = 1;
    int choice; 

    if(op == 1) {
        FILE *f = fopen("stopw.txt", "r");
        while(fgets(line, 255, f)) {
            for(int i = 0; i < strlen(line); i++) {
                if(line[i] >= 'a' && line[i] <= 'z') { 
                    buffer[j++] = line[i];
                }
                else {
                    buffer[j] = '\0'; 
                    if(*buffer != '\0') {
                        strcpy(stop_token[index++].word, buffer);
                        stop_range += 1;
                    }
                    j = 0;
                }            
            }        
        }
        fclose(f);
    }

    else if(op == 2) {
        FILE *f = fopen("vanban.txt", "r");
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
                        while((line[i] >= 'a' && line[i] <= 'z') || (line[i] >= 'A' && line[i] <= 'Z'))
                            buffer[j++] = line[i++];
                        buffer[j] = '\0';
                        j = 0;
                        //if(strcmp(buffer, "At")) printf("haha\n");
                        to_lower(buffer);
                        if(!check_stop_token(buffer)) {
                            if(!check_proNoun(buffer)) {
                                printf("%s co phai danh tu rieng khong (1 la dung, 0 la sai):", buffer);
                                scanf("%d", &choice);
                                switch (choice) {
                                    case 1:
                                        strcpy(proNoun[proNoun_range++].word, buffer);
                                        break;
                                    case 0:
                                        push_hash_list(buffer, line_no);
                                        break;
                                    default:
                                        break;    
                                }
                            }
                            
                        }
                        //buffer[j++] = line[i];    
                    }
                }
                else {
                    buffer[j] = '\0'; 
                    if(*buffer != '\0') {
                        to_lower(buffer);
                        if(!check_stop_token(buffer)) {  // neu khong ton tai trong stop_token
                            push_hash_list(buffer, line_no);
                        }
                    }
                j = 0;
                }           
            }
        line_no += 1;      // tang chi so dong len 1        
        }
        fclose(f);
    }   
}

void swap(token *t1, token *t2) {
    token *tmp = (token *)malloc(sizeof(token));
    *tmp = *t1;
    *t1 = *t2;
    *t2 = *tmp;
    free(tmp);
}

void sort(token *t, int k) {
    int min;

    for(int i = 0; i < num[k] - 1; i++) {
        min = i;
        for(int j = i + 1; j < num[k]; j++) 
            if(strcmp(t[min].word, t[j].word) > 0) 
                min = j;
        swap(t + min, t + i);
    }
}

void print() {
    for(int i = 0; i < 23; i++) {
        if(num[i] != 0) {
            sort(hash_token[i], i);    
            for(int j = 0; j < num[i]; j++) {
                printf("%s %d", hash_token[i][j].word, hash_token[i][j].num);
                for(int k = 0; k < get_num_line(i, j);)
                    printf(",%s", hash_token[i][j].lnum[k++]);
                printf("\n"); 
            }
        }
    }
}

void main() {
    push(1);    // xay dung stop list
    init();
    push(2);    // xay dung useful list
    print();    // print ket qua
    exit(0);
}
