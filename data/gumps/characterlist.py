
from data import *
from ui import *
import client
import theme

def create(args):
    g = GumpMenu("characterlist", 400, 300)
    g.closable = False

    charList = args["characterlist"]
    # characterlist is a list of tuples (index, name, password)
    if len(charList) > 0:
        g.onEnter = selectFirst
        g.store["index"] = charList[0][0]
        g.store["name"] = charList[0][1]
        g.store["password"] = charList[0][2]

    g.addImage((0, 0), Texture(TextureSource.THEME, "images/background_250x250.png"))

    scroll = theme.addScrollArea(g, (20, 20, 210, 137))

    y = 0
    for (idx, name, password) in charList:
        btnChar = theme.addPythonButton(scroll, (0, y, 160, 25), selectCharacter)
        btnChar.text = name
        btnChar.store["index"] = idx
        btnChar.store["name"] = name
        btnChar.store["password"] = password

        btnDel = theme.addPythonButton(scroll, (165, y, 25, 25), deleteCharacter)
        btnDel.store["index"] = idx
        btnDel.store["password"] = password
        btnDel.text = "X"

        y += 28

    scroll.updateScrollbars()

    btnDummy = theme.addPythonButton(g, (20, 175, 210, 25), createDummyCharacter)
    btnDummy.text = "Create Dummy Character"

    btnDisconnect = theme.addPythonButton(g, (20, 203, 210, 25), disconnect)
    btnDisconnect.text = "Disconnect"

def disconnect(button):
    client.disconnect()
    button.gump.close()
    return True

def selectFirst(gump):
    client.selectCharacter(gump.store["index"], gump.store["name"], gump.store["password"])
    gump.close()
    return True

def selectCharacter(button):
    client.selectCharacter(button.store["index"], button.store["name"], button.store["password"])
    button.gump.close()
    return True

def createDummyCharacter(button):
    client.createDummyCharacter()
    button.gump.close()
    return True

def deleteCharacter(button):
    callbackArgs = {
        "index": button.store["index"],
        "password": button.store["password"],
        "gump": button.gump,
    }
    args = {
        "callbackYes": deleteCharacter2,
        "callbackNo": cancelDelete,
        "callbackArgs": callbackArgs,
        "question": "Do you really want to delete this character?",
    }
    client.openGump("yesnobox", args)
    return False

def deleteCharacter2(args):
    client.deleteCharacter(args["index"], args["password"])

def cancelDelete(args):
    pass
