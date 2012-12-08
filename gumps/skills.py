
from ui import *
from data import *
import theme
import client
import world

def create(args):
    g = GumpMenu("skills", args.get("x", 100), args.get("y", 100))
    g.mobile = args["mobile"]
    g.onPropertyUpdate = updateSkillValues

    g.addBackground((0, 0, 400, 400), 3000)
    scroll = theme.addScrollArea(g, (5, 5, 390, 390))

    # list of tuples:
    # [0]: id
    # [1]: usable
    # [2]: name
    skills = getSkillList()

    yOffset = 0
    for cur in skills:
        if (cur[1]):
            b = scroll.addPythonButton((1, 3 + yOffset), Texture(TextureSource.GUMPART, 2103), useSkill)
            b.state("mouseDown").texture = Texture(TextureSource.GUMPART, 2104)
            b.store["skillid"] = cur[0]

        scroll.addLabel((15, 1 + yOffset, 180, 20), cur[2])

        lValue = scroll.addLabel((200, 1 + yOffset, 80, 20), "")
        lValue.name = "lValue.%s" % cur[2]

        lBase = scroll.addLabel((290, 1 + yOffset, 80, 20), "")
        lBase.name = "lBase.%s" % cur[2]

        yOffset += 22

    scroll.updateScrollbars()

    # init labels
    updateSkillValues(g)

    return g

def useSkill(button):
    client.useSkill(button.store["skillid"])
    return False

def updateSkillValues(gump):
    # list of tuples:
    # [0]: id
    # [1]: usable
    # [2]: name
    skills = getSkillList()

    for cur in skills:
        valueTxt = "%s / %s" % (gump.mobile.getProperty("skills.%s.value" % cur[2]), gump.mobile.getProperty("skills.%s.cap" % cur[2]))
        gump.component("lValue.%s" % cur[2]).text = valueTxt

        baseTxt = "Base: %s" % (gump.mobile.getProperty("skills.%s.base" % cur[2]))
        gump.component("lBase.%s" % cur[2]).text = baseTxt

def save(gump):
    saveData = {
        "x": gump.x,
        "y": gump.y,
        "serial": gump.mobile.serial
    }
    return saveData

def load(saveData):
    mob = world.getMobile(saveData["serial"])
    if mob:
        saveData["mobile"] = mob
        gump = create(saveData)

