With themes, you can customize the appearance of fluorescence. Additionally, they allow you to speed up development of new gump menus. To understand this page, you need to familiarize yourself with [XML gump menus](XMLGumpMenus.md) first.



# Structure #
At the core of every theme stands the [templates.xml](#Templates.md) file. All other files are optional.

In the predefined themes shipped with fluorescence, there is an images folder (containing, unsurprisingly, images). But how you structure the files for your theme is up to you.

# Templates #
The templates.xml file starts, as every other XML file in fluorescence, with the XML header:
```
<?xml version="1.0"?>
```
After that, the `<templates>` tag is opened, enclosing all templates in this file. At the end of the file, don't forget to close the `</templates>` tag again.

Inside the templates tag, you can define an arbitrary number of templates. Simply put, a template provides default values for gump components. Each template has a name, and encloses exactly one gump component (see [XMLGumpMenus](XMLGumpMenus.md)). For example, a template for the small blue knob button (used for example in the skills menu) could look like this:
```
<template name="blueknob">
    <button>
        <normal source="gumpart" id="2103" />
        <mouseover source="gumpart" id="2103" />
        <mousedown source="gumpart" id="2104" />
    </button>
</template>
```
As you can see, the template is named _blueknob_ and uses the gumpart ids 2103 and 2104. To use this template in a gump menu, you would write:
```
<!-- other components -->
<button template="blueknob" x="30" y="30" />
<button template="blueknob" x="30" y="60" />
<!-- other components -->
```
This creates blue knobs at 30/30 and 30/60. As you can see, the gump definition gets much shorter this way - without templates, it would look like this:
```
<!-- other components -->
<button x="30" y="30">
    <normal source="gumpart" id="2103" />
    <mouseover source="gumpart" id="2103" />
    <mousedown source="gumpart" id="2104" />
</buton>
<button x="30" y="60">
    <normal source="gumpart" id="2103" />
    <mouseover source="gumpart" id="2103" />
    <mousedown source="gumpart" id="2104" />
</button>
<!-- other components -->
```

The attributes used to create the buttons in the gump are the **sum total** of the template and the values given in the XML gump definition itself. If an attribute appears in both, the value from the gump definition is favored.

Thus, you can also selectively overwrite values from the template. For example, lets change the texture of the button when its pressed, and the color for mouseover:
```
<button template="blueknob" x="30" y="30">
    <mouseover color="#ff0000" />
    <mousedown id="2345" />
</button>
```

As said above, the resulting gump component is created from the sum total of all attributes. Thus, the combination of the blueknob template and the fancy button is equal to
```
<button x="30" y="30">
    <normal source="gumpart" id="2103" />
    <mouseover source="gumpart" id="2103" color="#ff0000 />
    <mousedown source="gumpart" id="2345" />
</button>
```

Thus, defining templates for often used gump components can speed up the gump creation process (you don't have to look for these damn numbers all the time) and increase the readability of the resulting XML gumps.

## Default templates ##
You can also define a template as default:
```
<template name="label" default="1">
    <label font="Arial" font-height="12" color="#00ff00" />
</template>
```
The values of this template are then used for every label without a specific _template_ attribute. Keep in mind though that in this case, every label without a template is green, if it does not define its own color attribute.

## Summary ##
To sum it up, here is what you need to know about templates:
  * Most importantly, each template can hold values for **only one** component. Adding a definition for labels to the _blueknob_ template above would result in an invalid template file
  * A template is referenced by its name (see [XMLGumpMenus#Common\_Attributes](XMLGumpMenus#Common_Attributes.md))
  * The components are created based on the **sum total** of all attributes in the template and those in the XML gump
  * The attributes defined in the actual XML gump overwrite the values from the template
  * You can define a template as default

# Theming #
A theme is a full set of all templates used in the XML gumps. To create a new theme, it is probably the easiest approach to copy an existing theme and modify its graphical properties. If a required template is missing, some components might not be rendered correctly or even throw errors while parsing.

## Server side gumps ##
The gumps sent by the server also need some information from your theme, for example which font to use for a label. In detail, you **have to** provide the following templates in your theme (otherwise server side gumps will not work):

| **Template name** | **XML component** | **Required attributes**| **Comment** |
|:------------------|:------------------|:-----------------------|:------------|
|ssglabel|label|font, font-height|You can define which font to use in all server side gump labels|
|ssghtmllabel|htmllabel|background-template, scrollbar-template, font, font-height, color|The default font and font color for htmllabel components (color can be overridden with the _basefont_ tag, as well as valid templates for the auto scrollbar and auto background|
|ssglineedit|lineedit|font, font-height|You can define which font to use in all server side gump text entries|

For all other components, all the required attributes are sent by the server.

## preshard ##
The preshard theme is a minimal theme, covering only the dialogs until a shard is selected (i.e. the shard selection, shard creation and messagebox gumps). In this theme, data from the mul files is not available.

## Textures inside the theme ##
Several components, for example the image, are based on textures. There is an extra texture source "theme" that points to the active theme directory (see [XMLGumpMenus#Image\_Sources](XMLGumpMenus#Image_Sources.md)). All _id_ attributes where _source="theme"_ are relative paths inside the active theme directory.