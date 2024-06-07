#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 1e9

void sort(int *input, int n) {
    int flag = 1;

    asm(
            // Bắt đầu vòng lặp ngoài với việc thiết lập cờ flag thành 1

            "movl $0x1, %0\n\t"             // Đặt giá trị 1 vào biến flag
            "outer_cycle_begin:\n\t"        // Đặt nhãn bắt đầu vòng lặp ngoài
            "cmpl $0x0, %0\n\t"             // So sánh flag với 0
            "je outer_cycle_end\n\t"        // Nếu flag là 0, nhảy đến nhãn kết thúc vòng lặp ngoài

            // Bắt đầu vòng lặp trong với việc thiết lập cờ flag thành 0

            "movl $0x0, %0\n\t"             // Đặt flag về 0 để bắt đầu vòng lặp trong
            "movq $0, %%rsi\n\t"            // Đặt bộ đếm vòng lặp trong (rsi) về 0
            "outer_loop:\n\t"               // Đặt nhãn bắt đầu vòng lặp trong
            "incq %%rsi\n\t"            // Tăng bộ đếm rsi lên 1
            "cmpq %%rax, %%rsi\n\t"     // So sánh rsi với n
            "jge outer_loop_end\n\t"    // Nếu rsi >= n, nhảy đến nhãn kết thúc vòng lặp trong
            "movl (%%rbx, %%rsi, 0x4), %%ecx\n\t"  // Lấy phần tử hiện tại từ mảng vào ecx
            "movl -0x4(%%rbx, %%rsi, 0x4), %%edx\n\t" // Lấy phần tử trước đó vào edx
            "cmpl %%edx, %%ecx\n\t"     // So sánh phần tử hiện tại với phần tử trước đó
            "jge dont_swap\n\t"         // Nếu phần tử hiện tại >= phần tử trước đó, nhảy đến nhãn dont_swap (không hoán đổi)

            // Hoán đổi các phần tử sử dụng phép toán XOR

            "xorl %%edx, %%ecx\n\t"     // ecx = ecx ^ edx
            "xorl %%ecx, %%edx\n\t"     // edx = ecx ^ edx
            "xorl %%edx, %%ecx\n\t"     // ecx = ecx ^ edx
            "movl %%ecx, (%%rbx, %%rsi, 0x4)\n\t"  // Lưu phần tử hiện tại vào vị trí của phần tử trước đó
            "movl %%edx, -0x4(%%rbx, %%rsi, 0x4)\n\t" // Lưu phần tử trước đó vào vị trí của phần tử hiện tại
            "movl $0x1, %0\n\t"         // Đặt flag thành 1 (có hoán đổi)

            // Tiếp tục vòng lặp

            "dont_swap:\n\t"            // Nhãn không hoán đổi
            "jmp outer_loop\n\t"        // Quay lại vòng lặp trong
            "outer_loop_end:\n\t"           // Nhãn kết thúc vòng lặp trong
            "decq %%rax\n\t"            // Giảm bộ đếm n
            "testl %0, %0\n\t"          // Kiểm tra flag sau khi kết thúc vòng lặp trong
            "jnz outer_cycle_begin\n\t" // Nếu flag khác 0, quay lại vòng lặp ngoài
            "outer_cycle_end:\n\t"          // Nhãn kết thúc vòng lặp ngoài


            : "+r"(flag) // Biến flag được cập nhật trong assembly
            : "b"(input), "a"(n) // input và n được truyền vào assembly
            : "memory", "cc", "rsi", "ecx", "edx" // Các thanh ghi và bộ nhớ được sử dụng và thay đổi trong assembly
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