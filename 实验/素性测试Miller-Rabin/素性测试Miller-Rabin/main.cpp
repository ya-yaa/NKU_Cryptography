#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

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

//vector<long long int> binary(long long int result) {
//    vector<long long int> bin;
//    int i = 0;
//    while (result != 0) {
//        bin.push_back(result % 2);
//        result /= 2;
//    }
//    return bin;
//}
//
//long long int my_mul(long long int base, long long int result, long long  mod) {
//    long long int temp = 0;
//    long long int tt = 0;
//    vector<long long int>bin = binary(result);
//    for (int i = 0; i < bin.size(); i++) {
//        tt = base * bin[i] % mod;
//        for (int j = 0; j < i; j++) {
//            tt = tt * 2 % mod;
//        }
//        temp = (temp + tt) % mod;
//    }
//    return temp;
//}
//

bool MillerRabin(long long int n){
	long long int k = 0;
	long long int m = n - 1;
	while ((m & 1) == 0) {  // 最后一位为0，为偶数，循环计算 
		m = m >> 1;  // 除2
		k++;
	}

	// 生成1到n-1之间的随机整数 a
	std::srand(std::time(0));
	long long int a = std::rand() % (n - 1) + 1;

    long long int b = anm(a, m, n);
    
    if (b == 1) {
        return true;
    }
    for (long long int i = 0; i < k; i++) {
        if (b == n - 1) {
            return true;
        }
        else {
            b = anm(b, 2, n);
        }
    }
    return false;
}

//bool isPrime(long long int n) {
//    // 多次调用MillerRabin，有判断为false的就退出
//    for (int i = 0; i < 6; i++) {
//        if (!MillerRabin(n)) {
//            return false;
//        }   
//    }
//    return true;
//}

int main() {
    long long int n;
    cin >> n;
    bool flag = MillerRabin(n);
    if (flag) {
        cout << "Yes";
    }
    else { cout << "No"; }

    return 0;
}