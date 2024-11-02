from collections import Counter
import string

# 字母映射到数字
letter_to_num = {letter: idx for idx, letter in enumerate(string.ascii_uppercase)}

# 数字映射到字母
num_to_letter = {idx: letter for idx, letter in enumerate(string.ascii_uppercase)}

# 字母频率
letter_p = {'A': 0.082, 'B': 0.015, 'C': 0.028, 'D': 0.043, 'E': 0.127, 'F': 0.022, 'G': 0.020, 'H': 0.061,
            'I': 0.070, 'J': 0.002, 'K': 0.008, 'L': 0.040, 'M': 0.024, 'N': 0.067, 'O': 0.075, 'P': 0.019,
            'Q': 0.001, 'R': 0.060, 'S': 0.063, 'T': 0.091, 'U': 0.028, 'V': 0.010, 'W': 0.023, 'X': 0.001,
            'Y': 0.020, 'Z': 0.001}

# 已经变成普通维吉尼亚加密的密文
s = 'IYMYSHKNMQDLAOVNGBAPDXQEXXEPUWIFZKSLVATMHWIYIAVAEXDNPMXWVLUHIYIZXDNPMXWVAMMSRVPXDKPQWTEAMXXFVMPLRGURLYQOLWVAMMSZBMRSKPUWLRAYSHKXQVUVKFPNZVOLPIIOXTINAVWVZNIGKWEFDIVQCPILXINNIETTIPMTHKPQQDJBQRVROURVRZFMRLTMXTRVPXTTPZMRFTAVTUXQVHFVMPXKGURIYMNYHZVQWH'

# 密钥长度
l = 5

# 密钥字
key = []


# 计算当前密文字g对应的Mg，s为当前密文段
def get_Mg(g, s):
    Mg = 0
    frequency = Counter(s)  # 统计密文中每一个大写字母的频率
    # 累加26个英文单词的情况
    for i in range(26):
        now_letter = num_to_letter[i]  # 当前字母
        pi = letter_p[now_letter]
        n = len(s)
        f = frequency[num_to_letter[(i + g) % 26]]  # i+g加密后字母在密文中出现的频率
        Mg += pi * f / n
    return Mg


# 切分子字符串，每一个子字符串的加密字相同
for i in range(l):
    Mgs = []
    part_text = s[i::l]
    # 循环尝试26个密钥字
    for g in range(26):
        Mg = get_Mg(g, part_text)
        Mgs.append(Mg)
    print(Mgs)
    # 第i个子字符串的所有Mg计算完成，找到最接近0.065的
    num = max(enumerate(Mgs), key=lambda x: (x[1] - 0.065))[0]
    print("第", i, "个密钥字为：", num_to_letter[num])
    print("对应的Mg值为：", Mgs[num])
    key.append(num_to_letter[num])

keyword = ''.join(key)
print("找到最可能的密钥字为：", keyword)  # PRIME
