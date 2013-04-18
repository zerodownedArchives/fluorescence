
import ui
import theme
import client
import data

def create(args):
    g = ui.GumpMenu("objectpropertylist", args["x"], args["y"])
    g.draggable = False
    g.closable = False

    g.addBackground((0, 0, 250, len(args["lines"]) * 16 + 10), 3000)

    y = 5
    # lines is a list of strings
    for cur in args["lines"]:
        g.addLabel((5, y), cur)
        y += 16
