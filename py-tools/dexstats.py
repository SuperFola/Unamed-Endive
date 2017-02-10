# script to get statistics about the creatures
# copyright Loodoor (alias Folaefolc sometime)
import os


def main():
    os.chdir("F:/ENDIVE/assets/config/")
    
    keys = 'type', 'stade'
    stats = {k: {} for k in keys}
    
    with open("dex.json") as dex:
        formatted = eval(dex.read())
    
    for _, v in formatted.items():
        for k in keys:
            if v[k] not in stats[k].keys():
                stats[k][v[k]] = 1
            else:
                stats[k][v[k]] += 1
    
    for k, v in stats.items():
        print(k, "\t* " + "\n\t* ".join(str(_k) + ": " + str(_v) for _k, _v in v.items()), sep='\n')


if __name__ == '__main__':
    main()
    # input("\nType RETURN to exit ...")