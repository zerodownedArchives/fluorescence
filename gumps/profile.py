
from ui import *
from data import *
import world

def create(args):
    g = GumpMenu("profile", args.get("x", 100), args.get("y", 100))
    g.mobile = args["mobile"]

    # TODO: make this look like the uo one
    g.addImage((0, 0), Texture(TextureSource.THEME, "images/background_250x250.png"))

    # TODO: editable for own profile
    g.addPropertyLabel((10, 10, 230, 40), "profile-header")
    g.addPropertyLabel((10, 55, 230, 140), "profile-text")
    g.addPropertyLabel((10, 205, 230, 40), "profile-static")

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
        # request profile data from server
        saveData["mobile"].openProfile()
