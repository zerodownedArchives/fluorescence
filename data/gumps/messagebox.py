
from gumps import *
from data import *
import client

def create():
    g = GumpMenu("login", 387, 320)
    g.setFont("Devinne Swash", 16)
    g.onEnter = onGumpEnter
    g.onEscape = onGumpEscape

    g.addImage((0, 0), Texture(TextureSource.THEME, "images/background_250x125.png"))

    label = g.addLabel((10, 15, 230, 80), "foobert")
    label.halign = HAlign.CENTER

    btnClose = g.addPythonButton((60, 90, 130, 25), Texture(TextureSource.THEME, "images/button.png"), onClick)
    btnClose.state("mouseover").rgba = rgba("#cceecc")
    btnClose.state("mousedown").rgba = rgba("#ccffcc")
    btnClose.text = "OK"

def onClick(button):
    # returning true closes the gump
    return True

def onGumpEnter(gump):
    print "enter pressed"

def onGumpEscape(gump):
    print "escape pressed"
