paiS = {'0': 'E', '1': '4', '2': 'D', '3': '1',
        '4': '2', '5': 'F', '6': 'B', '7': '8',
        '8': '3', '9': 'A', 'A': '6', 'B': 'C',
        'C': '5', 'D': '9', 'E': '0', 'F': '7'}

paiP = {1: 1, 2: 5, 3: 9, 4: 13,
        5: 2, 6: 6, 7: 10, 8: 14,
        9: 3, 10: 7, 11: 11, 12: 15,
        13: 4, 14: 8, 15: 12, 16: 16}

l = m = Nr = 4

# 密钥生成（注意生成的序列k中，k[0]是0）
def get_krs(K, num):
    # 定义一个空列表存储k数组中的kr，空出k[0]
    k = [0]

    Klen = len(K)

    # 生成num个密钥
    for r in range(1, num + 1):
        # 计算起始位置，注意索引从0开始，(4r - 3) 要减1
        start = (4 * r - 3) - 1

        # 初始化kr
        kr = ""

        # 从start开始，循环获取16个字符
        for i in range(16):
            # 计算循环索引
            index_now = (start + i) % Klen
            kr += K[index_now]

        # 将生成的kr转为二进制int加入数组k
        kr = int(kr, 2)
        k.append(kr)

    return k

# s置换(参数与返回值都是二进制int)
def s_block(u_int):
    u_hex = hex(u_int)[2:].upper().zfill(4)  # u四位一组变成十六进制字符串
    u_hex = list(u_hex)
    for i in range(1, m + 1):
        u_hex[i - 1] = paiS[u_hex[i - 1]]
    ustr = ''.join(u_hex)  # 代换后十六进制字符串
    uint = int(ustr, 16)  # 转为int

    return uint

# p代换(参数与返回值都是二进制int)
def p_block(v_int):
    v_bin = bin(v_int)[2:].zfill(16)  # 转为二进制字符串，便于置换
    v_bin = list(v_bin)
    v_bin_after = ['0'] * 16
    for i in range(0, 16):
        v_bin_after[i] = v_bin[(paiP[i + 1]) - 1]
    vstr = ''.join(v_bin_after)  # 置换后二进制字符串
    vint = int(vstr, 2)  # 转为int

    return vint


if __name__ == "__main__":
    x = input()  # "0010011010110111"
    K = input()  # "00111010100101001101011000111111"
    x = int(x, 2)

    k = get_krs(K, Nr + 1)
    w = [0] * Nr  # 存每轮初始密文（p置换结果）
    u = [0] * (Nr + 1)  # 存异或结果，共Nr个，0位置空出
    v = [0] * (Nr + 1)  # 存s代换结果，共Nr个，0位置空出
    w[0] = x

    # 轮次循环，1到Nr-1轮
    for r in range(1, Nr):
        # 异或
        u[r] = w[r - 1] ^ k[r]
        # s代换
        v[r] = s_block(u[r])  # 存入v[r]
        # p置换
        w[r] = p_block(v[r])  # 存入w[r]

    # 最后一轮，第Nr轮
    # 异或
    u[Nr] = w[Nr-1] ^ k[Nr]
    # s代换
    v[Nr] = s_block(u[Nr])  # 存入v[Nr]
    # 最终异或
    y = v[Nr] ^ k[Nr+1]
    y_str = bin(y)[2:].zfill(16)

    print(y_str)