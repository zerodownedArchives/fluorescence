
import ui
from data import Texture, TextureSource
import client
import theme

def create(args):
    g = ui.GumpMenu("yesnobox", 300, 300)
    g.onEnter = choiceYes
    g.onEscape = choiceNo
    g.store["callbackYes"] = args["callbackYes"]
    g.store["callbackNo"] = args["callbackNo"]
    g.store["callbackArgs"] = args["callbackArgs"]

    g.addImage((0, 0), Texture(TextureSource.THEME, "images/background_250x125.png"))
    question = g.addLabel((10, 15, 230, 80), args["question"])
    question.halign = ui.HAlign.CENTER

    btYes = theme.addPythonButton(g, (20, 90, 80, 25), choiceYes)
    btYes.text = "Yes"
    btYes.store["callbackYes"] = args["callbackYes"]
    btYes.store["callbackArgs"] = args["callbackArgs"]

    btNo = theme.addPythonButton(g, (120, 90, 80, 25), choiceNo)
    btNo.text = "No"
    btNo.store["callbackNo"] = args["callbackNo"]
    btNo.store["callbackArgs"] = args["callbackArgs"]


def choiceYes(obj):
    obj.store["callbackYes"](obj.store["callbackArgs"])
    return True

def choiceNo(obj):
    obj.store["callbackNo"](obj.store["callbackArgs"])
    return True
