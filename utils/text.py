import argparse
import MeCab

class Text(object):
    def __init__(self, path="data/wiki.txt"):
        self.wakati = MeCab.Tagger('-Owakati')
        self.chasen = MeCab.Tagger('-Ochasen')
        with open(path, "r", encoding="utf-8") as f:
            self.text = f.readlines()
        for i in range(len(self.text)):
            self.text[i] = self.text[i].replace("\n", "")

    def _wakati(self, path):
        wakatied = []
        for i in range(len(self.text)):
            new_t = self.wakati.parse(self.text[i]).strip("\n")
            wakatied.append(new_t)
        
        with open(path, "w") as f:
            for t in wakatied:
                f.write(t+'\n')
        return None

    def _pos(self, path):
        self.chasen.parse("")
        with open(path, "w") as f:
            for i in range(len(self.text)):
                node = self.chasen.parseToNode(self.text[i])
                while node:
                    sf = node.surface
                    pos = node.feature.split(",")[0]
                    if sf != '':
                        f.write(sf+'/'+pos+' ')
                    node = node.next
        return None
    
    def _count_num_of_pos(self, return_pos_list=False):
        self.chasen.parse("")
        s = set()
        for i in range(len(self.text)):
            node = self.chasen.parseToNode(self.text[i])
            while node:
                pos = node.feature.split(",")[0]
                s.add(pos)
                node = node.next
        if return_pos_list:
            return len(s), list(s)
        return len(s)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='this script for text processing.')
    parser.add_argument('--tar_path', help='text file path you want to process')
    parser.add_argument('--wakati_save_path', help='path you wanto to save processed text')
    parser.add_argument('--pos_save_path', help='path you want to save processed and pos tagged text')
    args = parser.parse_args()
    t = Text(args.tar_path)
    t._wakati(args.wakati_save_path)
    t._pos(args.pos_save_path)
    num_of_pos, pos_list = t._count_num_of_pos(return_pos_list=True)
    # print("num of pos:", num_of_pos)
    # print("emerged part of speech:")
    # print(" ".join(pos_list))