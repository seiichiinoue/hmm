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
                f.write(t)
        return None

    def _morpho_analysis(self, path):
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
