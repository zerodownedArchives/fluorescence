
from ui import *
from data import *
import world

def create(args):
    g = GumpMenu("status", 100, 100)
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
        l.rgba = rgba("#444444")

        # TODO: rest of the gump...
    else:
        # TODO: proper uo gump
        g.addBackground((0, 0, 250, 52), 3000)
        g.addPropertyLabel((5, 5, 250, 20), "name")
        g.addPropertyLabel((5, 22, 250, 20), "hitpoints")
