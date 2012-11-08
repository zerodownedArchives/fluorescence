
from gumps import *
from data import *
import client

def create(args):
    g = GumpMenu("gamewindow", 20, 20, True)
    g.closable = False
    g.onEnter = showSpeechEntry

    g.addPage(0)
    g.addBackground((0, 0, 810, 610), 2620)
    wv = g.addWorldView((5, 5, 800, 600))
    wv.name = "worldview"

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
    }
    # nothing specific to store
    return saveData

def load(saveData):
    args = { }
    gump = create(args)
    gump.x = saveData["x"]
    gump.y = saveData["y"]

