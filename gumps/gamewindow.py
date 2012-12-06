
from ui import *
from data import *
import client

def create(args):
    g = GumpMenu("gamewindow", args.get("x", 20), args.get("y", 20), True)
    g.closable = False
    g.onEnter = showSpeechEntry

    width = args.get("width", 800)
    height = args.get("height", 600)

    g.addPage(0)
    g.addBackground((0, 0, width + 10, height + 10), 2620)
    wv = g.addWorldView((5, 5, width, height))
    wv.name = "worldview"
    syslog = g.addSysLogLabel((7, 7, 250, height - 40))
    syslog.setFont("unifont1", 12, True)
    syslog.rgba = rgba("#dddddd") # default color

    # empty page to prevent the speech entry from being displayed at startup
    g.addPage(1)

    g.addPage(2)
    bg = g.addImage((5, 581, 608, 22), Texture(TextureSource.THEME, "images/solid.png"))
    bg.rgba = rgba("#dddddd33")
    speechEntry = g.addLineEdit((7, 583, 600, 20))
    speechEntry.setFont("unifont1", 12)
    speechEntry.name = "speechentry"
    speechEntry.onEnter = sendSpeech

    return g

def showSpeechEntry(gump):
    gump.page = 2
    gump.component("speechentry").focus()

def sendSpeech(entry):
    client.handleTextInput(entry.text)
    entry.text = ""
    entry.gump.page = 1
    entry.gump.component("worldview").focus()

def save(gump):
    saveData = {
        "x": gump.x,
        "y": gump.y,
        "width": gump.component("worldview").width,
        "height": gump.component("worldview").height,
    }
    # nothing specific to store
    return saveData

def load(saveData):
    args = { }
    gump = create(args)

