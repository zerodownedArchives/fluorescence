
# this gump is opened after receiving packet 0x7C and can be either
# just a question or a gump to pick an item

from ui import *
from data import *
import client
import theme

def create(args):
    g = GumpMenu("", 250, 150)
    g.store["serial"] = args["serial"] # to be included in the response
    g.store["menuid"] = args["menuid"] # to be included in the response

    g.addBackground((0, 0, 250, 300), 3000)

    title = g.addLabel((5, 7, 240, 20), args["question"])
    title.halign = HAlign.CENTER

    scroll = theme.addScrollArea(g, (5, 30, 240, 265))

    # args["answers"] is a list of tuples
    # (answerid, text, artid, hue)
    y = 0
    for answer in args["answers"]:
        if answer[2] == 0:
            # question only
            btText = theme.addPythonButton(scroll, (5, y + 5, 210, 25), onButtonClick)
            btText.store["data"] = answer
            btText.text = answer[1]
        else:
            # display art texture
            btArt = scroll.addPythonButton((5, y + 5), Texture(TextureSource.ART, answer[2]), onButtonClick)
            btArt.store["data"] = answer
            btArt.hue = answer[3]
            scroll.addLabel((50, y + 10), answer[1])
        y += 28

    scroll.updateScrollbars()
    return g

def onButtonClick(button):
    client.objectPickerResponse(button.gump.store["serial"], button.gump.store["menuid"], button.store["data"][0], button.store["data"][2], button.store["data"][3])
    return True
