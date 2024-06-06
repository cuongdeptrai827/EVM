#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 100

void calculate_repetitions(int* arr, int* result, int n) {
    asm (
            "movl %0, %%ecx\n\t"              // Move n to ecx
            "movl %1, %%esi\n\t"              // Move arr to esi
            "movl %2, %%edi\n\t"              // Move result to edi
            "xorl %%edx, %%edx\n\t"           // Clear edx
            "outer_loop:\n\t"
            "cmpl %%ecx, %%edx\n\t"           // Compare edx with ecx
            "jge outer_loop_end\n\t"          // Jump to end if edx >= ecx
            "movl (%%esi, %%edx, 4), %%eax\n\t" // Load arr[edx] into eax
            "cmpl $0, %%eax\n\t"              // Check if arr[edx] >= 0
            "jl next_iteration\n\t"           // Skip if arr[edx] < 0
            "cmpl %3, %%eax\n\t"              // Check if arr[edx] < MAX_SIZE
            "jge next_iteration\n\t"          // Skip if arr[edx] >= MAX_SIZE
            "movl (%%edi, %%eax, 4), %%ebx\n\t" // Load result[arr[edx]] into ebx
            "incl %%ebx\n\t"                  // Increment ebx
            "movl %%ebx, (%%edi, %%eax, 4)\n\t" // Store ebx back to result[arr[edx]]
            "next_iteration:\n\t"
            "incl %%edx\n\t"                  // Increment edx
            "jmp outer_loop\n\t"              // Jump back to outer_loop
            "outer_loop_end:\n\t"
            :
            : "g" (n), "g" (arr), "g" (result), "g" (MAX_SIZE)
            : "%eax", "%ebx", "%ecx", "%edx", "%esi", "%edi"
            );
}

int main() {
    int* arr = (int*)malloc(MAX_SIZE * sizeof(int));
    int* result = (int*)calloc(MAX_SIZE, sizeof(int));
    if (arr == NULL || result == NULL) {
        printf("ERROR: Memory allocation failed\n");
        return 1;
    }

    int n;
    FILE* input_file = fopen("INPUT.txt", "r");
    if (input_file == NULL) {
        printf("ERROR: Could not open INPUT.txt\n");
        free(arr);
        free(result);
        return 1;
    }

    // Read the number of elements
    fscanf(input_file, "%d", &n);
    if (n < 0 || n > MAX_SIZE) {
        printf("ERROR: Invalid number of elements\n");
        fclose(input_file);
        free(arr);
        free(result);
        return 1;
    }

    // Read the elements into arr
    for (int i = 0; i < n; i++) {
        fscanf(input_file, "%d", &arr[i]);
        if (arr[i] < 0 || arr[i] >= MAX_SIZE) {
            printf("ERROR: Element out of bounds\n");
            fclose(input_file);
            free(arr);
            free(result);
            return 1;
        }
    }

    fclose(input_file);

    FILE* output_file = fopen("OUTPUT.txt", "w");
    if (output_file == NULL) {
        printf("ERROR: Could not open OUTPUT.txt\n");
        free(arr);
        free(result);
        return 1;
    }

    calculate_repetitions(arr, result, n);  // Call ASM function

    int flag = 0;
    for (int i = 0; i < n; i++) {
        if (result[arr[i]] > 1) {
            flag = 1;
            fprintf(output_file, "%d - %d\n", arr[i], result[arr[i]]);
            result[arr[i]] = 0; // Reset to avoid duplicate output
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
