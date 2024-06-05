#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VALUE 1000  // Adjust this based on the range of values in your array

void find_duplicates(int *array, int size) {
    // Create and initialize the counts array to 0
    int counts[MAX_VALUE + 1] = {0};

    // Inline assembly to count occurrences of each element in the array
    __asm__ (
            "movl %1, %%ecx\n"           // Load the size of the array into ECX
            "movl %2, %%esi\n"           // Load the base address of the input array into ESI
            "leal %0, %%ebx\n"           // Load the address of the counts array into EBX
            "xorl %%edx, %%edx\n"        // Clear EDX (loop counter for the input array)
            "count_loop:\n"
            "cmpl %%edx, %%ecx\n"        // Compare EDX (loop counter) with ECX (array size)
            "jge count_end\n"            // If EDX >= ECX, end the loop
            "movl (%%esi,%%edx,4), %%eax\n" // Load the current array element into EAX
            "addl $0, (%%ebx, %%eax, 4)\n" // Dummy addl to adjust the address for correct use in incl
            "incl (%%ebx, %%eax, 4)\n"   // Increment the count of this element in the counts array
            "incl %%edx\n"               // Increment the loop counter
            "jmp count_loop\n"           // Jump back to the start of the loop
            "count_end:\n"
            :                             // No output operands
            : "m"(counts), "r"(size), "r"(array)
            : "eax", "ebx", "ecx", "edx", "esi"
            );

    // Loop through the counts array to find and print duplicates
    for (int i = 0; i <= MAX_VALUE; ++i) {
        if (counts[i] > 1) {
            printf("Element %d has %d duplications\n", i, counts[i]);
        }
    }
}

int main() {
    int array[] = {1, 2, 2, 3, 4, 4, 4, 5, 5, 5, 5};
    int size = sizeof(array) / sizeof(array[0]);

    find_duplicates(array, size);

    return 0;
}
