# publishing script for Unamed-Endive
# copyright Loodoor (alias Folaefolc sometime)
# only to create the iss script and build it, defining specials values (such as the game version)
import os
import sys


def read_version():
    v = ''
    with open('version.h') as version_file:
        content = version_file.readlines()
    v += content[16][content[16].index('=') + 2:-2] + '.'  # MAJOR
    v += content[17][content[17].index('=') + 2:-2] + '.'  # MINOR
    v += content[18][content[18].index('=') + 2:-2] + '-'  # BUILD
    v += content[19][content[19].index('=') + 2:-2] + ' '  # REVISION
    v += content[12][content[12].index('=') + 4:-3]        # STATUS
    return v


def convert_version_to_dict(v):
    w = {
        'MAJOR': -1,
        'MINOR': -1,
        'BUILD': -1,
        'REVISION': -1,
        'STATUS': -1
    }
    v = v.replace('.', ' ').replace('-', ' ').split(' ')
    w['MAJOR'] = v[0]
    w['MINOR'] = v[1]
    w['BUILD'] = v[2]
    w['REVISION'] = v[3]
    w['STATUS'] = v[4]
    
    return w


def format_out(r):
    if r:
        print('\n'.join(r.split('\n')[-3:-1]))
    else:
        print("Nothing to be done")


def main():
    os.chdir("F:\\ENDIVE")
    
    if sys.argv[1:]:
        mode = sys.argv[1]
    else:
        mode = 'd'
    
    ver = read_version()
    
    if os.path.exists('F:/ENDIVE-protos/' + ver):
        return ""

    with open('inno_build.iss') as base:
        with open('build.iss', 'w') as new:
            formatted = base.readlines()
            formatted[4] = formatted[4][:-1] + '"{version}"\n'.format(version=ver)
            formatted[5] = formatted[5][:-1] + '"{publisher}"\n'.format(publisher="Kubiangle (c) (r) tm")
            formatted[6] = formatted[6][:-1] + '"{website}"\n'.format(website='https://loodoor.github.io/Unamed-Endive')
            if mode == 'd':
                formatted[52] = ';; ' + formatted[52]
            else:
                formatted[51] = ';; ' + formatted[51]
            
            new.write("".join(formatted))

    r = os.popen('"C:\\Program Files (x86)\\Inno Setup 5\\ISCC.exe" build.iss').read()
    os.remove("build.iss")
    return r


if __name__ == '__main__':
    r = main()
    format_out(r)