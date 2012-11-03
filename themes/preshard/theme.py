
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

def addScrollArea(gump, coordinates):
    scroll = gump.addScrollArea(coordinates)
    scroll.vertical.width = 15
    scroll.vertical.increment.texture = Texture(TextureSource.THEME, "images/scroll_increment.png")
    scroll.vertical.incrementMouseOver.rgba = rgba("#cceecc")
    scroll.vertical.incrementMouseDown.rgba = rgba("#ccffcc")
    scroll.vertical.decrement.texture = Texture(TextureSource.THEME, "images/scroll_decrement.png")
    scroll.vertical.decrementMouseOver.rgba = rgba("#cceecc")
    scroll.vertical.decrementMouseDown.rgba = rgba("#ccffcc")
    scroll.vertical.thumb.texture = Texture(TextureSource.THEME, "images/scroll_thumb.png")
    scroll.vertical.thumbMouseOver.rgba = rgba("#cceecc")
    scroll.vertical.thumbMouseDown.rgba = rgba("#ccffcc")
    scroll.vertical.track.texture = Texture(TextureSource.THEME, "images/scroll_track.png")
    return scroll
