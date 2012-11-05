
from data import *
from gumps import *
import client
import theme

def create(args):
    g = GumpMenu("serverlist", 400, 300)
    g.closable = False

    print args

    serverList = args["serverlist"]
    # serverlist is a list of tuples (index, name)
    if len(serverList) > 0:
        g.onEnter = selectFirst
        g.store["firstServer"] = serverList[0][0]

    g.addImage((0, 0), Texture(TextureSource.THEME, "images/background_250x250.png"))

    scroll = theme.addScrollArea(g, (20, 20, 210, 137))

    y = 0
    for (idx, name) in serverList:
        btnServer = theme.addPythonButton(scroll, (0, y, 190, 25), selectServer)
        btnServer.text = name
        btnServer.store["server"] = idx
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
    client.selectServer(gump.store["firstServer"])
    gump.close()
    return True

def selectServer(button):
    client.selectServer(button.store["server"])
    button.gump.close()
    return True
