
from data import *
from gumps import *
import client
import theme

def create(args):
    g = GumpMenu("characterlist", 400, 300)
    g.closable = False

    print args

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
        btnChar = theme.addPythonButton(scroll, (0, y, 190, 25), selectCharacter)
        btnChar.text = name
        btnChar.store["index"] = idx
        btnChar.store["name"] = name
        btnChar.store["password"] = password
        y += 28

    scroll.updateScrollbars()

    btnDisconnect = theme.addPythonButton(g, (20, 175, 210, 25), disconnect)
    btnDisconnect.text = "Disconnect"

    btnExit = theme.addPythonButton(g, (20, 203, 210, 25), shutdown)
    btnExit.text = "Exit"

def disconnect(button):
    client.disconnect()
    button.gump.close()
    return True

def shutdown(button):
    client.shutdown()
    return True

def selectFirst(gump):
    client.selectCharacter(gump.store["index"], gump.store["name"], gump.store["password"])
    gump.close()
    return True

def selectCharacter(button):
    client.selectCharacter(button.store["index"], button.store["name"], button.store["password"])
    button.gump.close()
    return True
