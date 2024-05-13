#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define WORD_SIZE 32
#define NUM_ROUNDS 20
#define LEFT_ROTATE_AMOUNT 5
#define S_ARRAY_SIZE NUM_ROUNDS*2+4
#define P_CONSTANT 0xB7E15163
#define Q_CONSTANT 0x9E3779B9

// Function to find maximum of two numbers
int find_max(int num1, int num2) {
    return ((num1 > num2) ? num1 : num2);
}

// Function to perform left rotation
unsigned int left_rotate(unsigned int x, unsigned int bits) {
    return ((x << bits) | (x >> (32 - bits)));
}

// Function to perform right rotation
unsigned int right_rotate(unsigned int x, unsigned int bits) {
    return ((x >> bits) | (x << (32 - bits)));
}

int main(int argc, char** argv) {
    FILE *input_file;
    FILE *output_file;
    int usage;  // Indicates the usage of program
    char input_filename[30];
    char output_filename[30];

    int i;
    unsigned int input_value;

    int round_count;
    char input_type[20];
    unsigned int t_value;
    unsigned int u_value;
    unsigned int temp_value;
    unsigned int A_value, B_value, C_value, D_value;
    unsigned int S_array[S_ARRAY_SIZE];

    int key_size = 0;  // Size of the key
    int L_size = 0;    // Size of L
    int key_bit = 0;   // Maximum key size is 256
    int CC_value;      // Value for CC
    int v_value;

    unsigned int L_array[9];  // Array for L, maximum used index is 8
    unsigned int key_A_value;
    unsigned int key_B_value;
    unsigned int key_i_value;
    unsigned int key_j_value;
    char keyword[30];

    // Check for correct number of arguments
    if (argc != 3) {
        printf("\nIncorrect number of arguments. Usage: ./program_name input_file output_file\n");
        return 0;
    }

    // Open input file
    if ((input_file = fopen(argv[1], "r")) == NULL) {
        printf("\nFailed to open input file.\n");
        return 0;
    }

    // Read keyword
    fscanf(input_file, "%s", keyword);

    if (strcmp(keyword, "Encryption") == 0) {
        usage = 0;
    } else if (strcmp(keyword, "Decryption") == 0) {
        usage = 1;
    }

    // Read input type
    fscanf(input_file, "%s", input_type);

    if (usage == 0) {
        if (strcmp(input_type, "plaintext:") != 0) {
            printf("\nError: Expected \"plaintext:\" in input file.\n");
            fclose(input_file);
            return 0;
        }
    } else {
        if (strcmp(input_type, "ciphertext:") != 0) {
            printf("\nError: Expected \"ciphertext:\" in input file.\n");
            fclose(input_file);
            return 0;
        }
    }

    // Initialize A, B, C, D values
    A_value = 0x00000000;
    B_value = 0x00000000;
    C_value = 0x00000000;
    D_value = 0x00000000;

    // Read A, B, C, D values from input file
    for (i = 0; i < (WORD_SIZE / 8); i++) {
        fscanf(input_file, "%x", &input_value);
        A_value = A_value | (input_value << (i * 8));
    }
    for (i = 0; i < (WORD_SIZE / 8); i++) {
        fscanf(input_file, "%x", &input_value);
        B_value = B_value | (input_value << (i * 8));
    }
    for (i = 0; i < (WORD_SIZE / 8); i++) {
        fscanf(input_file, "%x", &input_value);
        C_value = C_value | (input_value << (i * 8));
    }
    for (i = 0; i < (WORD_SIZE / 8); i++) {
        fscanf(input_file, "%x", &input_value);
        D_value = D_value | (input_value << (i * 8));
    }

    // Read userkey
    fscanf(input_file, "%s", input_type);

    if (strcmp(input_type, "userkey:") != 0) {
        printf("Error: \"userkey:\" not found in input file.\n");
        fclose(input_file);
        return 0;
    }

    // Initialize L array
    for (i = 0; i < 9; i++) {
        L_array[i] = 0x00000000;
    }

    // Read key values
    while ((fscanf(input_file, "%x", &input_value) == 1) && (key_bit <= 256)) {
        L_size = key_size / 4;
        L_array[L_size] = (L_array[L_size] | (input_value << (key_size % (WORD_SIZE / 8) * 8)));
        key_size++;
        key_bit += 8;
    }

    CC_value = L_size + 1;
    if (key_bit > 256)
        printf("\n\nNotice: Input key exceeds 256 bits, but the program ignores the rest.\n");
    else
        printf("\nKey size is %d\n\n", CC_value * 32);

    fclose(input_file);

    // Key schedule
    S_array[0] = P_CONSTANT;

    for (i = 1; i < S_ARRAY_SIZE; i++) {
        S_array[i] = S_array[i - 1] + Q_CONSTANT;
    }

    key_A_value = key_B_value = key_i_value = key_j_value = 0;

    v_value = 3 * find_max(CC_value, S_ARRAY_SIZE);
    for (i = 1; i <= v_value; i++) {
        key_A_value = S_array[key_i_value] = left_rotate((S_array[key_i_value] + key_A_value + key_B_value), 3);
        key_B_value = L_array[key_j_value] = left_rotate((L_array[key_j_value] + key_A_value + key_B_value), (key_A_value + key_B_value));
        key_i_value = (key_i_value + 1) % (S_ARRAY_SIZE);
        key_j_value = (key_j_value + 1) % (CC_value);
    }

    // Encryption or Decryption
    if (usage == 0) {
        B_value = B_value + S_array[0];
        D_value = D_value + S_array[1];

        for (round_count = 1; round_count <= NUM_ROUNDS; round_count++) {
            t_value = left_rotate((B_value * (2 * B_value + 1)), LEFT_ROTATE_AMOUNT);
            u_value = left_rotate((D_value * (2 * D_value + 1)), LEFT_ROTATE_AMOUNT);
            A_value = left_rotate((A_value ^ t_value), (u_value & 0x1f)) + S_array[2 * round_count];
            C_value = left_rotate((C_value ^ u_value), (t_value & 0x1f)) + S_array[2 * round_count + 1];
            temp_value = A_value;
            A_value = B_value;
            B_value = C_value;
            C_value = D_value;
            D_value = temp_value;
        }

        A_value = A_value + S_array[2 * NUM_ROUNDS + 2];
        C_value = C_value + S_array[2 * NUM_ROUNDS + 3];

    } else {
        C_value = C_value - S_array[2 * NUM_ROUNDS + 3];
        A_value = A_value - S_array[2 * NUM_ROUNDS + 2];

        for (round_count = NUM_ROUNDS; round_count >= 1; round_count--) {
            temp_value = D_value;
            D_value = C_value;
            C_value = B_value;
            B_value = A_value;
            A_value = temp_value;

            u_value = left_rotate((D_value * (2 * D_value + 1)), LEFT_ROTATE_AMOUNT);
            t_value = left_rotate((B_value * (2 * B_value + 1)), LEFT_ROTATE_AMOUNT);
            C_value = (right_rotate((C_value - S_array[2 * round_count + 1]), (t_value & 0x1f)) ^ u_value);
            A_value = (right_rotate((A_value - S_array[2 * round_count]), (u_value & 0x1f)) ^ t_value);
        }
        D_value = D_value - S_array[1];
        B_value = B_value - S_array[0];
    }

    // Output
    if ((output_file = fopen(argv[2], "w")) == NULL) {
        printf("\nFailed to open output file.\n");
        exit(-1);
    }

    if (usage == 0) {
        fprintf(output_file, "ciphertext: ");
        printf(" ciphertext:\n\n");

    } else {
        fprintf(output_file, "plaintext: ");
        printf("plaintext:\n\n");

    }

    for (i = 0; i < (WORD_SIZE / 8); i++) {
        fprintf(output_file, "%.2x ", (A_value & (0xff << (i * 8))) >> (i * 8));
        printf("%.2x ", (A_value & (0xff << (i * 8))) >> (i * 8));
    }

    for (i = 0; i < (WORD_SIZE / 8); i++) {
        fprintf(output_file, "%.2x ", (B_value & (0xff << (i * 8))) >> (i * 8));
        printf("%.2x ", (B_value & (0xff << (i * 8))) >> (i * 8));

    }
    for (i = 0; i < (WORD_SIZE / 8); i++) {
        fprintf(output_file, "%.2x ", (C_value & (0xff << (i * 8))) >> (i * 8));
        printf("%.2x ", (C_value & (0xff << (i * 8))) >> (i * 8));

    }
    for (i = 0; i < (WORD_SIZE / 8); i++) {
        fprintf(output_file, "%.2x ", (D_value & (0xff << (i * 8))) >> (i * 8));
        printf("%.2x ", (D_value & (0xff << (i * 8))) >> (i * 8));

    }

    fclose(output_file);

    printf("\n\n output is also stored in %s\n\n", argv[2]);

    return 0;
}
