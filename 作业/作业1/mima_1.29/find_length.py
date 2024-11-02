import string
from collections import Counter

# 密文
s = 'IYMYSILONRFNCQXQJEDSHBUIBCJUZBOLFQYSCHATPEQGQJEJNGNXZWHHGWFSUKULJQACZKKJOAAHGKEMTAFGMKVRDOPXNEHEKZNKFSKIFRQVHHOVXINPHMRTJPYWQGJWPUUVKFPOAWPMRKKQZWLQDYAZDRMLPBJKJOBWIWPSEPVVQMBCRYVCRUZAAOUMBCHDAGDIEMSZFZHALIGKEMJJFPCIWKRMLMPINAYOFIREAOLDTHITDVRMSE'

# 字母映射到它的前一个字母（恢复第一个切片后的字符串到维吉尼亚的加密形式）
letter_back = {letter: string.ascii_uppercase[idx - 1] for idx, letter in enumerate(string.ascii_uppercase)}


# 计算重合指数ic
def get_ic(text):
    n = len(text)
    frequency = Counter(text)  # 统计每一个大写字母的频率
    up = sum(f * (f - 1) for f in frequency.values())  # 分子
    down = n * (n - 1)  # 分母
    ic = up / down
    return ic


# 把密文恢复成标准维吉尼亚的形式
def text_recover(text, l):
    # 转为列表，便于替换
    after = list(text)

    # 对从 l 到 len(text) 的字符进行不同次数的映射
    for i in range(l, len(text), l):
        # 映射的次数是 i // l（即从第 l 到 2l 映射一次，从 2l 到 3l 映射两次，以此类推）
        times = i // l
        # 将 i 到 i + l 的字符段逐个字符进行 times 次映射
        for j in range(i, min(i + l, len(text))):
            after[j] = times_back(after[j], times)

    # 将字符列表转换回字符串
    return ''.join(after)


# 多次映射函数
def times_back(char, times):
    # 根据字典映射 char 字符，并且根据 times 进行多次映射
    for _ in range(times):
        char = letter_back[char]
    return char


# 找最可能的密文长度，逐个尝试长度l
def length_find(text, max_len):
    ic_avg = []
    for l in range(1, max_len + 1):
        # 按当前长度恢复密文
        recover = text_recover(text, l)
        ics = 0
        for i in range(l):
            part_text = recover[i::l]  # 提取一个子序列
            ic = get_ic(part_text)  # 计算这个子序列的ic
            ics += ic  # 记录ic之和

        ic_avg.append(ics / l)  # 记录l个子串的平均ic值

    print(ic_avg)
    # 找到ic_avg列表中比0.065大最多的下标，加1，即为最可能的密钥长度
    length = 1 + max(enumerate(ic_avg), key=lambda x: (x[1] - 0.065))[0]
    return length


key_length = length_find(s, 10)
print("最可能的密钥长度：", key_length)  # 5

text_recovered = text_recover(s, key_length)
print("修复后的密文：\n" + text_recovered)
# IYMYSHKNMQDLAOVNGBAPDXQEXXEPUWIFZKSLVATMHWIYIAVAEXDNPMXWVLUHIYIZXDNPMXWVAMMSRVPXDKPQWTEAMXXFVMPLRGURLYQOLWVAMMSZBMRSKPUWLRAYSHKXQVUVKFPNZVOLPIIOXTINAVWVZNIGKWEFDIVQCPILXINNIETTIPMTHKPQQDJBQRVROURVRZFMRLTMXTRVPXTTPZMRFTAVTUXQVHFVMPXKGURIYMNYHZVQWH
