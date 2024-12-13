#include <iostream>
#include <climits>
using namespace std;

// a 的 n 次方模 m
int anm(int a, int n, int m) {
    int result = 1;
    while (n > 0) {
        if (n % 2 == 1) {
            result = a * result % m;
        }
        a = a * a % m;
        n = n / 2;
    }
    return result;
}

// 求最大公约数
int gcd(int a, int b) { return b ? gcd(b, a % b) : a; }

// 扩展欧几里得算法求逆元
int inverse(int a, int m) {
    while (a < 0) a += m;
    if (gcd(a, m) != 1) return -1;
    int r, q, s1 = 1, s2 = 0, s3, t1 = 0, t2 = 1, t3 = 1, mt = m;
    while (1) {
        r = m % a; q = m / a;
        if (!r) break;
        m = a; a = r;
        s3 = s1 - q * s2;
        t3 = t1 - q * t2;
        s1 = s2; s2 = s3;
        t1 = t2; t2 = t3;
    }
    while (t3 < 0) t3 += mt;
    return t3;
}

// Hash(x)：2 的 x 次方模 13
int Hash(int x) {
    return anm(2, x, 13);
}

// 点
struct Point {
    int x, y;
    bool inf; // 记录是否为无穷远点
    // 创建平常点
    Point(int x, int y) : x(x), y(y), inf(false) {}
    // 创建无穷远点
    Point(bool inf) : x(0), y(0), inf(inf) {} // 用于创建无穷远点
    // 重载等于号
    bool operator==(const Point& a) const {
        // 无穷远点
        if (inf != a.inf) return false;
        if (inf && a.inf) return true;
        // 平常点
        return x == a.x && y == a.y;
    }
    // 重载不等于号
    bool operator!=(const Point& other) const { return !(*this == other); }
};

// 曲线
class EllipticCurve {
public:
    int a, b, p;
    Point ZERO; // 无穷远点
    EllipticCurve(int a, int b, int p) : a(a), b(b), p(p), ZERO(true) {}
    
    // 计算 P+P，两点之和相同点情况
    Point doublePoint(const Point& P) {
        int k = ((3 * P.x * P.x + a) * inverse(2 * P.y, p)) % p;
        // 负 k 转正
        while (k < 0) k += p;
        int xR = (k * k - 2 * P.x) % p;
        // 负 x 转正
        while (xR < 0) xR += p;
        int yR = (k * (P.x - xR) - P.y) % p;
        // 负 y 转正
        while (yR < 0) yR += p;
        return Point(xR, yR);
    }
    // 计算两点之和
    Point addPoints(const Point& P, const Point& Q) {
        // 无穷远点返回另一个
        if (P == ZERO) return Q;
        if (Q == ZERO) return P;
        // 两点相同
        if (P == Q) return doublePoint(P);
        // x 点坐标相等，但 y 不相等，无穷远点
        if (P.x == Q.x) return ZERO;
        // 都不相等
        int k = ((Q.y - P.y) * inverse(Q.x - P.x, p)) % p;
        while (k < 0) k += p;
        int xR = (k * k - P.x - Q.x) % p;
        while (xR < 0) xR += p;
        int yR = (k * (P.x - xR) - P.y) % p;
        while (yR < 0) yR += p;
        return Point(xR, yR);
    }
    // 使用倍加-和算法计算 mP
    Point multiplyPoint(const Point& P, int m) {
        Point R = ZERO;
        Point Q = P;
        while (m) {
            if (m & 1) R = addPoints(R, Q);
            Q = doublePoint(Q);
            m >>= 1;
        }
        return R;
    }
    // 计算点的阶
    int orderOfPoint(const Point& P) {
        // 无穷远点
        if (P == ZERO) return INT_MAX;
        Point Q = P;
        int order = 1;
        // 循环加，直到为无穷远点
        while (Q != ZERO) {
            Q = addPoints(P, Q);
            order++;
        }
        return order;
    }
};

int main() {
	int x0, y0, m, x, k;
	cin >> x0 >> y0 >> m >> x >> k;

    Point A(x0, y0);
    EllipticCurve Z(1, 6, 11);
    int q = Z.orderOfPoint(A);  // A的阶数

    // sig(x, k) = (r, s)
    Point B = Z.multiplyPoint(A, m);  // B = mA
    Point kA = Z.multiplyPoint(A, k);
    int u = kA.x;
    int v = kA.y;

    int r = u % q;
    int s = (inverse(k, q) * (Hash(x) + m * r)) % q;

    cout << r << " " << s;

    return 0;
}