# asking for comments about the update script for Unamed-Endive
# copyright Loodoor (alias Folaefolc sometime)
# to ask for comments (text) about the release (using Tk)
import os
import tkinter as tk


def get_default_model():
    with open("release-comments-model.txt") as file:
        model = file.read()
    return model


def save(text):
    with open("comments.txt", "w") as file:
        file.write(text)


def main(version=""):
    os.chdir("F:/ENDIVE")
    
    win = tk.Tk()
    win.title('Comments ' + version)
    win.geometry('720x540+10+10')
    
    text_mod = tk.Text(win, width=70, height=32, wrap=tk.WORD)
    text_mod.pack(side=tk.LEFT)
    scroll_txmod = tk.Scrollbar(win, command=text_mod.yview)
    scroll_txmod.pack(side=tk.LEFT, anchor='ne', fill=tk.BOTH)
    text_mod['yscrollcommand'] = scroll_txmod.set
    text_mod.insert(1.0, get_default_model())
    
    savebtn = tk.Button(win, text="Sauvegarder", command=lambda: (save(text_mod.get(1.0, tk.END)), win.destroy()))
    savebtn.pack(side=tk.BOTTOM)
    
    win.mainloop()


if __name__ == '__main__':
    main()