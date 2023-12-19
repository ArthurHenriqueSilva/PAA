#include <stdio.h>

#define CODE_SIZE 12
#define CNPJ_SIZE 18
#define MAX_CONTAINERS 100

typedef struct {
    char code[CODE_SIZE];
    char cnpj[CNPJ_SIZE];
    int weight;
} Container;

typedef struct {
    char code[CODE_SIZE];
    int diff;
    int percent;
}Weight_error;

int my_round(float x) {
    return (int)(x + 0.5);
}

char *copy_string(char *dest, char *src) {
    char *original_dest = dest;
    while ((*dest++ = *src++) != '\0');
    return original_dest;
}

int compare_strings(const char *s1, const char *s2) {
    while (*s1 != '\0' && *s2 != '\0' && *s1 == *s2) {
        s1++;
        s2++;
    }
    return (*s1 - *s2);
}

int read_number(FILE *file) {
    int x;
    fscanf(file, "%d", &x);
    return x;
}

void fillContainers(FILE *file, Container *cont, int n) {
    for (int i = 0; i < n; i++) {
        fscanf(file, "%s %s %d", cont[i].code, cont[i].cnpj, &cont[i].weight);
    }
}

void merge(Container *arr, int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Container L[n1], R[n2];

    for (i = 0; i < n1; i++) {
        for (int c = 0; c < CODE_SIZE; c++) {
            L[i].code[c] = arr[left + i].code[c];
        }
        for (int c = 0; c < CNPJ_SIZE; c++) {
            L[i].cnpj[c] = arr[left + i].cnpj[c];
        }
        L[i].weight = arr[left + i].weight;
    }
    for (j = 0; j < n2; j++) {
        for (int c = 0; c < CODE_SIZE; c++) {
            R[j].code[c] = arr[mid + 1 + j].code[c];
        }
        for (int c = 0; c < CNPJ_SIZE; c++) {
            R[j].cnpj[c] = arr[mid + 1 + j].cnpj[c];
        }
        R[j].weight = arr[mid + 1 + j].weight;
    }

    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2) {
        int compareResult = 0;
        for (int c = 0; c < CODE_SIZE; c++) {
            if (L[i].code[c] < R[j].code[c]) {
                compareResult = -1;
                break;
            } else if (L[i].code[c] > R[j].code[c]) {
                compareResult = 1;
                break;
            }
        }

        if (compareResult <= 0) {
            for (int c = 0; c < CODE_SIZE; c++) {
                arr[k].code[c] = L[i].code[c];
            }
            for (int c = 0; c < CNPJ_SIZE; c++) {
                arr[k].cnpj[c] = L[i].cnpj[c];
            }
            arr[k].weight = L[i].weight;
            i++;
        } else {
            for (int c = 0; c < CODE_SIZE; c++) {
                arr[k].code[c] = R[j].code[c];
            }
            for (int c = 0; c < CNPJ_SIZE; c++) {
                arr[k].cnpj[c] = R[j].cnpj[c];
            }
            arr[k].weight = R[j].weight;
            j++;
        }
        k++;
    }

    while (i < n1) {
        for (int c = 0; c < CODE_SIZE; c++) {
            arr[k].code[c] = L[i].code[c];
        }
        for (int c = 0; c < CNPJ_SIZE; c++) {
            arr[k].cnpj[c] = L[i].cnpj[c];
        }
        arr[k].weight = L[i].weight;
        i++;
        k++;
    }

    while (j < n2) {
        for (int c = 0; c < CODE_SIZE; c++) {
            arr[k].code[c] = R[j].code[c];
        }
        for (int c = 0; c < CNPJ_SIZE; c++) {
            arr[k].cnpj[c] = R[j].cnpj[c];
        }
        arr[k].weight = R[j].weight;
        j++;
        k++;
    }
}

void mergeSort(Container *arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void merge2(Weight_error *arr, int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Weight_error L[n1], R[n2];

    
    for (i = 0; i < n1; i++) {
        L[i] = arr[left + i];
    }
    for (j = 0; j < n2; j++) {
        R[j] = arr[mid + 1 + j];
    }

    
    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2) {
        if (L[i].percent <= R[j].percent) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort2(Weight_error *arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort2(arr, left, mid);
        mergeSort2(arr, mid + 1, right);
        merge2(arr, left, mid, right);
    }
}



int binarySearch(Container *c, int low, int high, const char *code) {
    while (low <= high) {
        int mid = low + (high - low) / 2;
        int comparing = compare_strings(c[mid].code, code);
        if (comparing == 0)
            return mid;
        else if (comparing < 0)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return -1;
}


int define_error(Container c1, Container c2, int *percent, int *diff){
    if(compare_strings(c1.cnpj, c2.cnpj) != 0) return 0;
    *diff = c1.weight - c2.weight;
    *diff = *diff < 0 ? (-*diff):*diff;
    *percent = my_round(((float)*diff / (float)c1.weight) * 100);
    if (*percent > 10) return 1;
    return 2;
}

void search_container(Container *c1, int n1, Container *c2, int n2, FILE *output, Weight_error *q) {
    int percent, diff;
    int w_index = 0;
    for (int i = 0; i < n1; i++) {
        char *code = c1[i].code;
        int index = binarySearch(c2, 0, n2 - 1, code);
        if (index != -1){
            int error_code = define_error(c1[i], c2[index], &percent, &diff);
            if(error_code == 0) fprintf(output, "%s: %s<->%s\n", c1[i].code, c1[i].cnpj, c2[index].cnpj);
            if(error_code == 1) {
                copy_string(q[w_index].code, code);
                q[w_index].percent = percent;
                q[w_index].diff = diff;
                w_index++;
                // fprintf(output, "%s: %dkg (%d%%)\n", c1[i].code, diff, percent);
            }
        }

    }
}

int main(int argc, char *argv[]) {
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");

    int size_1 = read_number(input);
    Container container_1[size_1];
    fillContainers(input, container_1, size_1);

    int size_2 = read_number(input);
    Container container_2[size_2];
    fillContainers(input, container_2, size_2);

    mergeSort(container_2, 0, size_2 - 1);

    Weight_error w_q[size_2];
    for (int i = 0; i < size_2; i++) {
    w_q[i].percent = 0;
}

    search_container(container_1, size_1, container_2, size_2, output, w_q);
    mergeSort2(w_q, 0, size_2-1);
    for (int i = 0; i < size_2; i++) {
        if (w_q[i].percent != 0) {
            fprintf(output, "%s: %dkg (%d%%)\n", w_q[i].code, w_q[i].diff, w_q[i].percent);
        }
    }
    fclose(input);
    fclose(output);

    return 0;
}
