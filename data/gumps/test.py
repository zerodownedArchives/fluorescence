# -*- coding: utf-8 -*-

from gumps import *
from data import *

import time

def create():
	g = GumpMenu(30, 30)

	b1 = g.addBackground((0, 0, 500, 500), 3000)
	b1.setRgba("#ff0000")

	# test for texture class
	i1 = g.addImage((0, 0, 100, 100), Texture(TextureSource.THEME, "images/button.png"))
	i1.texture = Texture(TextureSource.GUMPART, 13)
	i1.geometry = (30, 30)
	i1.hue = 13

	i2 = g.addImage((0, 0), Texture(TextureSource.GUMPART, 12))
	i2.rgba = (0.8, 0.2, 0.0)
	i2.alpha = 0.6
	i2.hue = 2

	i2.state("foo").rgba = (0.0, 1.0, 0.0)
	i2.setState("foo")

	but = g.addButton((10, 10), Texture(TextureSource.THEME, "images/button.png"))
	but.state("mouseover").hue = 13
	but.state("mousedown").rgba = (1.0, 1.0, 0.0)


	# cliloc test
	str2 = Cliloc(501522)
	str3 = Cliloc(1072058, [ "fo", "awef" ])
	print str2
	print str3

	g.addAlphaRegion((50, 50, 100, 100), 0.9)

	#l1 = g.addLabel((300, 300, 120, 25), str2)
	#print l1.align
	#l1.align = Alignment.RIGHT
	#foo = Alignment.RIGHT
	#print l1.align
	#print foo

	return g
