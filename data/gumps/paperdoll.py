
from ui import *
from data import *
import client
import world

def create(args):
    g = GumpMenu("paperdoll", 50, 50)
    g.mobile = args["mobile"]
    g.onWarmodeChanged = onWarmodeChanged

    g.addImage((0, 0), Texture(TextureSource.GUMPART, 2000))
    g.addPaperdollView((3, 9, 240, 300), g.mobile)
    g.addPythonButton((20, 195), Texture(TextureSource.GUMPART, 2002), openProfile)

    if g.mobile == world.getPlayer():
        btnHelp = g.addPythonButton((185, 44), Texture(TextureSource.GUMPART, 2031), sendHelpRequest)
        btnHelp.state("mouseOver").texture = Texture(TextureSource.GUMPART, 2033)
        btnHelp.state("mouseDown").texture = Texture(TextureSource.GUMPART, 2032)

        btnDisconnect = g.addPythonButton((185, 98), Texture(TextureSource.GUMPART, 2009), disconnect)
        btnDisconnect.state("mouseOver").texture = Texture(TextureSource.GUMPART, 2011)
        btnDisconnect.state("mouseDown").texture = Texture(TextureSource.GUMPART, 2010)

        btnSkills = g.addPythonButton((185, 154), Texture(TextureSource.GUMPART, 2015), openSkills)
        btnSkills.state("mouseOver").texture = Texture(TextureSource.GUMPART, 2017)
        btnSkills.state("mouseDown").texture = Texture(TextureSource.GUMPART, 2016)

        if g.mobile.warmode:
            btnWarmode = g.addPythonButton((185, 205), Texture(TextureSource.GUMPART, 2024), toggleWarmode)
            btnWarmode.state("mouseOver").texture = Texture(TextureSource.GUMPART, 2026)
            btnWarmode.state("mouseDown").texture = Texture(TextureSource.GUMPART, 2025)
        else:
            btnWarmode = g.addPythonButton((185, 205), Texture(TextureSource.GUMPART, 2021), toggleWarmode)
            btnWarmode.state("mouseOver").texture = Texture(TextureSource.GUMPART, 2023)
            btnWarmode.state("mouseDown").texture = Texture(TextureSource.GUMPART, 2022)
        btnWarmode.name = "btnWarmode"

    btStatus = g.addPythonButton((185, 233), Texture(TextureSource.GUMPART, 2027), openStatus)
    btStatus.state("mouseOver").texture = Texture(TextureSource.GUMPART, 2029)
    btStatus.state("mouseDown").texture = Texture(TextureSource.GUMPART, 2028)

    g.addPropertyLabel((20, 270, 180, 20), "paperdoll-name")

    return g

def onWarmodeChanged(gump):
    btnWarmode = gump.component("btnWarmode")
    if gump.mobile.warmode:
        btnWarmode.texture = Texture(TextureSource.GUMPART, 2024)
        btnWarmode.state("mouseOver").texture = Texture(TextureSource.GUMPART, 2026)
        btnWarmode.state("mouseDown").texture = Texture(TextureSource.GUMPART, 2025)
    else:
        btnWarmode.texture = Texture(TextureSource.GUMPART, 2021)
        btnWarmode.state("mouseOver").texture = Texture(TextureSource.GUMPART, 2023)
        btnWarmode.state("mouseDown").texture = Texture(TextureSource.GUMPART, 2022)

def openProfile(button):
    button.gump.mobile.openProfile()
    return False

def sendHelpRequest(button):
    client.sendHelpRequest()
    return False

def disconnect(button):
    client.disconnect()
    return True

def openSkills(button):
    button.gump.mobile.openSkills()
    return False

def toggleWarmode(button):
    mob = button.gump.mobile
    mob.warmode = not mob.warmode

def openStatus(button):
    button.gump.mobile.openStatus()
    return False

def save(gump):
    saveData = {
        "x": gump.x,
        "y": gump.y,
        "mobile": gump.mobile
    }
    return saveData

def load(saveData):
    gump = create(saveData)
    gump.x = saveData["x"]
    gump.y = saveData["y"]
