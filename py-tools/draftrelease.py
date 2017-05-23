# drafting-github-release script for Unamed-Endive
# copyright Loodoor (alias Folaefolc sometime)
# to draft a new release on the official repository of the project
import os
import time
import github3
import tkcommentsrelease


def read_comments():
    comments = ""
    if os.path.exists("comments.txt"):
        with open("comments.txt") as file:
            comments = file.read()
    return comments


def read_credentials():
    with open("credentials.json", "r") as credentials:
        data = eval(credentials.read())
    return data


def main():
    os.chdir("F:/ENDIVE")
    
    print("Connecting to GitHub")
    me = github3.login(**read_credentials())
    print("Getting the repository")
    official_repository = me.repository("Loodoor", "Unamed-Endive")
    
    print("Reading the versions to upload")
    if os.path.exists("releases-to-upload"):
        with open("releases-to-upload.txt") as file:
            versions = file.readlines()
    else:
        versions = []
    
    ok = True
    print("Processing upload(s)")
    for ver in versions:
        version = ver.strip("\n")
        print("Drafting a new release :", version, flush=True)
        tkcommentsrelease.main(version)
        try:
            release = official_repository.create_release(version.split(" ")[0], "master", name=time.strftime("%d/%M/%Y"), body=read_comments())
            release.upload_asset("binaryfile", "installer.exe", open("../ENDIVE-protos/" + version + "/Unamed Rebirth - " + version + ".exe", "rb"))
        except github3.GitHubError as e:
            print(e.errors)
            ok = False
    
    if ok:
        with open("releases-to-upload.txt", "w") as file:
            file.write("")

if __name__ == '__main__':
    main()