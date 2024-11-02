# πs逆置换,val为整数0-15
def pais_inverse(val):
    paiS_inverse = {'E': '0', '4': '1', 'D': '2', '1': '3',
                    '2': '4', 'F': '5', 'B': '6', '8': '7',
                    '3': '8', 'A': '9', '6': 'A', 'C': 'B',
                    '5': 'C', '9': 'D', '0': 'E', '7': 'F'}
    # 将整数转换为十六进制字符串
    hex_val = format(val, 'X')  # 大写十六进制
    after = paiS_inverse[hex_val]
    return int(after, 16)


# 线性攻击算法
# linear_attack(2,4)
def linear_attack1(pairs, T):
    Count = [0] * 256  # 每个密钥的计数器
    for (x, y) in pairs:
        for i in range(256):
            L1 = (i >> 4) & 0xF  # 提取密钥高4位
            L2 = i & 0xF  # 提取密钥低4位
            # 取出四段密文，转为int
            y2 = int(y[4:8], 2)
            y4 = int(y[12:16], 2)

            # 与对应密钥进行异或运算，得到两个v
            v2 = L1 ^ y2
            v4 = L2 ^ y4

            # 逆映射得到u
            u2 = pais_inverse(v2)
            u4 = pais_inverse(v4)

            # 将u转换为二进制字符串并去掉 '0b' 前缀
            # 确保有4位，不足的用前面补0
            u2_str = bin(u2)[2:].zfill(4)
            u4_str = bin(u4)[2:].zfill(4)

            # 计算z
            x5 = int(x[4], 2)
            x7 = int(x[6], 2)
            x8 = int(x[7], 2)
            u6 = int(u2_str[1], 2)
            u8 = int(u2_str[3], 2)
            u14 = int(u4_str[1], 2)
            u16 = int(u4_str[3], 2)
            z = x5 ^ x7 ^ x8 ^ u6 ^ u8 ^ u14 ^ u16

            if z == 0:  # 异或结果为0，对应计数器加1
                Count[i] += 1


    # 最大计数
    maxnum = -1
    maxkey = 0
    for i in range(256):
        Count[i] = abs(Count[i] - T / 2)
        if Count[i] > maxnum:
            maxnum = Count[i]
            maxkey = i

    maxkey_str = bin(maxkey)[2:].zfill(8)
    print("2、4块最可能的子密钥：", maxkey_str)

    return maxkey

# linear_attack(1,3)
def linear_attack2(pairs, T, L2, L4):
    Count = [0] * 256  # 每个密钥的计数器
    for (x, y) in pairs:
        for i in range(256):
            L1 = (i >> 4) & 0xF  # 提取密钥高4位
            L3 = i & 0xF  # 提取密钥低4位

            # 取出四段密文，转为int
            y1 = int(y[0:4], 2)
            y2 = int(y[4:8], 2)
            y3 = int(y[8:12], 2)
            y4 = int(y[12:16], 2)

            # 与对应密钥进行异或运算，得到v
            v1 = L1 ^ y1
            v2 = L2 ^ y2
            v3 = L3 ^ y3
            v4 = L4 ^ y4

            # 逆映射得到u
            u1 = pais_inverse(v1)
            u2 = pais_inverse(v2)
            u3 = pais_inverse(v3)
            u4 = pais_inverse(v4)

            # 将u转换为二进制字符串并去掉 '0b' 前缀
            # 确保有4位，不足的用前面补0
            u1_str = bin(u1)[2:].zfill(4)
            u2_str = bin(u2)[2:].zfill(4)
            u3_str = bin(u3)[2:].zfill(4)
            u4_str = bin(u4)[2:].zfill(4)

            # 计算z
            # 分析2
            x1 = int(x[0], 2)
            x2 = int(x[1], 2)
            x4 = int(x[3], 2)
            x9 = int(x[8], 2)
            x10 = int(x[9], 2)
            x12 = int(x[11], 2)
            u1 = int(u1_str[0], 2)
            u3 = int(u1_str[2], 2)
            u5 = int(u2_str[0], 2)
            u7 = int(u2_str[2], 2)
            u9 = int(u3_str[0], 2)
            u11 = int(u3_str[2], 2)
            u13 = int(u4_str[0], 2)
            u15 = int(u4_str[2], 2)
            z1 = x1 ^ x2 ^ x4 ^ u1 ^ u5 ^ u9 ^ u13
            z2 = x9 ^ x10 ^ x12 ^ u3 ^ u7 ^ u11 ^ u15

            if z1+z2 == 0:  # 二者异或结果都为0，对应计数器加1
                Count[i] += 1


    # 最大计数
    maxnum = -1
    maxkey = 0
    for i in range(256):
        Count[i] = abs(Count[i] - T / 2)
        if Count[i] > maxnum:
            maxnum = Count[i]
            maxkey = i

    maxkey_str = bin(maxkey)[2:].zfill(8)
    print("1、3块最可能的子密钥：", maxkey_str)

    return maxkey


if __name__ == "__main__":
    pairs = []
    # 打开两个文件并逐行读取
    with open("xs.txt", 'r') as f1, open("ys2.txt", 'r') as f2:
        for line1, line2 in zip(f1, f2):  # 使用 zip 同时遍历两个文件
            # 去掉行末的换行符，并构造元组
            pair = (line1.strip(), line2.strip())
            pairs.append(pair)  # 将元组添加到列表中

    T = len(pairs)  # 明密文对的个数
    # 执行线性攻击1
    L = linear_attack1(pairs, T)
    # 获得2、4部分密钥
    L2 = (L >> 4) & 0xF
    L4 = L & 0xF
    # 执行线性攻击2
    L = linear_attack2(pairs, T, L2, L4)
    # 获得1、3部分密钥
    L1 = (L >> 4) & 0xF
    L3 = L & 0xF

    # 拼接得最终密钥
    maxkey_str = (bin(L1)[2:].zfill(4) + ' ' +
                  bin(L2)[2:].zfill(4) + ' ' +
                  bin(L3)[2:].zfill(4) + ' ' +
                  bin(L4)[2:].zfill(4))
    print("最可能的轮密钥结果：",maxkey_str)