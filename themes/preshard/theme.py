
from data import *

def themeTest():
    print "I am the preshard theme!"

def setFont(obj):
    obj.setFont("Devinne Swash", 16)

def addPythonButton(gump, coordinates, callback):
    b = gump.addPythonButton(coordinates, Texture(TextureSource.THEME, "images/button.png"), callback)
    styleButton(b)
    return b

def addPageButton(gump, coordinates, page):
    b = gump.addPageButton(coordinates, Texture(TextureSource.THEME, "images/button.png"), page)
    styleButton(b)
    return b

def addServerButton(gump, coordinates, buttonId):
    b = gump.addPageButton(coordinates, Texture(TextureSource.THEME, "images/button.png"), buttonId)
    styleButton(b)
    return b

def styleButton(b):
    b.state("mouseOver").rgba = rgba("#cceecc")
    b.state("mouseDown").rgba = rgba("#ccffcc")
    b.setFont("Devinne Swash", 18)
