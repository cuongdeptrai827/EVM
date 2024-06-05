#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 1e9

void sort(int *input, int n) {
    int flag = 1;

    asm(
        // Устанавливаем флаг в 1 для начала внешнего цикла
            "movl $0x1, %0\n\t"
            "outer_cycle_begin:\n\t" // Метка начала внешнего цикла
            "cmpl $0x0, %0\n\t" // Сравниваем флаг с 0
            "je outer_cycle_end\n\t" // Если флаг равен 0, завершаем цикл
            "movl $0x0, %0\n\t" // Сбрасываем флаг для начала внутреннего цикла
            "movq $0, %%rsi\n\t" // Устанавливаем счетчик для внутреннего цикла
            "outer_loop:\n\t" // Метка начала внутреннего цикла
            "    incq %%rsi\n\t" // Увеличиваем счетчик
            "    cmpq %%rax, %%rsi\n\t" // Сравниваем счетчик с n
            "    jge outer_loop_end\n\t" // Если счетчик >= n, завершаем цикл
            "    movl (%%rbx, %%rsi, 0x4), %%ecx\n\t" // Загружаем текущий элемент массива в ecx
            "    movl -0x4(%%rbx, %%rsi, 0x4), %%edx\n\t" // Загружаем предыдущий элемент массива в edx
            "    cmpl %%edx, %%ecx\n\t" // Сравниваем текущий и предыдущий элементы
            "    jge dont_swap\n\t" // Если текущий >= предыдущего, пропускаем перестановку
            "    xorl %%edx, %%ecx\n\t" // Меняем местами элементы с помощью XOR
            "    xorl %%ecx, %%edx\n\t"
            "    xorl %%edx, %%ecx\n\t"
            "    movl %%ecx, (%%rbx, %%rsi, 0x4)\n\t" // Сохраняем текущий элемент на место предыдущего
            "    movl %%edx, -0x4(%%rbx, %%rsi, 0x4)\n\t" // Сохраняем предыдущий элемент на место текущего
            "    movl $0x1, %0\n\t" // Устанавливаем флаг в 1 (была произведена перестановка)
            "    dont_swap:\n\t"
            "    jmp outer_loop\n\t" // Переходим к следующей итерации внутреннего цикла
            "outer_loop_end:\n\t" // Метка конца внутреннего цикла
            "    decq %%rax\n\t" // Уменьшаем счетчик n
            "    testl %0, %0\n\t" // Проверяем флаг после завершения внутреннего цикла
            "    jnz outer_cycle_begin\n\t" // Если флаг установлен, продолжаем внешний цикл
            "outer_cycle_end:\n\t" // Метка конца внешнего цикла
            : "+r"(flag) // Входной операнд - флаг (изменяемый)
            : "b"(input), "a"(n) // Входные операнды - массив input и n
            : "memory", "cc", "rsi", "ecx", "edx" // Список регистров, которые используются и изменяются в ассемблерной вставке
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
    sort(arr, n);

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