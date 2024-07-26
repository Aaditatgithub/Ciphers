#include <iostream>
#include <string>
#include <vector>
#include <bitset>
using namespace std;

std::string w0, w1, w2, w3, w4, w5;
std::string roundConstant1 = "10000000";
std::string roundConstant2 = "00110000";
string key1, key2, key3;

vector<vector<string>> sbox = {{"1001", "0100", "1010", "1011"}, 
{"1101", "0001", "1000", "0101"}, 
{"0110", "0010", "0000", "0011"}, 
{"1100", "1110", "1111", "0111"}};

std::string xorStrings(const std::string &str1, const std::string &str2) {
    string xor_result = "";
    for (size_t i = 0; i < str1.size(); i++) {
        char xor_char = (str1[i] == '1' ? 1 : 0) ^ (str2[i] == '1' ? 1 : 0);
        xor_result += (xor_char ? '1' : '0');
    }
    return xor_result;
}

std::string rotateNibble(const std::string &s) {
    return s.substr(4, 4) + s.substr(0, 4);
}

int getMatrixCoordinates(const std::string &s) {
    if (s == "00") return 0;
    if (s == "01") return 1;
    if (s == "10") return 2;
    if (s == "11") return 3;
    return -1;
}

std::string subNibble(const std::string &s) {
    string result = "";
    for (size_t i = 0; i < s.size(); i += 4) {
        string nibble = s.substr(i, 4);
        int r = getMatrixCoordinates(nibble.substr(0, 2));
        int c = getMatrixCoordinates(nibble.substr(2, 2));
        result += sbox[r][c];
    }
    return result;
}

void generate_subkeys(const string &key) {
    w0 = key.substr(0, 8);
    w1 = key.substr(8, 8);

    w2 = xorStrings(w0, roundConstant1);
    w2 = xorStrings(w2, subNibble(rotateNibble(w1)));
    
    w3 = xorStrings(w2, w1);

    w4 = xorStrings(w2, roundConstant2);
    w4 = xorStrings(w4, subNibble(rotateNibble(w3)));

    w5 = xorStrings(w4, w3);

    key1 = w0 + w1;
    key2 = w2 + w3;
    key3 = w4 + w5;

    cout << "Key 1: " << key1 << endl;
    cout << "Key 2: " << key2 << endl;
    cout << "Key 3: " << key3 << endl;
}

std::string shiftRow(const std::string &s) {
    if (s.size() != 16) {
        throw std::invalid_argument("String length must be 16 for shiftRow");
    }
    string shifted = s.substr(0, 4) + s.substr(12, 4) + s.substr(8, 4) +  s.substr(4, 4);
    return shifted;
}

std::string mixColumns(const std::string &s) {
    int mixMatrix[2][2] = {{1, 4}, {4, 1}};
    int stateMatrix[2][2];
    
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            stateMatrix[i][j] = stoi(s.substr(8 * i + 4 * j, 4), 0, 2);
        }
    }

    int mixedMatrix[2][2];
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            mixedMatrix[i][j] = 0;
            for (int k = 0; k < 2; k++) {
                int mul = mixMatrix[i][k] * stateMatrix[k][j];
                mixedMatrix[i][j] ^= (mul ^ ((mul & 0x10) >> 4) * 0x13) & 0xF;
            }
        }
    }

    string result = "";
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            result += bitset<4>(mixedMatrix[i][j]).to_string();
        }
    }
    return result;
}

std::string encrypt(const std::string &input) {
    if (input.size() != 16) {
        throw std::invalid_argument("Input length must be 16 for encrypt");
    }
    std::string cipher = input;
    cout << "Initial input: " << cipher << endl;
    // Add round key 1
    cipher = xorStrings(cipher, key1);
    cout << "After round key 1: " << cipher << endl;

    // Start round 1
    // Nibble substitution
    cipher = subNibble(cipher);
    cout << "After subNibble: " << cipher << endl;
    // Shift row
    cipher = shiftRow(cipher);
    cout << "After shiftRow: " << cipher << endl;
    // Mix columns
    cipher = mixColumns(cipher);
    cout << "After mixColumns: " << cipher << endl;
    // Add round key 2
    cipher = xorStrings(cipher, key2);
    cout << "After round key 2: " << cipher << endl;

    // Start round 2
    // Nibble substitution
    cipher = subNibble(cipher);
    cout << "After subNibble: " << cipher << endl;
    // Shift row
    cipher = shiftRow(cipher);
    cout << "After shiftRow: " << cipher << endl;
    // Add round key 3
    cipher = xorStrings(cipher, key3);
    cout << "After round key 3: " << cipher << endl;

    return cipher;
}

int main() {
    string plaintext = "1101011100101000";
    string key = "0100101011110101";
    generate_subkeys(key);
    string ciphertext = encrypt(plaintext);

    cout << "Plaintext: " << plaintext << endl;
    cout << "Ciphertext: " << ciphertext << endl;

    return 0;
}
