#include <iostream>
#include <string>

using namespace std;

// Function to encrypt plaintext using Caesar cipher
string encrypt(string plaintext, int key) {
    string ciphertext = "";
    for (char& ch : plaintext) {
        if (isalpha(ch)) {  // Process only alphabetic characters
            char base = isupper(ch) ? 'A' : 'a';
            ch = static_cast<char>((ch - base + key) % 26 + base);  // Shift character
        }
        ciphertext += ch;
    }
    return ciphertext;
}

// Function to decrypt ciphertext using Caesar cipher
string decrypt(string ciphertext, int key) {
    return encrypt(ciphertext, 26 - key);  // Decrypt by shifting in opposite direction
}

int main() {
    string plaintext = "Wassis Deutsche?";
    int key = 9;

    // Encrypt plaintext
    string encrypted = encrypt(plaintext, key);
    cout << "Encrypted: " << encrypted << endl;

    // Decrypt ciphertext
    string decrypted = decrypt(encrypted, key);
    cout << "Decrypted: " << decrypted << endl;

    return 0;
}
