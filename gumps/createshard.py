
from ui import *
from data import *
import client
import theme

#In case you want to redesign this gump, keep in mind that at this point, you can not
#access any uo data (e.g. art.mul graphics, hues), as the mul files are not loaded yet.
def create(args):
    g = GumpMenu("createshard", 400, 300)
    g.draggable = False
    g.onEnter = createShard
    theme.setFont(g)

    g.addImage((0, 0), Texture(TextureSource.THEME, "images/background_350x190.png"))

    g.addLabel((15, 16), "Name:")
    g.addImage((115, 13, 220, 22), Texture(TextureSource.THEME, "images/lineedit_back.png"))
    le = g.addLineEdit((120, 15, 215, 20))
    le.name = "shardname"

    g.addLabel((15, 46), "UO path:")
    g.addImage((115, 43, 220, 22), Texture(TextureSource.THEME, "images/lineedit_back.png"))
    le = g.addLineEdit((120, 45, 215, 20))
    le.name = "uopath"

    cb = theme.addCheckbox(g, (15, 80))
    cb.name = "highseas"
    g.addLabel((35, 80), "High seas")

    btnCreate = theme.addPythonButton(g, (30, 150, 130, 25), createShardButton)
    btnCreate.text = "Create shard"

    btnCancel = theme.addPythonButton(g, (190, 150, 130, 25), cancel)
    btnCancel.text = "Cancel"

def createShardButton(btn):
    return createShard(btn.gump)

def createShard(gump):
    if client.createShard(gump.component("shardname").text, gump.component("uopath").text, gump.component("highseas").checked):
        return True
    else:
        return False

def cancel(_):
    # close gump
    return True
