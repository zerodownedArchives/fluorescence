
from ui import *
from data import *
import client
import theme

def create(args):
    g = GumpMenu("login", 387, 320)
    g.setFont("Devinne Swash", 16)
    g.onEnter = onGumpEnter
    g.onEscape = onGumpEscape

    g.addImage((0, 0), Texture(TextureSource.THEME, "images/background_250x125.png"))

    label = g.addLabel((10, 15, 230, 80), args["message"])
    label.halign = HAlign.CENTER

    btnClose = theme.addPythonButton(g, (60, 90, 130, 25), onClick)
    btnClose.text = "OK"

def onClick(button):
    # returning true closes the gump
    return True

def onGumpEnter(gump):
    gump.close()

def onGumpEscape(gump):
    gump.close()
