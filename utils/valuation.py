import re
import argparse

def mapping(ref_path, test_path):
    with open(ref_path, "r") as f:
        ref = f.readlines()
    with open(test_path, "r") as f:
        test = f.readlines()
    m = {}
    for r, t in zip(ref, test):
        r, t = r.strip(), t.strip()
        r, t = re.sub(r"[^ ]*\/", "", r), re.sub(r"[^ ]*\/", "", t)
        r, t = r.split(" "), t.split(" ")
        for i in range(len(r)):
            if t[i] not in m.keys():
                m[t[i]] = {}
            if r[i] not in m[t[i]].keys():
                m[t[i]][r[i]] = 0
            m[t[i]][r[i]] += 1
    return m

def valuation(m):
    total, correct = 0, 0
    for num, pos_list in m.items():
        sorted_pos_list = sorted(pos_list.items(), reverse=True ,key=lambda x:x[1])
        print("{} -> {}".format(num, sorted_pos_list[0][0]))
        print(" ".join(["=".join(list(map(str, sorted_pos_list[i]))) for i in range(len(sorted_pos_list))]))
        correct += sorted_pos_list[0][1]
        total += sum(pos_list.values())
    acc = correct/total
    print("accuracy:", acc)
    return acc

# RUN: python valuation.py --ref ../data/neko-pos.txt --test ../data/neko-result.txt
if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='this script for valuation.')
    parser.add_argument('--ref', help='reference file path')
    parser.add_argument('--test', help='test file path')
    args = parser.parse_args()
    m = mapping(args.ref, args.test)
    valuation(m)