
from ui import *
from data import *

def init(gump):
    gump.setFont("unifont1", 12, False)

def addHtmlBackground(gump, geom):
    return gump.addBackground(geom, 3000)

def addHtmlScrollbar(gump, geom):
    scroll = gump.addScrollArea(geom)
    scroll.marginLeft = 3
    scroll.marginTop = 3
    scroll.marginRight = 3
    scroll.marginBottom = 3
    scroll.vertical.width = 15
    scroll.vertical.thumb.texture = Texture(TextureSource.GUMPART, 254)
    scroll.vertical.increment.texture = Texture(TextureSource.GUMPART, 253)
    scroll.vertical.incrementMouseDown.texture = Texture(TextureSource.GUMPART, 252)
    scroll.vertical.decrement.texture = Texture(TextureSource.GUMPART, 251)
    scroll.vertical.decrementMouseOver.texture = Texture(TextureSource.GUMPART, 250)
    scroll.vertical.track.texture = Texture(TextureSource.GUMPART, 255)

    scroll.updateScrollbars()

    return scroll
