# interface to add/edit easily the in-game object
# copyright Loodoor (alias Folaefolc sometime)
# idea from https://psdk.communityscriptproject.com/index.php/Objets_(BDD)
# copyright PSDK, NuriYuri
import os
import tkinter as tk
import tkinter.ttk as ttk


def load_objlb(objlb, vars):
    objlb.delete(0, tk.END)
    for o in vars["objects"]["objects"]:
        objlb.insert(tk.END, o["name"])


def load_objects():
    objects = {}
    with open("assets/inventory/objects.json") as file:
        objects = eval(file.read().replace('false', 'False').replace('true', 'True').replace('null', 'None'))
    return objects


def setv(vars, k, v):
    vars[k] = v


def affectmod(vars, types, lbmod, txmod):
    o = vars["objects"]["objects"][vars["objectselected"]]
    lbmod.set(types[o["type"]])
    vars["modid"].set(o["id"])
    txmod.delete(1.0, tk.END)
    txmod.insert(1.0, o["desc"])
    vars["modnom"].set(o["name"])
    vars["modprice"].set(o["price"])
    vars["moduse"].set(int(o["limited_use"]))
    vars["moddrop"].set(int(o["throwaway"]))
    vars["modval"].set(o["value"])


def clear(vars, types, kind):
    vars[kind + "id"].set(0)
    vars[kind + "nom"].set("")
    vars[kind + "price"].set(0)
    vars[kind + "use"].set(0)
    vars[kind + "drop"].set(0)
    vars[kind + "val"].set(0)
    vars["type" + kind + "sel"] = types[0]


def save(vars, types, txmod):
    vars["objects"]["objects"].append({
        "id": int(vars["newid"].get()),
        "name": vars["newnom"].get(),
        "desc": vars["newdesc"].get(),
        "price": int(vars["newprice"].get()),
        "limited_use": bool(vars["newuse"].get()),
        "throwaway": bool(vars["newdrop"].get()),
        "value": int(vars["newval"].get()),
        "type": int(types.index(vars["typenewesel"]))
    })
    with open("assets/inventory/objects.json", "w") as file:
        file.write(
            str(vars["objects"]).replace("True", "true").replace("False", "false").replace("None", "null")
        )
    clear(vars, types, "new")


def modify(vars, types):
    o = vars["objects"]["objects"][vars["objectselected"]]
    o["id"] = int(vars["modid"].get())
    o["name"] = vars["modnom"].get()
    o["desc"] = vars["moddesc"].get()
    o["price"] = int(vars["modprice"].get())
    o["limited_use"] = bool(vars["moduse"].get())
    o["throwaway"] = bool(vars["moddrop"].get())
    o["value"] = int(vars["modval"].get())
    o["type"] = int(types.index(vars["typemodsel"]))
    with open("assets/inventory/objects.json", "w") as file:
        file.write(
            str(vars["objects"]).replace("True", "true").replace("False", "false").replace("None", "null")
        )
    clear(vars, types, "mod")


def delete(vars):
    if vars.get("objectselected", False) is not False:
        del vars["objects"]["objects"][vars["objectselected"]]
        with open("assets/inventory/objects.json", "w") as file:
            file.write(
                str(vars["objects"]).replace("True", "true").replace("False", "false").replace("None", "null")
            )


def main():
    os.chdir("F:/ENDIVE")
    
    win = tk.Tk()
    win.title('ObjEdit')
    win.geometry('720x540+10+10')
    
    vars = {}
    
    
    ################################################################
    # Frame to select which object to edit
    ################################################################
    menu = tk.Menu(win)
    win.config(menu=menu)
    
    fmenu = tk.Menu(menu)
    menu.add_cascade(label="Fichier", menu=fmenu)
    fmenu.add_command(label="Ouvrir", command=lambda: None)
    ################################################################
    # end
    ################################################################
    
    
    ################################################################
    # Frame to select which object to edit
    ################################################################
    frame = tk.Frame(win, borderwidth=2, relief=tk.GROOVE, bg="blue")
    
    sframe = tk.Frame(frame)
    tk.Label(sframe, text="Objets :").pack(side=tk.LEFT, padx=20)
    refresh_btn = tk.Button(sframe, text="Rafraichir", command=lambda: (
            setv(vars, "objects", load_objects())
            , load_objlb(objects_lb, vars)
        )
    )
    refresh_btn.pack(side=tk.RIGHT)
    delete_btn = tk.Button(sframe, text="Supprimer", command=lambda: (
            delete(vars)
            , setv(vars, "objects", load_objects())
            , load_objlb(objects_lb, vars)
        )
    )
    delete_btn.pack(side=tk.RIGHT)
    sframe.pack(side=tk.TOP)
    
    objects_lb = tk.Listbox(frame)
    setv(vars, "objects", load_objects())
    load_objlb(objects_lb, vars)
    objects_lb.pack(side=tk.TOP, fill="both", expand=True)
    
    frame.pack(side=tk.LEFT, fill="both", expand=True)
    ################################################################
    # end
    ################################################################
    
    
    types = (
        'player',
        'healpv',
        'healpp',
        'healstatus',
        'capture',
        'levelup',
        'lowercooldown',
        'none'
    )
    
    
    ################################################################
    # Frame to modify the selected object
    ################################################################
    frame_mod = tk.Frame(win, borderwidth=2, relief=tk.GROOVE, bg="red")
    
    tk.Label(frame_mod, text="Modification").pack(side=tk.TOP)
    
    sframe_mod = tk.Frame(frame_mod)
    tk.Label(sframe_mod, text="Nom :").pack(side=tk.LEFT)
    setv(vars, "modnom", tk.StringVar())
    tk.Entry(sframe_mod, textvariable=vars["modnom"]).pack(side=tk.RIGHT)
    sframe_mod.pack(side=tk.TOP, pady=10)
    
    sframe_mod2 = tk.Frame(frame_mod)
    tk.Label(sframe_mod2, text="ID :").pack(side=tk.LEFT)
    setv(vars, "modid", tk.StringVar())
    vars["modid"].set(0)
    tk.Spinbox(sframe_mod2, from_=0, to=999, increment=1, textvariable=vars["modid"], width=5).pack(side=tk.RIGHT)
    sframe_mod2.pack(side=tk.TOP, pady=10)
    
    sframe_mod3 = tk.Frame(frame_mod)
    tk.Label(sframe_mod3, text="Description :").pack(side=tk.TOP)
    text_mod = tk.Text(sframe_mod3, width=24, height=4, wrap=tk.WORD)
    text_mod.pack(side=tk.LEFT)
    scroll_txmod = tk.Scrollbar(sframe_mod3, command=text_mod.yview)
    scroll_txmod.pack(side=tk.RIGHT, anchor='ne', fill=tk.BOTH)
    text_mod['yscrollcommand'] = scroll_txmod.set
    sframe_mod3.pack(side=tk.TOP, pady=10)
    
    sframe_mod4 = tk.Frame(frame_mod)
    tk.Label(sframe_mod4, text="Prix :").pack(side=tk.LEFT)
    setv(vars, "modprice", tk.StringVar())
    vars["modprice"].set(0)
    tk.Spinbox(sframe_mod4, from_=0, to=999, increment=1, textvariable=vars["modprice"], width=5).pack(side=tk.RIGHT)
    sframe_mod4.pack(side=tk.TOP, pady=10)
    
    sframe_mod5 = tk.Frame(frame_mod)
    tk.Label(sframe_mod5, text="Usage limité :").pack(side=tk.LEFT)
    setv(vars, "moduse", tk.StringVar())
    vars["moduse"].set(0)
    tk.Spinbox(sframe_mod5, from_=0, to=1, increment=1, textvariable=vars["moduse"], width=5).pack(side=tk.RIGHT)
    sframe_mod5.pack(side=tk.TOP, pady=10)
    
    sframe_mod6 = tk.Frame(frame_mod)
    tk.Label(sframe_mod6, text="Jetable :").pack(side=tk.LEFT)
    setv(vars, "moddrop", tk.StringVar())
    vars["moddrop"].set(0)
    tk.Spinbox(sframe_mod6, from_=0, to=1, increment=1, textvariable=vars["moddrop"], width=5).pack(side=tk.RIGHT)
    sframe_mod6.pack(side=tk.TOP, pady=10)
    
    sframe_mod7 = tk.Frame(frame_mod)
    tk.Label(sframe_mod7, text="Type :").pack(side=tk.LEFT)
    list_type_mod = ttk.Combobox(sframe_mod7, values=types, state='readonly')
    list_type_mod.pack()
    list_type_mod.set(types[0])
    list_type_mod.bind('<<ComboboxSelected>>', lambda _: setv(vars, "typemodesel", list_type_mod.get()))
    sframe_mod7.pack(side=tk.TOP, pady=10)
    
    sframe_mod8 = tk.Frame(frame_mod)
    tk.Label(sframe_mod8, text="Valeur :").pack(side=tk.LEFT)
    setv(vars, "modval", tk.StringVar())
    vars["modval"].set(0)
    tk.Spinbox(sframe_mod8, from_=-999, to=999, increment=1, textvariable=vars["modval"], width=5).pack(side=tk.RIGHT)
    sframe_mod8.pack(side=tk.TOP, pady=10)
    
    savemod_btn = tk.Button(frame_mod, text="Enregistrer", command=lambda: (
            setv(vars, "modnom", text_mod.get(1.0, tk.END))
            , modify(vars, types)
            , setv(vars, "objects", load_objects())
            , load_objlb(objects_lb, vars)
        )
    )
    savemod_btn.pack(side=tk.BOTTOM)
    
    frame_mod.pack(side=tk.LEFT, fill="both", expand=True)
    ################################################################
    # end
    ################################################################
    
    
    ################################################################
    # Frame to create a new object
    ################################################################
    frame_new = tk.Frame(win, borderwidth=2, relief=tk.GROOVE, bg="green")
    
    tk.Label(frame_new, text="Ajout").pack(side=tk.TOP)
    
    sframe_new = tk.Frame(frame_new)
    tk.Label(sframe_new, text="Nom :").pack(side=tk.LEFT)
    setv(vars, "newnom", tk.StringVar())
    tk.Entry(sframe_new, textvariable=vars["newnom"]).pack(side=tk.RIGHT)
    sframe_new.pack(side=tk.TOP, pady=10)
    
    sframe_new2 = tk.Frame(frame_new)
    tk.Label(sframe_new2, text="ID :").pack(side=tk.LEFT)
    setv(vars, "newid", tk.StringVar())
    vars["newid"].set(0)
    tk.Spinbox(sframe_new2, from_=0, to=999, increment=1, textvariable=vars["newid"], width=5).pack(side=tk.RIGHT)
    sframe_new2.pack(side=tk.TOP, pady=10)
    
    sframe_new3 = tk.Frame(frame_new)
    tk.Label(sframe_new3, text="Description :").pack(side=tk.TOP)
    text_new = tk.Text(sframe_new3, width=24, height=4, wrap=tk.WORD)
    text_new.pack(side=tk.LEFT)
    scroll_txnew = tk.Scrollbar(sframe_new3, command=text_new.yview)
    scroll_txnew.pack(side=tk.RIGHT, anchor='ne', fill=tk.BOTH)
    text_new['yscrollcommand'] = scroll_txnew.set
    sframe_new3.pack(side=tk.TOP, pady=10)
    
    sframe_new4 = tk.Frame(frame_new)
    tk.Label(sframe_new4, text="Prix :").pack(side=tk.LEFT)
    setv(vars, "newprice", tk.StringVar())
    vars["newprice"].set(0)
    tk.Spinbox(sframe_new4, from_=0, to=999, increment=1, textvariable=vars["newprice"], width=5).pack(side=tk.RIGHT)
    sframe_new4.pack(side=tk.TOP, pady=10)
    
    sframe_new5 = tk.Frame(frame_new)
    tk.Label(sframe_new5, text="Usage limité :").pack(side=tk.LEFT)
    setv(vars, "newuse", tk.StringVar())
    vars["newuse"].set(0)
    tk.Spinbox(sframe_new5, from_=0, to=1, increment=1, textvariable=vars["newuse"], width=5).pack(side=tk.RIGHT)
    sframe_new5.pack(side=tk.TOP, pady=10)
    
    sframe_new6 = tk.Frame(frame_new)
    tk.Label(sframe_new6, text="Jetable :").pack(side=tk.LEFT)
    setv(vars, "newdrop", tk.StringVar())
    vars["newdrop"].set(0)
    tk.Spinbox(sframe_new6, from_=0, to=1, increment=1, textvariable=vars["newdrop"], width=5).pack(side=tk.RIGHT)
    sframe_new6.pack(side=tk.TOP, pady=10)
    
    sframe_new7 = tk.Frame(frame_new)
    tk.Label(sframe_new7, text="Type :").pack(side=tk.LEFT)
    list_type_new = ttk.Combobox(sframe_new7, values=types, state='readonly')
    list_type_new.pack()
    list_type_new.set(types[0])
    list_type_new.bind('<<ComboboxSelected>>', lambda _: setv(vars, "typenewesel", list_type_new.get()))
    sframe_new7.pack(side=tk.TOP, pady=10)
    
    sframe_new8 = tk.Frame(frame_new)
    tk.Label(sframe_new8, text="Valeur :").pack(side=tk.LEFT)
    setv(vars, "newval", tk.StringVar())
    vars["newval"].set(0)
    tk.Spinbox(sframe_new8, from_=-999, to=999, increment=1, textvariable=vars["newval"], width=5).pack(side=tk.RIGHT)
    sframe_new8.pack(side=tk.TOP, pady=10)
    
    savenew_btn = tk.Button(frame_new, text="Enregistrer", command=lambda: (
            setv(vars, "newnom", text_new.get(1.0, tk.END))
            , save(vars, types)
            , setv(vars, "objects", load_objects())
            , load_objlb(objects_lb, vars)
        )
    )
    savenew_btn.pack(side=tk.BOTTOM)
    
    frame_new.pack(side=tk.LEFT, fill="both", expand=True)
    ################################################################
    # end
    ################################################################
    
    objects_lb.bind('<<ListboxSelect>>', lambda _: (
            setv(vars, "objectselected", int(objects_lb.curselection()[0]))
            , affectmod(vars, types, list_type_mod, text_mod)
        )
    )
    
    win.mainloop()


if __name__ == '__main__':
    main()