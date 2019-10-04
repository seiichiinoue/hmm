import re
import argparse
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

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

def visualize(m):
    cols = []
    for line in m.values():
        cols += line.keys()
    cols = list(set(cols))
    # print(cols)
    cols_dict = {}
    for i, c in enumerate(cols):
        cols_dict[c] = i
    rows = sorted(list(map(int, m.keys())))
    # print(sorted(list(map(int, rows))))
    data = [[0 for _ in range(len(rows))] for _ in range(len(cols))]
    for tag, dic in m.items():
        for pos, num in dic.items():
            data[cols_dict[pos]][int(tag)] = int(num)
    data = np.array(data)
    df = pd.DataFrame(data, index=cols, columns=rows)
    plt.figure(figsize=(30, 20))
    sns.heatmap(df, annot=True)
    # plt.show()
            
# RUN: python valuation.py --ref ../data/neko-pos.txt --test ../data/neko-result.txt
if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='this script for valuation.')
    parser.add_argument('--ref', help='reference file path')
    parser.add_argument('--test', help='test file path')
    args = parser.parse_args()
    m = mapping(args.ref, args.test)
    valuation(m)