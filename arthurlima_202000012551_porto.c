#include <stdio.h>

#define CODE_SIZE 12
#define CNPJ_SIZE 18
#define MAX_CONTAINERS 100

typedef struct {
    char code[CODE_SIZE];
    char cnpj[CNPJ_SIZE];
    int weight;
} Container;

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

void search_container(Container *c1, int n1, Container *c2, int n2) {
    for (int i = 0; i < n1; i++) {
        const char *code = c1[i].code;
        int index = binarySearch(c2, 0, n2 - 1, code);
        if (index != -1)
            printf("Found: %s == %s\n", code, c2[index].code);
        else
            printf("Not found: %s\n", code);
    }
}

void printContainers(Container *cont, int n) {
    for (int i = 0; i < n; i++) {
        printf("%s %s %d\n", cont[i].code, cont[i].cnpj, cont[i].weight);
    }
}

int main(int argc, char *argv[]) {
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");

    int size_1 = read_number(input);
    Container container_1[MAX_CONTAINERS];
    fillContainers(input, container_1, size_1);

    int size_2 = read_number(input);
    Container container_2[MAX_CONTAINERS];
    fillContainers(input, container_2, size_2);

    mergeSort(container_2, 0, size_2 - 1);

    printf("Sorted container_2:\n");
    printContainers(container_2, size_2);

    printf("\nSearch results:\n");
    search_print(container_1, size_1, container_2, size_2);

    fclose(input);
    fclose(output);

    return 0;
}
