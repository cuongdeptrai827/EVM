#include <stdio.h>
#include <stdlib.h>

void findDuplicates(int* arr, int* result, int n) {
    asm (
            "movl %0, %%ecx\n\t"
            "movl %1, %%esi\n\t"
            "movl %2, %%edi\n\t"
            "xorl %%edx, %%edx\n\t"
            "outer_loop:\n\t"
            "cmpl %%edx, %%ecx\n\t"
            "jge outer_loop_end\n\t"
            "movl (%%esi, %%edx, 4), %%eax\n\t"
            "movl (%%edi, %%eax, 4), %%ebx\n\t"
            "incl %%ebx\n\t"
            "movl %%ebx, (%%edi, %%eax, 4)\n\t"
            "incl %%edx\n\t"
            "jmp outer_loop\n\t"
            "outer_loop_end:\n\t"
            :
            : "g" (n), "g" (arr), "g" (result)
            : "%eax", "%ebx", "%ecx", "%edx", "%esi", "%edi", "memory"
            );
}
int main() {
    FILE *inputFile = fopen("INPUT.TXT", "r");
    FILE *outputFile = fopen("OUTPUT.TXT", "w");

    if (inputFile == NULL || outputFile == NULL) {
        printf("Lỗi khi mở file.\n");
        return 1;
    }

    int N;
    fscanf(inputFile, "%d", &N);

    if (N < 1 || N > 100) {
        printf("N không nằm trong khoảng cho phép.\n");
        fclose(inputFile);
        fclose(outputFile);
        return 1;
    }

    int *numbers = (int *)malloc(N * sizeof(int));
    if (numbers == NULL) {
        printf("Lỗi khi cấp phát bộ nhớ.\n");
        fclose(inputFile);
        fclose(outputFile);
        return 1;
    }

    // Đọc các số từ file vào mảng numbers
    for (int i = 0; i < N; i++) {
        fscanf(inputFile, "%d", &numbers[i]);
        if (abs(numbers[i]) > 1000000000) {
            printf("Số không nằm trong khoảng cho phép.\n");
            free(numbers);
            fclose(inputFile);
            fclose(outputFile);
            return 1;
        }
    }

    // Tạo mảng kết quả để lưu số lần xuất hiện của mỗi số
    int *result = (int *)calloc(1000000001, sizeof(int));

    // Gọi hàm để tìm và lưu các số trùng lặp
    findDuplicates(numbers, result, N);

    // In ra file OUTPUT.TXT các số trùng lặp
    int foundDuplicates = 0;
    for (int i = 0; i < 1000000001; i++) {
        if (result[i] > 1) {
            fprintf(outputFile, "%d - %d\n", i, result[i]);
            foundDuplicates = 1;
        }
    }

    // Nếu không có số trùng lặp
    if (!foundDuplicates) {
        fprintf(outputFile, "None\n");
    }

    // Giải phóng bộ nhớ và đóng file
    free(numbers);
    free(result);
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
