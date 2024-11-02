import random
import SPN

n = 8000

# 生成随机的明文字符串并写入文件xs.txt
def gen_xs(n):
    with open("xs.txt", "w") as f:
        for _ in range(n):
            binary_string = ''.join(random.choice('01') for _ in range(16))
            f.write(binary_string + '\n')

gen_xs(n)


# 读取 xs.txt 文件，SPN加密每行内容并写入 ys.txt
with open('xs.txt', 'r', encoding='utf-8') as infile, open('ys2.txt', 'w', encoding='utf-8') as outfile:
    for line in infile:
        # 调用处理函数处理每行数据
        # 00111010100101001101011000111111
        # 01000111000010101110010110001000
        # 11001010000011001101010100100011
        y = SPN.SPN(line, "11001010000011001101010100100011")
        # 将处理后的结果写入 ys.txt
        outfile.write(y + '\n')

print("文件处理完成，结果已写入 ys.txt")


