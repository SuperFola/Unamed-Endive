#if Unamed.getCurrentViewId() == 1 and not Unamed.hasActiveHud(1):
#    Unamed.displayWithView("bulle", 1, 1)
ev = Unamed.getEvent()
if ev.get("type", "none") == "keyreleased" and ev.get("key", -1) == keys_ev_code.index('P'):
    print("py> P")
    print(Unamed.screenshot())