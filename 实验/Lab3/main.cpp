#include <iostream>
#include <vector>
using namespace std;

// 扩展欧几里得求乘法逆元
long long int extended_gcd(long long int a, long long int b, long long int& x, long long int& y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    long long int x1, y1;
    long long int gcd = extended_gcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return gcd;
}

long long int my_mul(long long int base, long long int result, long long mod) {
    long long int temp = 0;
    long long int power = base % mod; // 初始为 base，并逐次平方

    while (result > 0) {
        if (result % 2 == 1) {
            // 如果当前位为1，则累加当前 power
            temp = (temp + power) % mod;
        }
        // 更新 power 为下一个位的值，即 base^(2^i)
        power = (power * 2) % mod;
        result /= 2; // 移动到下一位
    }

    return temp;
}

// a 的 n 次方模 m
long long int anm(long long int a, long long int n, long long int m) {
    long long int result = 1;
    while (n > 0) {
        if (n % 2 == 1) {
            result = my_mul(a, result, m);
        }
        a = my_mul(a, a, m);
        n = n / 2;
    }
    return result;
}

int main() {
    // 两个素数
    long long int p, q;
    cin >> p >> q;
    // 公钥 e ，密文 c
    long long int e, c;
    cin >> e >> c;

    long long int n = p * q;
    long long int phi_n = (p - 1) * (q - 1);

    // 私钥 d
    long long int d;
    long long int x, y;
    extended_gcd(e, phi_n, x, y);
    d = (x % phi_n + phi_n) % phi_n; // 确保 d 为正数


    // 使用快速幂取模计算明文 m
    long long int m = anm(c, d, n);

    cout << m;

    return 0;
}
