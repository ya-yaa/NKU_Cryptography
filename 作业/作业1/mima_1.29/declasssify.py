import string

# 字母映射到数字
letter_to_num = {letter: idx for idx, letter in enumerate(string.ascii_uppercase)}

# 数字映射到字母
num_to_letter = {idx: letter for idx, letter in enumerate(string.ascii_uppercase)}

# 密文字
key = list("PRIME")
l = len(key)

key_num = [letter_to_num[element] for element in key]

# 已经变成普通维吉尼亚加密的密文
s = 'IYMYSHKNMQDLAOVNGBAPDXQEXXEPUWIFZKSLVATMHWIYIAVAEXDNPMXWVLUHIYIZXDNPMXWVAMMSRVPXDKPQWTEAMXXFVMPLRGURLYQOLWVAMMSZBMRSKPUWLRAYSHKXQVUVKFPNZVOLPIIOXTINAVWVZNIGKWEFDIVQCPILXINNIETTIPMTHKPQQDJBQRVROURVRZFMRLTMXTRVPXTTPZMRFTAVTUXQVHFVMPXKGURIYMNYHZVQWH'

answer = []

for i in range(0, len(s), l):
    for j in range(i, min(i + l, len(s))):
        text_num = (letter_to_num[s[j]] - key_num[j % l]) % 26
        text = num_to_letter[text_num]
        answer.append(text)

answer = ''.join(answer)
print(answer)
# THEMOSTFAMOUSCRYPTOLOGISTINHISTORYOWESHISFAMELESSTOWHATHEDIDTHANTOWHATHESAIDANDTOTHESENSATIONALWAYINWHICHHESAIDITANDTHISWASMOSTPERFECTLYINCHARACTERFORHERBERTOSBORNEYARDLEYWASPERHAPSTHEMOSTENGAGINGARTICULATEANDTECHNICOLOREDPERSONALITYINTHEBUSINESS
# 整理成可读的：The most famous cryptologist in history owes his fame less to what he did than to what he said, and to the sensational way in which he said it. And this was most perfectly in character, for Herbert Osborne Yardley was perhaps the most engaging, articulate, and technicolored personality in the business.