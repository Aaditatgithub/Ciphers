#include <iostream>
#include <string>
#include<bitset>
#include <utility>
using namespace std;

// Global Constants for the algorithm
int P10[] = { 3, 5, 2, 7, 4, 10, 1, 9, 8, 6 };
int P8[] = { 6, 3, 7, 4, 8, 5, 10, 9 };
int P4[] = { 2, 4, 3, 1 };

//Initial and Reverse Permutations
int IP[] = { 2, 6, 3, 1, 4, 8, 5, 7 };
int IP_inv[] = { 4, 1, 3, 5, 7, 2, 8, 6 };


//Expansion Pemutation
int E_P[] = { 4, 1, 2, 3, 2, 3, 4, 1 };


//Swap Boxes for substitution
int S0[4][4] = {
    {1, 0, 3, 2},
    {3, 2, 1, 0},
    {0, 2, 1, 3},
    {3, 1, 3, 2}
};

int S1[4][4] = {
    {0, 1, 2, 3},
    {2, 0, 1, 3},
    {3, 0, 1, 0},
    {2, 1, 0, 3}
};

// Function to perform XOR between two strings
string XOR(const string& a, const string& b) {
    string result = "";
    for (size_t i = 0; i < a.size(); i++) {
        result += (a[i] == b[i] ? '0' : '1');
    }
    return result;
}

// Function to perform S-box substitution
string sBoxSubstitution(const string& input, int sBox[4][4]) {
    int row = 2 * (input[0] - '0') + (input[3] - '0');
    int col = 2 * (input[1] - '0') + (input[2] - '0');
    int val = sBox[row][col];
    return bitset<2>(val).to_string();
}

// Function to perform permutation using a given table
string permute(const string& key, const int* perm, int n) {
    string permuted_key = "";
    for (int i = 0; i < n; i++) {
        permuted_key += key[perm[i] - 1];
    }
    return permuted_key;
}

// Function to perform a left circular shift by 1
string leftShift(const string& key, int shifts) {
    string shifted_key = key.substr(shifts) + key.substr(0, shifts);
    return shifted_key;
}

// Key Generation
void generateKeys(string key, string& k1, string& k2) {
    cout << "Original Key: " << key << endl;

    // Step 1: Apply P10 permutation
    string permuted_key = permute(key, P10, 10);
    cout << "Permuted Key (P10): " << permuted_key << endl;

    // Step 2: Split into two halves and perform left circular shift by 1
    string left_key = permuted_key.substr(0, 5);
    string right_key = permuted_key.substr(5, 5);
    left_key = leftShift(left_key, 1);
    right_key = leftShift(right_key, 1);
    cout << "After LCS-1: Left Key: " << left_key << ", Right Key: " << right_key << endl;

    // Step 3: Apply P8 permutation to generate k1
    string combined_key = left_key + right_key;
    k1 = permute(combined_key, P8, 8);
    cout << "Key 1 (P8): " << k1 << endl;

    // Step 4: Perform left circular shift by 2
    left_key = leftShift(left_key, 2);
    right_key = leftShift(right_key, 2);
    cout << "After LCS-2: Left Key: " << left_key << ", Right Key: " << right_key << endl;

    // Step 5: Apply P8 permutation again to generate k2
    combined_key = left_key + right_key;
    k2 = permute(combined_key, P8, 8);
    cout << "Key 2 (P8): " << k2 << endl;
}

// Feistel function fK
string fK(const string& R, const string& K) {
    // Step 1: Expansion/Permutation (E/P)
    string ep = permute(R, E_P, 8);
    // Step 2: XOR with key
    string xored = XOR(ep, K);
    // Step 3: S-box substitution
    string left = xored.substr(0, 4);
    string right = xored.substr(4, 4);
    string s0 = sBoxSubstitution(left, S0);
    string s1 = sBoxSubstitution(right, S1);
    // Step 4: P4 permutation
    string p4 = permute(s0 + s1, P4, 4);
    return p4;
}

// Function to switch the halves
string switchHalves(const string& input) {
    return input.substr(4, 4) + input.substr(0, 4);
}


// Function to generate ciphertext using SDES
void cipherTextGeneration(const string& plaintext, const string& k1, const string& k2) {
    cout << "Plaintext: " << plaintext << endl;

    // Initial Permutation (IP)
    string permuted = permute(plaintext, IP, 8);
    cout << "After IP: " << permuted << endl;

    // Split into left and right halves
    string L = permuted.substr(0, 4);
    string R = permuted.substr(4, 4);

    // First round of Feistel function with k1
    string f1 = fK(R, k1);
    string newL = XOR(L, f1);

    // Switch the halves (SW)
    string switched = switchHalves(newL + R);

    // Split into new left and right halves
    L = switched.substr(0, 4);
    R = switched.substr(4, 4);

    // Second round of Feistel function with k2
    string f2 = fK(R, k2);
    newL = XOR(L, f2);

    // Combine the final halves
    string combined = newL + R;

    // Inverse Initial Permutation (IP^-1)
    string cipherText = permute(combined, IP_inv, 8);

    cout << "CipherText: " << cipherText << endl;
}




int main() {
    string key = "0010010111";  // Example key
    string plaintext = "10100101";
    string k1, k2;
    generateKeys(key,k1,k2);
    cipherTextGeneration(plaintext, k1, k2);


    return 0;
}
