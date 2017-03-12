# drafting-github-release script for Unamed-Endive
# copyright Loodoor (alias Folaefolc sometime)
# to draft a new release on the official repository of the project
import os
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
    with open("releases-to-upload.txt") as file:
        versions = file.readlines()
    
    print("Processing upload(s)")
    for ver in versions:
        version = ver.strip("\n")
        print("Drafting a new release :", version, flush=True)
        tkcommentsrelease.main(version)
        release = official_repository.create_release(version, name="New update ! Version " + version, body=read_comments(), prerelease=True)
        release.upload_asset("binaryfile", "installer.exe", open("../ENDIVE-protos/" + version + "/Unamed Rebirth - " + version + ".exe", "rb"))
    
    os.remove("releases-to-upload.txt")  # we have upload everything, no need to keep the file (which will be regenerated automatically when needed)