
import ui

def create(args):
    g = ui.GumpMenu("container", 250, 250)

    cont = g.addContainerView((0, 0), args["item"])
    cont.background = args["background"]
