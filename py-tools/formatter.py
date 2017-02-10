# script to format the attack file in a correct format
# copyright Loodoor (alias Folaefolc sometime)
import sys


def command_line():
    argv = sys.argv[1:]
    filename = "../assets/config/attacks.umd"
    if '-h' in argv:
        print("""Source formatter for JSON
        -h display this help
        -p [file] specify path to file
        """)
        sys.exit(1)
    if '-p' in argv:
        filename = argv[argv.index('-p') + 1]
    
    return filename

def main(filename):
    with open(filename) as f:
        content = eval(f.read())
    
    if isinstance(content, dict):
        new = {}
        for k, v in content.items():
            if k == 'a_type':
                new['type'] = v
            elif k == 'types_ok':
                pass
            elif k in ('puissance', 'precision'):
                new[k] = int(v)
            else:
                new[k] = v
    elif isinstance(content, list):
        new = []
        for i in content:
            new.append({})
            for k, v in i.items():
                if k == 'a_type':
                    new[-1]['type'] = v
                elif k == 'types_ok':
                    pass
                elif k in ('puissance', 'precision'):
                    new[-1][k] = int(v)
                else:
                    new[-1][k] = v
    
    with open(filename + ".formatted", "w") as f:
        f.write(str(new))


if __name__ == '__main__':
    f = command_line()
    main(f)