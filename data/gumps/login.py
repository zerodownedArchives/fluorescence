
from world import *
from ui import *
from data import *
import client
import theme

def create(args):
    g = GumpMenu("login", 337, 289, True)
    theme.setFont(g)
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

    btnExit = theme.addPythonButton(g, (30, 150, 130, 25), shutdown)
    btnExit.text = "Exit"

    btnConnect = theme.addPythonButton(g, (190, 150, 130, 25), connect)
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
