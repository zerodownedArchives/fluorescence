
import ui
import theme
import client
import data

def create(args):
    g = ui.GumpMenu("contextmenu", 50, 50)
    g.draggable = False

    g.store["serial"] = args["serial"]

    y = 0
    # entries is a list of tuples
    # (entry id, cliloc id)
    for cur in args["entries"]:
        bt = theme.addPythonButton(g, (0, y, 300, 25), onClick)
        bt.store["entryId"] = cur[0]
        bt.text = data.Cliloc(cur[1])
        y += 25

def onClick(button):
    client.contextMenuReply(button.gump.store["serial"], button.store["entryId"])
    return True
