
from data import *
from gumps import *
import client
import theme

# In case you want to redesign this gump, keep in mind that at this point, you can not
# access any uo data (e.g. art.mul graphics, hues), as the mul files are not loaded yet.
def create(args):
    g = GumpMenu("shardselection", 400, 300)
    g.closable = False

    shardlist = args["shards"]
    if len(shardlist) > 0:
        g.onEnter = selectFirst
        g.store["firstName"] = shardlist[0]
    else:
        g.onEnter = createShard

    g.addImage((0, 0), Texture(TextureSource.THEME, "images/background_250x250.png"))

    y = 20
    for shard in shardlist:
        btnShard = theme.addPythonButton(g, (20, y, 190, 25), selectShard)
        btnShard.text = shard
        btnShard.store["shard"] = shard
        y += 28

    btnCreate = theme.addPythonButton(g, (20, 175, 210, 25), createShard)
    btnCreate.text = "Create shard"

    btnExit = theme.addPythonButton(g, (20, 203, 210, 25), shutdown)
    btnExit.text = "Exit"

def createShard(button):
    print "todo"
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


#<?xml version="1.0"?>
#<gump x="400" y="300" closable="0" action="selectshard-first" cancelaction="shutdown">

    #<image x="0" y="0" source="theme" id="images/background_250x250.png" />
    #<scrollarea x="20" y="20" width="210" height="137">
        #<content>
            #<repeat name="shardlist" yincrease="28" xlimit="1">
                #<!--
                #Each entry represents one shard. Properties:
                #- shardname: Name of the shard
                #-->
                #<button x="0" y="0" width="190" name="selectshard" action="selectshard" param="shardname" text="shardname" />
            #</repeat>
        #</content>
    #</scrollarea>

    #<button x="20" y="175" width="210" text="Create shard" action="opengump" param="createshard" />
    #<button x="20" y="203" width="210" text="Close fluorescence" action="shutdown" />
#</gump>
