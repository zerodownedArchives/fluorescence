
from data import *
from gumps import *
import client
import theme

# In case you want to redesign this gump, keep in mind that at this point, you can not
# access any uo data (e.g. art.mul graphics, hues), as the mul files are not loaded yet.
def create(args):
    g = GumpMenu("shardlist", 400, 300)
    g.closable = False

    shardlist = args["shardlist"]
    if len(shardlist) > 0:
        g.onEnter = selectFirst
        g.store["firstName"] = shardlist[0]
    else:
        g.onEnter = createShard

    g.addImage((0, 0), Texture(TextureSource.THEME, "images/background_250x250.png"))

    scroll = theme.addScrollArea(g, (20, 20, 210, 137))

    y = 0
    for shard in shardlist:
        btnShard = theme.addPythonButton(scroll, (0, y, 190, 25), selectShard)
        btnShard.text = shard
        btnShard.store["shard"] = shard
        y += 28

    scroll.updateScrollbars()

    btnCreate = theme.addPythonButton(g, (20, 175, 210, 25), createShard)
    btnCreate.text = "Create shard"

    btnExit = theme.addPythonButton(g, (20, 203, 210, 25), shutdown)
    btnExit.text = "Exit"

def createShard(button):
    client.openGump("createshard")
    # don't close the gump, shard creation can be cancelled
    return False

def shutdown(button):
    client.shutdown()
    return True

def selectFirst(gump):
    client.setShard(gump.store["firstName"])
    return True

def selectShard(button):
    client.setShard(button.store["shard"])
    return True
