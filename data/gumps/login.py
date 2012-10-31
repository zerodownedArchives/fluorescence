
from gumps import *
from data import *
import client

def create():
    g = GumpMenu("login", 337, 289, True)
    g.setFont("Devinne Swash", 16)
    g.closable = False

    g.addImage((0, 0), Texture(TextureSource.THEME, "images/background_350x190.png"))

    g.addLabel((15, 16), "IP/Hostname:")
    g.addImage((115, 13, 220, 22), Texture(TextureSource.THEME, "images/lineedit_back.png"))
    le = g.addLineEdit((120, 15, 215, 20))
    le.text = client.getConfig("/fluo/shard/address@host")
    le.name = "hostname"
    le.onEnter = connect

    g.addLabel((15, 46), "Port:")
    g.addImage((115, 43, 220, 22), Texture(TextureSource.THEME, "images/lineedit_back.png"))
    le = g.addLineEdit((120, 45, 215, 20))
    le.numeric = True
    le.text = client.getConfig("/fluo/shard/address@port")
    le.name = "port"
    le.onEnter = connect

    g.addLabel((15, 76), "Account:")
    g.addImage((115, 73, 220, 22), Texture(TextureSource.THEME, "images/lineedit_back.png"))
    le = g.addLineEdit((120, 75, 215, 20))
    le.text = client.getConfig("/fluo/shard/account@name")
    le.name = "account"
    le.onEnter = connect

    g.addLabel((15, 106), "Password:")
    g.addImage((115, 103, 220, 22), Texture(TextureSource.THEME, "images/lineedit_back.png"))
    le = g.addLineEdit((120, 105, 215, 20))
    le.password = True
    le.text = client.getConfig("/fluo/shard/account@password")
    le.name = "password"
    le.onEnter = connect

    btnExit = g.addPythonButton((30, 150, 130, 25), Texture(TextureSource.THEME, "images/button.png"), shutdown)
    btnExit.state("mouseover").rgba = rgba("#cceecc")
    btnExit.state("mousedown").rgba = rgba("#ccffcc")
    btnExit.text = "Exit"

    btnConnect = g.addPythonButton((190, 150, 130, 25), Texture(TextureSource.THEME, "images/button.png"), connect)
    btnConnect.state("mouseover").rgba = rgba("#cceecc")
    btnConnect.state("mousedown").rgba = rgba("#ccffcc")
    btnConnect.text = "Connect"

def shutdown(component):
    client.shutdown()

def connect(component):
    g = component.gump
    if client.connect(g.component("hostname").text, int(g.component("port").text), g.component("account").text, g.component("password").text):
        return True
    else:
        client.messagebox("Could not connect to %s" % g.component("hostname").text)
        return False
