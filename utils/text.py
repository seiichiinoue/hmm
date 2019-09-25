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

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='this script for text processing.')
    parser.add_argument('--tar_path', help='text file path you want to process')
    parser.add_argument('--wakati_save_path', help='path you wanto to save processed text')
    parser.add_argument('--pos_save_path', help='path you want to save processed and pos tagged text')
    args = parser.parse_args()
    t = Text(args.tar_path)             # "../data/neko.txt"
    t._wakati(args.wakati_save_path)    # "../data/neko-wakati.txt"
    t._pos(args.pos_save_path)          # "../data/neko-pos.txt"