### README.md

This is an implementation of the RC6 encryption algorithm in C. RC6 is a symmetric key block cipher designed by Rivest et al. The implementation provides functionalities for both encryption and decryption of plaintext and ciphertext.
#### Note ####
My code runs for all test cases provided in the research paper.

#### Usage

1. **Compilation**: If you prefer not to use the provided Makefile, you can manually compile the RC6 implementation using the following command:
    ```
    gcc -Wall -Wextra -std=c99 rc6.c -o rc6
    ```

    This command compiles the `rc6.c` source code into an executable named `rc6`.

2. **Encryption**: To encrypt a plaintext, run the compiled program `rc6` with the input file containing the plaintext and the output file to store the resulting ciphertext. Example:
    ```
    ./rc6 input.txt output.txt
    ```

    The input file should contain the plaintext in the following format:
    ```
    Encryption plaintext:
    A_value B_value C_value D_value
    userkey: key_values
    ```

    - `A_value`, `B_value`, `C_value`, `D_value` are 32-bit hexadecimal values representing the plaintext block.
    - `key_values` are hexadecimal values representing the encryption key.

3. **Decryption**: To decrypt a ciphertext, run the compiled program `rc6` with the input file containing the ciphertext and the output file to store the resulting plaintext. Example:
    ```
    ./rc6 input.txt output.txt
    ```

    The input file should contain the ciphertext in the following format:
    ```
    Decryption ciphertext:
    A_value B_value C_value D_value
    userkey: key_values
    ```

    - `A_value`, `B_value`, `C_value`, `D_value` are 32-bit hexadecimal values representing the ciphertext block.
    - `key_values` are hexadecimal values representing the decryption key.

#### Notes

- Ensure that the input file containing the plaintext/ciphertext is properly formatted as described above.
- The key size should not exceed 256 bits. If the input key exceeds 256 bits, the program ignores the excess.
- The program outputs the resulting ciphertext/plaintext to the specified output file.
- The success message will indicate the name of the output file where the result is stored.

### Compiling Without Makefile

If you choose not to use the provided Makefile, you can compile the code manually using the `gcc` compiler. Here's the command:

```
gcc -Wall -Wextra -std=c99 rc6.c -o rc6
```

After compilation, you can follow the same steps mentioned above for encryption and decryption.