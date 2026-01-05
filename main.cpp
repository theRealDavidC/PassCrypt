#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

string toBase64(vector<unsigned char> data) {
    string result = "";
    int i = 0;
    int j = 0;
    unsigned char arr3[3];
    unsigned char arr4[4];
    
    for(size_t n = 0; n < data.size(); n++) {
        arr3[i++] = data[n];
        if(i == 3) {
            arr4[0] = (arr3[0] & 0xfc) >> 2;
            arr4[1] = ((arr3[0] & 0x03) << 4) + ((arr3[1] & 0xf0) >> 4);
            arr4[2] = ((arr3[1] & 0x0f) << 2) + ((arr3[2] & 0xc0) >> 6);
            arr4[3] = arr3[2] & 0x3f;
            
            for(i = 0; i < 4; i++)
                result += base64_chars[arr4[i]];
            i = 0;
        }
    }
    
    if(i) {
        for(j = i; j < 3; j++)
            arr3[j] = '\0';
            
        arr4[0] = (arr3[0] & 0xfc) >> 2;
        arr4[1] = ((arr3[0] & 0x03) << 4) + ((arr3[1] & 0xf0) >> 4);
        arr4[2] = ((arr3[1] & 0x0f) << 2) + ((arr3[2] & 0xc0) >> 6);
        
        for(j = 0; j < i + 1; j++)
            result += base64_chars[arr4[j]];
            
        while(i++ < 3)
            result += '=';
    }
    
    return result;
}

vector<unsigned char> fromBase64(string encoded) {
    vector<unsigned char> result;
    int i = 0;
    int j = 0;
    int in = 0;
    unsigned char arr4[4], arr3[3];
    
    for(size_t n = 0; n < encoded.length(); n++) {
        if(encoded[n] == '=') break;
        
        size_t pos = base64_chars.find(encoded[n]);
        if(pos == string::npos) continue;
        
        arr4[i++] = pos;
        if(i == 4) {
            arr3[0] = (arr4[0] << 2) + ((arr4[1] & 0x30) >> 4);
            arr3[1] = ((arr4[1] & 0xf) << 4) + ((arr4[2] & 0x3c) >> 2);
            arr3[2] = ((arr4[2] & 0x3) << 6) + arr4[3];
            
            for(i = 0; i < 3; i++)
                result.push_back(arr3[i]);
            i = 0;
        }
    }
    
    if(i) {
        for(j = i; j < 4; j++)
            arr4[j] = 0;
            
        arr3[0] = (arr4[0] << 2) + ((arr4[1] & 0x30) >> 4);
        arr3[1] = ((arr4[1] & 0xf) << 4) + ((arr4[2] & 0x3c) >> 2);
        
        for(j = 0; j < i - 1; j++)
            result.push_back(arr3[j]);
    }
    
    return result;
}

unsigned int hashKey(string key) {
    unsigned int h = 0;
    for(size_t i = 0; i < key.length(); i++) {
        h = h * 31 + (unsigned char)key[i];
    }
    return h;
}

vector<unsigned char> generateKeystream(string key, int len) {
    vector<unsigned char> stream;
    unsigned int seed = hashKey(key);
    
    unsigned int a = 1664525;
    unsigned int c = 1013904223;
    unsigned int state = seed;
    
    for(int i = 0; i < len; i++) {
        state = (a * state + c);
        stream.push_back((state >> 16) & 0xFF);
    }
    
    return stream;
}

string encrypt(string plaintext, string key) {
    if(plaintext.empty()) return "";
    if(key.empty()) {
        cout << "Error: Key cannot be empty!" << endl;
        return "";
    }
    
    vector<unsigned char> keystream = generateKeystream(key, plaintext.length());
    vector<unsigned char> encrypted;
    
    for(size_t i = 0; i < plaintext.length(); i++) {
        unsigned char byte = (unsigned char)plaintext[i] ^ keystream[i];
        encrypted.push_back(byte);
    }
    
    return toBase64(encrypted);
}

string decrypt(string ciphertext, string key) {
    if(ciphertext.empty()) return "";
    if(key.empty()) {
        cout << "Error: Key cannot be empty!" << endl;
        return "";
    }
    
    vector<unsigned char> encrypted = fromBase64(ciphertext);
    vector<unsigned char> keystream = generateKeystream(key, encrypted.size());
    string decrypted = "";
    
    for(size_t i = 0; i < encrypted.size(); i++) {
        unsigned char byte = encrypted[i] ^ keystream[i];
        decrypted += (char)byte;
    }
    
    return decrypted;
}

void showDemo() {
    cout << "\n========================================\n";
    cout << "      PassCrypt Demo\n";
    cout << "========================================\n\n";
    
    string password = "1234";
    string key = "my-secret-key";
    
    cout << "Original password: " << password << endl;
    cout << "Secret key: " << key << endl;
    
    string enc = encrypt(password, key);
    cout << "\nEncrypted: " << enc << endl;
    
    string dec = decrypt(enc, key);
    cout << "Decrypted: " << dec << endl;
    cout << "Match: " << (dec == password ? "YES" : "NO") << endl;
    
    cout << "\n--- Testing with different key ---\n";
    string wrongKey = "wrong-key";
    string wrongDec = decrypt(enc, wrongKey);
    cout << "Decrypt with wrong key: " << wrongDec << endl;
    cout << "Match: " << (wrongDec == password ? "YES" : "NO") << endl;
}

void interactiveMode() {
    string choice;
    
    while(true) {
        cout << "\n========================================\n";
        cout << "1. Encrypt password\n";
        cout << "2. Decrypt password\n";
        cout << "3. Exit\n";
        cout << "========================================\n";
        cout << "Choose option: ";
        
        getline(cin, choice);
        
        if(choice == "3") break;
        
        if(choice == "1") {
            string pass, k;
            cout << "\nEnter password: ";
            getline(cin, pass);
            cout << "Enter key: ";
            getline(cin, k);
            
            string result = encrypt(pass, k);
            if(!result.empty()) {
                cout << "\nEncrypted: " << result << endl;
            }
        }
        else if(choice == "2") {
            string cipher, k;
            cout << "\nEnter encrypted text: ";
            getline(cin, cipher);
            cout << "Enter key: ";
            getline(cin, k);
            
            string result = decrypt(cipher, k);
            if(!result.empty()) {
                cout << "\nDecrypted: " << result << endl;
            }
        }
        else {
            cout << "Invalid option!\n";
        }
    }
}

int main() {
    cout << "\n";
    cout << "╔══════════════════════════════════════════════╗\n";
    cout << "║         PassCrypt v1.0                       ║\n";
    cout << "║  Educational Password Encryption Tool        ║\n";
    cout << "║                                              ║\n";
    cout << "║  WARNING: For learning purposes only!       ║\n";
    cout << "║  NOT for production password storage!       ║\n";
    cout << "╚══════════════════════════════════════════════╝\n";
    
    showDemo();
    
    cout << "\n\nPress Enter to try interactive mode...";
    cin.get();
    
    interactiveMode();
    
    cout << "\nGoodbye!\n";
    return 0;
}
