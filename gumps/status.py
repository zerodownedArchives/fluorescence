
from ui import *
from data import *
import world

def create(args):
    g = GumpMenu("status", args.get("x", 100), args.get("y", 100))
    g.mobile = args["mobile"]

    if g.mobile == world.getPlayer():
        g.addImage((0, 0), Texture(TextureSource.GUMPART, 10860))
        l = g.addPropertyLabel((83, 45, 295, 20), "name")
        l.halign = HAlign.CENTER

        l = g.addPropertyLabel((83, 73, 35, 25), "strength")
        l.halign = HAlign.CENTER
        l = g.addPropertyLabel((83, 102, 35, 25), "dexterity")
        l.halign = HAlign.CENTER
        l = g.addPropertyLabel((83, 129, 35, 25), "intelligence")
        l.halign = HAlign.CENTER

        l = g.addPropertyLabel((145, 69, 35, 20), "hitpoints")
        l.halign = HAlign.CENTER
        l = g.addPropertyLabel((145, 81, 35, 20), "hitpoints-max")
        l.halign = HAlign.CENTER
        l = g.addPropertyLabel((145, 94, 35, 20), "stamina")
        l.halign = HAlign.CENTER
        l = g.addPropertyLabel((145, 107, 35, 20), "stamina-max")
        l.halign = HAlign.CENTER
        l = g.addPropertyLabel((145, 122, 35, 20), "mana")
        l.halign = HAlign.CENTER
        l = g.addPropertyLabel((145, 135, 35, 20), "mana-max")
        l.halign = HAlign.CENTER

        l = g.addPropertyLabel((219, 74, 35, 20), "statcap")
        l.halign = HAlign.CENTER
        l = g.addPropertyLabel((219, 102, 35, 20), "luck")
        l.halign = HAlign.CENTER
        l = g.addPropertyLabel((219, 130, 35, 20), "weight")
        l.halign = HAlign.CENTER

        l = g.addPropertyLabel((281, 74, 35, 20), "damage-min")
        l.halign = HAlign.CENTER
        # TODO: damage max
        l = g.addPropertyLabel((281, 102, 35, 20), "gold")
        l.halign = HAlign.CENTER
        l = g.addPropertyLabel((281, 130, 35, 20), "followers")
        l.halign = HAlign.CENTER
        # TODO: followers max

        l = g.addPropertyLabel((352, 69, 35, 20), "resist-physical")
        l.halign = HAlign.CENTER
        l = g.addPropertyLabel((352, 85, 35, 20), "resist-fire")
        l.halign = HAlign.CENTER
        l = g.addPropertyLabel((352, 100, 35, 20), "resist-cold")
        l.halign = HAlign.CENTER
        l = g.addPropertyLabel((352, 116, 35, 20), "resist-poison")
        l.halign = HAlign.CENTER
        l = g.addPropertyLabel((352, 132, 35, 20), "resist-energy")
        l.halign = HAlign.CENTER
        l.rgba = rgba("#444444")

        #TODO: SE/SA Addons (Buttons for StatsUp/Down, BuffList-Button)
    else:
        # TODO: proper uo gump
        g.addBackground((0, 0, 250, 52), 3000)
        g.addPropertyLabel((5, 5, 250, 20), "name")
        g.addPropertyLabel((5, 22, 250, 20), "hitpoints")

    return g

def save(gump):
    saveData = {
        "x": gump.x,
        "y": gump.y,
        # To be able to also restore the gump after a restart, save only the serial
        "serial": gump.mobile.serial
    }
    return saveData

def load(saveData):
    mob = world.getMobile(saveData["serial"])
    if mob:
        saveData["mobile"] = mob
        gump = create(saveData)
        # request status data from server
        saveData["mobile"].openStatus()
