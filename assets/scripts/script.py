#if Unamed.getCurrentViewId() == 1 and not Unamed.hasActiveHud(1):
#    Unamed.displayWithView("bulle", 1, 1)
if Unamed.getEvent().get("key") == keys_ev_code.index('M'):
    print("py> M")
    print(Unamed.screenshot())