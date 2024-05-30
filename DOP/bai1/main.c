#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100

void insertionSortAsm(int* arr, int n) {
    asm (
            "mov $1, %%ecx\n\t"          // Initialize outer loop index j to 1
            "outer_loop:\n\t"
            "cmp %1, %%ecx\n\t"          // Compare j with n
            "jge outer_loop_end\n\t"     // If j >= n, end outer loop

            // key = arr[j]
            "mov (%0, %%rcx, 4), %%eax\n\t"

            // i = j - 1
            "mov %%ecx, %%ebx\n\t"
            "dec %%ebx\n\t"

            // inner loop
            "inner_loop:\n\t"
            "cmp $0, %%ebx\n\t"          // Compare i with 0
            "jl inner_loop_end\n\t"      // If i < 0, end inner loop

            // Compare arr[i] with key
            "mov (%0, %%rbx, 4), %%edx\n\t"
            "cmp %%eax, %%edx\n\t"
            "jle inner_loop_end\n\t"     // If arr[i] <= key, end inner loop

            // arr[i + 1] = arr[i]
            "mov %%edx, 4(%0, %%rbx, 4)\n\t"

            // i--
            "dec %%ebx\n\t"
            "jmp inner_loop\n\t"         // Repeat inner loop

            "inner_loop_end:\n\t"
            // arr[i + 1] = key
            "mov %%eax, 4(%0, %%rbx, 4)\n\t"

            // j++
            "inc %%ecx\n\t"
            "jmp outer_loop\n\t"         // Repeat outer loop

            "outer_loop_end:\n\t"
            : // No output operands
            : "r" (arr), "r" (n)         // Input operands
            : "%eax", "%ebx", "%ecx", "%edx" // Clobbered registers
            );
}


int main() {
    int* arr = (int*)malloc(MAX_SIZE * sizeof(int));
    if (arr == NULL) {
        printf("ERROR: Could not allocate memory\n");
        return 1;
    }
    int n = 0;

    FILE* input_file = fopen("INPUT.txt", "r");
    if (input_file == NULL) {
        printf("ERROR: Could not open input.txt\n");
        free(arr);
        return 1;
    }

    // Đọc giá trị N
    if (fscanf(input_file, "%d", &n) != 1 || n < 1 || n > MAX_SIZE) {
        printf("ERROR: Invalid value for N\n");
        fclose(input_file);
        free(arr);
        return 1;
    }

    // Đảm bảo không đọc quá số lượng phần tử cho phép
    if (n > MAX_SIZE) {
        printf("ERROR: Number of elements exceeds maximum limit\n");
        fclose(input_file);
        free(arr);
        return 1;
    }

    // Đọc và kiểm tra mỗi số
    for (int i = 0; i < n; i++) {
        if (fscanf(input_file, "%d", &arr[i]) != 1 || abs(arr[i]) > 1000000000) {
            printf("ERROR: Invalid value for element at position %d\n", i + 1);
            fclose(input_file);
            free(arr);
            return 1;
        }
    }

    fclose(input_file);

    // Gọi hàm sắp xếp bằng inline assembly
    insertionSortAsm(arr, n);

    FILE* output_file = fopen("OUTPUT.txt", "w");
    if (output_file == NULL) {
        printf("ERROR: Could not open output.txt\n");
        free(arr);
        return 1;
    }
    for (int i = 0; i < n; i++) {
        fprintf(output_file, "%d\n", arr[i]);
    }

    fclose(output_file);
    free(arr);

    return 0;
}
