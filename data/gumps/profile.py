
from ui import *
from data import *

def create(args):
    g = GumpMenu("profile", 100, 100)
    g.mobile = args["mobile"]

    # TODO: make this look like the uo one
    g.addImage((0, 0), Texture(TextureSource.THEME, "images/background_250x250.png"))

    # TODO: editable for own profile
    g.addPropertyLabel((10, 10, 230, 40), "profile-header")
    g.addPropertyLabel((10, 55, 230, 140), "profile-text")
    g.addPropertyLabel((10, 205, 230, 40), "profile-static")
