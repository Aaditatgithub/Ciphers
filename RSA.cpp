#include <iostream>
using namespace std;

// Function to compute gcd
int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// Function to find modular inverse
int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 0) {
            return x;
        }
    }
    return 1;
}

// Function to perform modular exponentiation
long long modExp(long long base, long long exp, long long mod) {
    long long result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp = exp / 2;
    }
    return result;
}

int main() {
    // Two prime numbers
    int p = 61;
    int q = 53;

    // Calculate n
    int n = p * q;

    // Calculate the totient of n
    int phi = (p - 1) * (q - 1);

    // Choose an integer e such that 1 < e < phi(n) and gcd(e, phi) = 1
    int e;
    for (e = 2; e < phi; e++) {
        if (gcd(e, phi) == 1) {
            break;
        }
    }

    // Calculate d such that d*e ≡ 1 (mod phi)
    int d = modInverse(e, phi);

    cout << "Public key: (" << e << ", " << n << ")" << endl;
    cout << "Private key: (" << d << ", " << n << ")" << endl;

    // Message to be encrypted
    long long msg = 65;

    // Encrypt the message
    long long encrypted = modExp(msg, e, n);
    cout << "Encrypted message: " << encrypted << endl;

    // Decrypt the message
    long long decrypted = modExp(encrypted, d, n);
    cout << "Decrypted message: " << decrypted << endl;

    return 0;
}
