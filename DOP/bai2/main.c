#include <stdio.h>
#include <stdlib.h>
#define OFFSET 1000000000  // This offset is used to handle negative indices


void find_duplicates(int* arr, int* result, int n, int max_val);

int main() {
    int n;
    FILE* input_file = fopen("INPUT.txt", "r");
    if (input_file == NULL) {
        printf("ERROR: Could not open INPUT.txt\n");
        return 1;
    }

    // Read the number of elements
    fscanf(input_file, "%d", &n);
    if (n < 0) {
        printf("ERROR: Invalid number of elements\n");
        fclose(input_file);
        return 1;
    }

    int* arr = (int*)malloc(n * sizeof(int));
    if (arr == NULL) {
        printf("ERROR: Memory allocation failed\n");
        fclose(input_file);
        return 1;
    }

    // Read the elements into arr
    for (int i = 0; i < n; i++) {
        fscanf(input_file, "%d", &arr[i]);
    }

    fclose(input_file);

    // Determine the range of the array
    int max_val = 2 * OFFSET;  // Because of the range from -10^9 to 10^9
    int* result = (int*)calloc(max_val, sizeof(int));
    if (result == NULL) {
        printf("ERROR: Memory allocation failed\n");
        free(arr);
        return 1;
    }

    FILE* output_file = fopen("OUTPUT.txt", "w");
    if (output_file == NULL) {
        printf("ERROR: Could not open OUTPUT.txt\n");
        free(arr);
        free(result);
        return 1;
    }

    find_duplicates(arr, result, n, max_val);  // Call ASM function

    int flag = 0;
    for (int i = 0; i < n; i++) {
        int adjusted_index = arr[i] + OFFSET;
        if (result[adjusted_index] > 1) {
            flag = 1;
            fprintf(output_file, "%d - %d\n", arr[i], result[adjusted_index]);
            result[adjusted_index] = 0; // Reset to avoid duplicate output
        }
    }

    if (flag == 0) {
        fprintf(output_file, "None\n");
    }

    fclose(output_file);
    free(arr);
    free(result);

    return 0;
}


void find_duplicates(int* arr, int* result, int n, int max_val) {
    asm (
            "movl %0, %%ecx\n\t"              // Move n to ecx
            "movq %1, %%rsi\n\t"              // Move arr to rsi
            "movq %2, %%rdi\n\t"              // Move result to rdi
            "movl %3, %%r8d\n\t"              // Move max_val to r8d
            "xorq %%rdx, %%rdx\n\t"           // Clear rdx
            "outer_loop:\n\t"
            "cmpq %%rcx, %%rdx\n\t"           // Compare rdx with rcx
            "jge outer_loop_end\n\t"          // Jump to end if rdx >= rcx
            "movl (%%rsi, %%rdx, 4), %%eax\n\t" // Load arr[rdx] into eax
            "addl $1000000000, %%eax\n\t"     // Add OFFSET to eax
            "cmpl $0, %%eax\n\t"              // Check if arr[rdx] >= 0
            "jl next_iteration\n\t"           // Skip if arr[rdx] < 0
            "cmpl %%r8d, %%eax\n\t"           // Check if arr[rdx] < max_val
            "jge next_iteration\n\t"          // Skip if arr[rdx] >= max_val
            "movl (%%rdi, %%rax, 4), %%ebx\n\t" // Load result[arr[rdx]] into ebx
            "incl %%ebx\n\t"                  // Increment ebx
            "movl %%ebx, (%%rdi, %%rax, 4)\n\t" // Store ebx back to result[arr[rdx]]
            "next_iteration:\n\t"
            "incq %%rdx\n\t"                  // Increment rdx
            "jmp outer_loop\n\t"              // Jump back to outer_loop
            "outer_loop_end:\n\t"
            :
            : "g" (n), "g" (arr), "g" (result), "g" (max_val)
            : "%eax", "%ebx", "%ecx", "%rdx", "%rsi", "%rdi", "%r8"
            );
}