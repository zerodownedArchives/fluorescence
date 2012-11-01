
from data import *

def themeTest():
    print "I am the theme!"

def setFont(obj):
    obj.setFont("Devinne Swash", 16)

def addPythonButton(gump, coordinates, callback):
    b = gump.addPythonButton(coordinates, Texture(TextureSource.THEME, "images/button.png"), callback)
    b.state("mouseOver").rgba = rgba("#cceecc")
    b.state("mouseDown").rgba = rgba("#ccffcc")
    b.setFont("Devinne Swash", 18)
    return b
