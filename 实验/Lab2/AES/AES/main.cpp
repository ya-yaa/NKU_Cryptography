#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>
using namespace std;

const uint8_t S_BOX[16][16] = {
    {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76},
    {0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0},
    {0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15},
    {0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75},
    {0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84},
    {0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf},
    {0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8},
    {0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2},
    {0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73},
    {0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb},
    {0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79},
    {0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08},
    {0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a},
    {0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e},
    {0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf},
    {0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16}
};

const uint8_t RCON[10][4] = {
    {0x01, 0x00, 0x00, 0x00},
    {0x02, 0x00, 0x00, 0x00},
    {0x04, 0x00, 0x00, 0x00},
    {0x08, 0x00, 0x00, 0x00},
    {0x10, 0x00, 0x00, 0x00},
    {0x20, 0x00, 0x00, 0x00},
    {0x40, 0x00, 0x00, 0x00},
    {0x80, 0x00, 0x00, 0x00},
    {0x1b, 0x00, 0x00, 0x00},
    {0x36, 0x00, 0x00, 0x00}
};

const uint8_t MIX[4][4] = {
    {0x02, 0x03, 0x01, 0x01},
    {0x01, 0x02, 0x03, 0x01},
    {0x01, 0x01, 0x02, 0x03},
    {0x03, 0x01, 0x01, 0x02}
};

// 轮密钥数组
uint8_t round_keys[4][44] = {0};
// state数组
uint8_t state[4][4];

// 密钥扩展算法
void KeyExpansion(string& key) {
    // 初始密钥，先转16进制一维矩阵
    uint8_t hex_key[16];
    int j = 0;
    for (size_t i = 0; i < 32; i += 2) {
        hex_key[j] = stoi(key.substr(i, 2), nullptr, 16);
        // cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hex_key[j]) << " ";
        j++;
    }

    // 初始密钥为 0 轮密钥，加入矩阵
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            round_keys[j][i] = hex_key[4 * i + j];
            // cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>( round_keys[j][i]) << " ";
        }
        // cout << endl;
    }

    // 循环计算十轮密钥
    for (int i = 4; i < 44; i++) {
        if (i % 4 == 0) {
            // 获取前一列的数据
            uint8_t temp[4];
            for (int j = 0; j < 4; j++) {
                temp[j] = round_keys[j][i - 1];
            }

            // 字循环，左移1位
            uint8_t t = temp[0];
            for (int j = 0; j < 3; j++) {
                temp[j] = temp[j + 1];
            }
            temp[3] = t;

            // 字节代换
            for (int j = 0; j < 4; j++) {
                int high = temp[j] >> 4;
                int low = temp[j] & 0x0f;
                temp[j] = S_BOX[high][low];
            }

            // 与轮常量异或
            temp[0] ^= RCON[(i / 4) - 1][0];

            // 更新 round_keys
            for (int j = 0; j < 4; j++) {
                round_keys[j][i] = round_keys[j][i - 4] ^ temp[j];
            }
        }
        else {
            // 正常情况下，直接与第 i-4 列异或
            for (int j = 0; j < 4; j++) {
                round_keys[j][i] = round_keys[j][i - 4] ^ round_keys[j][i - 1];
            }
        }
    }
}

// 轮密钥加
void AddRoundKey(int round) {
    for (int i = 0; i < 4; i++) {  // 列数
        for (int j = 0; j < 4; j++) {  // 行数
            state[j][i] ^= round_keys[j][i + round * 4];
        }
    }
}

// 字节代换
void SubBytes() {
    for (int i = 0; i < 4; i++) {  
        for (int j = 0; j < 4; j++) { 
            int high = state[i][j] >> 4;
            int low = state[i][j] & 0x0f;
            state[i][j] = S_BOX[high][low];
        }
    }
}

// 行移位
void ShiftRows() {
    // 第0行不变
    // 第1行左移1
    uint8_t temp = state[1][0];
    for (int i = 0; i < 3; i++) {
        state[1][i] = state[1][i + 1];
    }
    state[1][3] = temp;

    // 第2行左移2
    uint8_t temp1 = state[2][0];
    uint8_t temp2 = state[2][1];
    for (int i = 0; i < 2; i++) {
        state[2][i] = state[2][i + 2];
    }
    state[2][2] = temp1;
    state[2][3] = temp2;

    // 第3行左移3(就是右移一位)
    uint8_t temp3 = state[3][3];
    for (int i = 3; i > 0; i--) {
        state[3][i] = state[3][i - 1];
    }
    state[3][0] = temp3;
}

// 有限域乘
uint8_t Multi(uint8_t a, uint8_t b) {
    if (a == 0x01) {
        return b;
    }
    else if (a == 0x02) {
        if (b >> 7 == 0) {
            return (b << 1) & 0xFF;
        }
        else {
            return ((b << 1) ^ 0x1B) & 0xFF;
        }
    }
    else if (a == 0x03) {
        return (b ^ Multi(0x02, b)) & 0xFF;
    }
}

// 列混合
void MixColumns() {
    uint8_t temp[4][4];

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            temp[i][j] = (Multi(MIX[i][0], state[0][j])
                ^ Multi(MIX[i][1], state[1][j])
                ^ Multi(MIX[i][2], state[2][j])
                ^ Multi(MIX[i][3], state[3][j])) & 0xFF;
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[i][j] = temp[i][j];
        }
    }
}

// AES
string AES(string& key) {
    
    KeyExpansion(key);

    AddRoundKey(0);

    // 9轮加密
    for (int round = 1; round < 10; round++) {
        SubBytes();
        ShiftRows();
        MixColumns();
        AddRoundKey(round);
    }

    // 最后一轮
    SubBytes();
    ShiftRows();
    AddRoundKey(10);

    stringstream ss;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ss << hex << setw(2) << setfill('0') << static_cast<int>(state[j][i]);
        }
    }

    return ss.str();
}



int main() {

	string key, plaintext;
    cin >> key;  // = "000102030405060708090a0b0c0d0e0f";
    cin >> plaintext;  // = "00112233445566778899aabbccddeeff";

	// 密文转为矩阵
    uint8_t hex_plain[16];  // 先转16进制一维矩阵
    int j = 0;
    for (size_t i = 0; i < 32; i += 2) {
        hex_plain[j] = stoi(plaintext.substr(i, 2), nullptr, 16);
        // cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hex_plain[j]) << " ";
        j++;
    }

    // 再转4*4矩阵
    for (int i = 0; i < 4; i++) {  // 列数
        for (int j = 0; j < 4; j++) {  // 行数
            state[j][i] = hex_plain[4 * i + j];
        }
    }

    // 调用AES
    string ciphertext = AES(key);
    
    // 输出
    for (char& c : ciphertext) {
        c = std::toupper(c);
    }
    cout << ciphertext;

    return 0;
}