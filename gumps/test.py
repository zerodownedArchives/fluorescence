# -*- coding: utf-8 -*-

from ui import *
from data import *

import time

def onButtonClick(button):
	print "onButtonClick"
	button.rgba = (1.0, 0.0, 1.0)
	print button.name
	button.halign = HAlign.RIGHT
	button.valign = VAlign.TOP

	button.gump.component("i1").hue = 90

	print button.gump.store
	print button.gump.store["i2"]
	button.gump.store["i2"].rgba = rgba(91)

	button.gump.component("l2").halign = HAlign.RIGHT

def create(args):
	g = GumpMenu("test", 30, 30)

	b1 = g.addBackground((0, 0, 500, 500), 3000)
	b1.rgba = rgba("#ff0000")

	# test for texture class
	i1 = g.addImage((0, 0, 100, 100), Texture(TextureSource.THEME, "images/button.png"))
	i1.texture = Texture(TextureSource.GUMPART, 13)
	i1.geometry = (30, 30)
	i1.hue = 13

	i2 = g.addImage((0, 0), Texture(TextureSource.GUMPART, 12))
	i2.rgba = (0.8, 0.2, 0.0)
	i2.alpha = 0.6
	i2.hue = 2

	but = g.addPythonButton((10, 10, 100, 60), Texture(TextureSource.THEME, "images/button.png"), onButtonClick)
	but.state("mouseOver").hue = 13
	but.state("mouseDown").rgba = (1.0, 1.0, 0.0)
	but.text = "foooobert"
	but.state("mouseDown").fontRgba = rgba(4)
	but.state("mouseOver").fontRgba = rgba("#00ffff")
	but.setFont("Arial", 8);
	i1.name = "i1"
	g.store["i2"] = i2

	# cliloc test
	str2 = Cliloc(501522)
	str3 = Cliloc(1072058, [ "fo", "awef" ])
	print str2
	print str3

	g.addAlphaRegion((50, 50, 100, 100), 0.9)

	l1 = g.addLabel((300, 300), str2)
	l1.halign = HAlign.RIGHT
	l1.rgba = rgba(13)

	l2 = g.addLabel((5, 300, 200, 30), "centered")
	l2.halign = HAlign.CENTER
	l2.name = "l2"
