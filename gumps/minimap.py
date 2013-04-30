
from ui import *
from data import *
import client

def create(args):
    g = GumpMenu("minimap", args.get("x", 120), args.get("y", 120))

    width = args.get("width", 300)
    height = args.get("height", 300)

    g.addBackground((0, 0, width + 6, height + 6), 2620)
    mmv = g.addMiniMapView((3, 3, width, height))
    mmv.name = "minimap"

    return g

def save(gump):
    saveData = {
        "x": gump.x,
        "y": gump.y,
    }
    # nothing specific to store
    return saveData

def load(saveData):
    gump = create(saveData)

