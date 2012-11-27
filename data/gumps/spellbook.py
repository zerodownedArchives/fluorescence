
import ui
from data import Texture, TextureSource, rgba
import client

def create(args):
    g = ui.GumpMenu("spellbook", 100, 100)
    g.setFont("Devinne Swash", 16)

    g.addPage(0)
    g.addImage((0, 0), Texture(TextureSource.GUMPART, 2220))

    # circle numbers
    g.addPageButton((58, 175), Texture(TextureSource.GUMPART, 2225), 1)
    g.addPageButton((92, 175), Texture(TextureSource.GUMPART, 2226), 1)
    g.addPageButton((129, 175), Texture(TextureSource.GUMPART, 2227), 2)
    g.addPageButton((164, 175), Texture(TextureSource.GUMPART, 2228), 2)
    g.addPageButton((227, 175), Texture(TextureSource.GUMPART, 2229), 3)
    g.addPageButton((260, 175), Texture(TextureSource.GUMPART, 2230), 3)
    g.addPageButton((297, 175), Texture(TextureSource.GUMPART, 2231), 4)
    g.addPageButton((332, 175), Texture(TextureSource.GUMPART, 2232), 4)

    # circles is a list of dictionaries, each dictionary holding a list of spells and infos about the circle:
    # circle["spells"]: list of spells
    # ["id"]: numerical id
    # ["name"]: name

    # each spell is a tuple
    # (id, name, wops, descriptionHeader, description, gumpid)
    # id: numerical id
    # name: name
    # wops: words of power
    # descriptionHeader: header text of the spell description
    # description: spell description
    # gumpid: spell icon id
    circles = args["circles"]

    page = 0
    spellPage = 5
    spellCount = 0
    # circle overviews
    for circle in circles:
        if circle["id"] % 2 == 1:
            # left page
            page += 1
            g.addPage(page)

            # flip back
            if page > 1:
                g.addPageButton((50, 8), Texture(TextureSource.GUMPART, 2235), page - 1)
            # flip forward
            g.addPageButton((321, 8), Texture(TextureSource.GUMPART, 2236), page + 1)

            addTitleLabel(g, (100, 14, 40, 15), "INDEX")
            addTitleLabel(g, (55, 34, 150, 15), circle["name"])

            spellX = 55
        else:
            # right page
            addTitleLabel(g, (267, 14, 40, 15), "INDEX")
            addTitleLabel(g, (223, 34, 150, 15), circle["name"])

            spellX = 223

        spellY = 50
        for spell in circle["spells"]:
            l = addTextLabel(g, (spellX, spellY, 150, 15), spell[1])
            l.page = spellPage # flip gump to this page if clicked
            spellCount += 1
            if spellCount % 2 == 0:
                spellPage += 1

            spellY += 15

    spellPage = 5
    spellCount = 0
    x = 0
    for circle in circles:
        for spell in circle["spells"]:
            if spellCount % 2 == 0:
                # add flip forward to previous page
                if spellCount > 1:
                    g.addPageButton((321, 8), Texture(TextureSource.GUMPART, 2236), spellPage + 1)
                g.addPage(spellPage)
                # flip back
                g.addPageButton((50, 8), Texture(TextureSource.GUMPART, 2235), spellPage - 1)
                spellPage += 1
                x = 0

                # circle header for left page
                addTitleLabel(g, (x + 85, 14, 100, 15), circle["name"])
            else:
                # circle header for right page
                addTitleLabel(g, (x + 65, 14, 100, 15), circle["name"])

            # spell name
            addTitleLabel(g, (x + 105, 38, 90, 35), spell[1])
            # description title
            addTitleLabel(g, (x + 55, 95, 150, 15), spell[3])
            # description
            addTextLabel(g, (x + 55, 110, 150, 55), spell[4])

            # spell icon
            btCast = g.addPythonButton((x + 55, 40), Texture(TextureSource.GUMPART, spell[5]), castSpell)
            btCast.store["spellid"] = spell[0]

            spellCount += 1
            x += 168

            #<repeatif xindex="0">
                #<label template="spellbooktitle" x="85" y="14" width="100" height="15" text="sectionname" />
            #</repeatif>
            #<repeatif xindex="1">
                #<label template="spellbooktitle" x="65" y="14" width="100" height="15" text="sectionname" />
            #</repeatif>

    args["item"].setSpellbookGump(g)

def addTitleLabel(g, geom, text):
    l = g.addLabel(geom, text)
    l.rgba = rgba("#001052")
    return l

def addTextLabel(g, geom, text):
    l = g.addLabel(geom, text)
    l.rgba = rgba("#5a4a31")
    return l

def castSpell(button):
    client.castSpell(button.store["spellid"])
