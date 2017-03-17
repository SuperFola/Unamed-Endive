# checking for new releases script for Unamed-Endive
# copyright Loodoor (alias Folaefolc sometime)
# to check if we need to start a release or not
import os
import sys
import publisher
import draftrelease


def is_debugging(argv):
    return "d" in argv


def main():
    argv = sys.argv[1:]
    
    os.chdir("F:/ENDIVE")
    
    version = publisher.read_version()
    d_ver = publisher.convert_version_to_dict(version)
    updated = False
    
    if d_ver['BUILD'] == '0' and not is_debugging(argv):
        print("Please wait ~2 minutes while the publishing tool is running", flush=True)
        updated = publisher.main()
    else:
        print("Nothing to do. Skipping", flush=True)
    
    if updated:
        if os.path.exists("releases-to-upload.txt"):
            in_it = False
            with open("releases-to-upload.txt") as test:
                c = test.read()
                in_it = version in c
            if not in_it:
                with open("releases-to-upload.txt", "a") as file:
                    file.write("\n" + version)
        else:
            with open("releases-to-upload.txt", "w") as file:
                file.write(version)
    draftrelease.main()


if __name__ == '__main__':
    main()