import MeCab

class Text(object):
    def __init__(self, path="data/wiki.txt"):
        self.wakati = MeCab.Tagger('-Owakati')
        self.chasen = MeCab.Tagger()
        with open(path, "r") as f:
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
                f.write(t)

    def _morpho_analysis(self):
        return

t = Text("./data/neko.txt")
# print(t.text[0].strip("\n"))
# print(t.text)
ret = t._wakati(path="./data/neko-wakati.txt")
# print(ret)
t._morpho_analysis()