# script to get statistics about the creatures
# copyright Loodoor (alias Folaefolc sometime)
import os


def main():
    os.chdir("F:/ENDIVE/assets/config/")
    
    keys = 'type', 'stade'
    stats = {k: {} for k in keys}
    stats['correct path'] = 0
    
    with open("dex.json") as dex:
        formatted = eval(dex.read())
    
    for _, v in formatted.items():
        for k in keys:
            if v[k] not in stats[k].keys():
                stats[k][v[k]] = 1
            else:
                stats[k][v[k]] += 1
        if os.path.exists("../creatures/" + v['file']):
            stats['correct path'] += 1
    
    tot = sum(stats['type'].values())
    
    for k, v in stats.items():
        if isinstance(v, dict):
            print(k, "\t*" + "\n\t*".join("{k:3}: {v:3} - {p:6.2f}%".format(k=_k, v=_v, p=_v / tot * 100) for _k, _v in v.items()), sep='\n')
        else:
            print(k + ":", "{v:3} - {p:6.2f}%".format(v=v, p=v / tot * 100))
    print("Count", "\t " + str(tot), sep="\n")


if __name__ == '__main__':
    main()
    # input("\nType RETURN to exit ...")