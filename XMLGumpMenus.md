In fluorescence, all the client-side gump menus are defined in XML. This page documents the various components and the options to customize them.
Examples for all the components can be found in the "simpletest.xml" gump.



# Header #
Every XML gump starts with the XML header, which can provide helpful information to the XML processor. It's no problem if you don't know exactly what it means, just copy it to your XML gumps.
```
<?xml version="1.0"?> 
```

After the XML header the gump tag is opened. Every XML gump file has to contain exactly one gump. Here is an example:
```
<gump x="100" y="30" closable="1" draggable="1">
```
In this tag, you can define attributes for the gump window itself. The available options are:

| **Attribute** | **[Type](#Attribute_Types.md)** | **Description** | **Default** | **Example** |
|:--------------|:--------------------------------|:----------------|:------------|:------------|
|x |int|X position | 0 | x="100"|
|y |int|Y position| 0 | y="30"|
|closable| bool| Whether or not the user can close the gump with the right mouse button | 0|closable="1"|
|draggable| bool|Wheter or not the user can drag the gump on the screen|0 |draggable="1"|
|inbackground| bool| Window stays in background (e.g. gamewindow) | 0|background="0"|
|action| action| Action to execute when user presses the return key. See GumpActions for more information| none| action="selectserver-first"|
|cancelaction| action |Action to execute when user presses the escape key. See GumpActions for more information| none| cancelaction="disconnect"|

If you omit any of these attributes, the default value for it is assigned. So for example, the example line above would create a gump that does not stay in the background, because the _inbackground_ parameter is automatically set to 0.

The last line of every gump definition is the closing gump tag:
```
</gump>
```

# Attribute Types #
An important thing to know when defining gumps is to know the valid values for each of the options. These are defined by the Type column.

| **Type** | **Description**|
|:---------|:---------------|
|int| An integral number, for example 3, -20 or 345096 |
|float| A floating point number. For example, 3.141592 or 12.345|
|bool| A truth value (true or false), represented as a number. 1 is true, 0 is false |
|string| Arbitrary text, e.g. "This is some arbitrary text" |
|color| An RGB(A) color, formatted either as "#RRGGBB" or "#RRGGBBAA". For example, "#8A0808" is a dark red, "#F7FE2E77" is a semi-transparent, intense yellow|
|action| This identifies an action that should be executed, e.g. after clicking a button. See GumpActions for more information on actions|
|enum| An enumeration, i.e. one of a given set of values. enum(left, right, center) defines three possible ways to align text|

Please note that independent of the type, all parameters have to be given in quotes (e.g. x="20").

# Common attributes #
Some attributes are available for all (or almost all) components. To reduce the size of this documentation, the common attributes are given in this section. If a component does not offer any of these values (or treats them in a special way), it will be mentioned in the component's section.
| **Attribute** | **[Type](#Attribute_Types.md)** | **Description** | **Default** | **Example** |
|:--------------|:--------------------------------|:----------------|:------------|:------------|
|name|string|Used to identify important gump elements from source code, e.g. the text entry that holds your account name|none|name="loginname"|
|x |int|X position | 0 | x="100"|
|y |int|Y position| 0 | y="30"|
|width|int|Width of the component| 0 | width="200"|
|height|int|Height of the component| 0 | height="200"|
|color|color|The RGB(A) color this component should have| #FFFFFF (white)|color="#FF6633"|
|hue|int|The UO hue from hues.mul this component should be rendered in|0 |hue="13"|
|template|string|The name of the template to use for this component. See GumpThemes for more details|none|theme="blueknob"|

The x/y/width/height properties define the position and size of the component, and thus need to be present - a component with default width (0) is invisible.

Color and hue are mutually exclusive, they are both ways to define the color the component should have. It is important to know that the color attribute is more significant. Thus, if a component defines both hue and color, the color attribute will be used for rendering.

# Image Sources #
XML gumps need a lot of different textures. Whether it is a button, a background or just an image, a lot of components are based on textures. To load a texture, fluorescence requires two pieces of information: From which source the texture should be loaded and an ID to identify the texture within that source.

| **Source** | **Description** | **id** | **Example** |
|:-----------|:----------------|:-------|:------------|
|mapart| Map tile graphics from art.mul|Tile ID|source="mapart" id="123"|
|staticart| Static tile graphics from art.mul|Art ID|source="staticart" id="456"|
|gumpart| Textures from gump.mul|Gump ID|source="gumpart" id="789"|
|file|Load texture from a file on the harddisk (e.g. png, jpg)|File path|source="file" id="effects/textures/explosion.png"|
|http|Load texture from a file on the internet|Full URL|source="http" `id="http://yourdomain.com/someimage.png"`|
|theme|Load texture from the current theme directory. See GumpThemes for more information on themes|File path|source="theme" `id="Images/ButtonNormal.png"`|

# Components #

## background ##
A background component is a scalable background image, created from 9 combined textures.
| **Attribute** | **[Type](#Attribute_Types.md)** | **Description** | **Default** | **Example** |
|:--------------|:--------------------------------|:----------------|:------------|:------------|
|gumpid| int |Gumpart id of the top left corner image|0 |gumpid="1010" |
|alpha|float|Opacity [0.0 - 1.0]|alpha="0.7"|

Example:
```
<background x="0" y="0" width="600" height="600" gumpid="2600" />
```

## image ##
A single image without any function.
| **Attribute** | **[Type](#Attribute_Types.md)** | **Description** | **Default** | **Example** |
|:--------------|:--------------------------------|:----------------|:------------|:------------|
|source |See [Image Sources](#Image_Sources.md)|See [Image Sources](#Image_Sources.md)|none |source="http", source="gumpart" |
|id| See [Image Sources](#Image_Sources.md) |See [Image Sources](#Image_Sources.md)|none|imgid="13", imgid="`http://yourdomain.com/someimage.png"` |
|alpha|float|Opacity [0.0 - 1.0]. Used to define transparent gumps with an UO hue. If a color is used, the opacity can be given as part or the RGBA color|1 (opaque)|alpha="0.7"|
|tiled|bool|Repeat texture over area|0 |tiled="1"|

For images, the width and height property can be omitted. The image is then displayed in its native size. For tiled images, width and height attributes are required.

Examples:
```
<image x="30" y="290" source="http" id="http://wiseheartdesign.com/page_attachments/0000/0062/default-avatar.png" hue="13" alpha="0.2" />
<image x="25" y="480" width="300" height="300" source="file" id="effects/textures/explosion.png" tiled="true" color="#00ff00" />
```


## page ##
The typical UO gump pages are represented by the page component.
| **Attribute** | **[Type](#Attribute_Types.md)** | **Description** | **Default** | **Example** |
|:--------------|:--------------------------------|:----------------|:------------|:------------|
|number|int|The ID of this page|0 |number="6"|

Please note that pages are special - they are not rendered directly, but are used as a container for other components. Thus, pages do not have any other attributes aside from the page number (no x/y/width/height, no color/hue).

The components on this page are enclosed between the opening and closing page tag (see example below). Any components not between page tags are automatically on page 0.

In the example below, the mapart image is visible on all pages (because it is on page 0), and there are two pages with one image each.
```
<?xml version="1.0"?>
<gump x="100" y="30" closable="1" draggable="true">
    <image x="65" y="40" source="mapart" id="15"/>
    <page number="1">
        <image x="65" y="480" source="staticart" id="16" hue="100" />
    </page>
    <page number="2">
        <image x="65" y="480" source="gumpart" id="17" />
    </page>
</gump>
```

## label ##
A label component is used to display text.
| **Attribute** | **[Type](#Attribute_Types.md)** | **Description** | **Default** | **Example** |
|:--------------|:--------------------------------|:----------------|:------------|:------------|
|align | enum(center, right, left) | Alignment of the text inside the component box |left| align="center" or align="right"|
|text | string|The text to display with this label|none|text="Something" |
|cliloc|int|To display a cliloc string in a label|0 |cliloc="1234567"|
|args|string|Arguments to the cliloc string. If the cliloc requires more than one parameter, they are separated with "\t".|none|args="Gold\tSilver\tBronze"|
|font|string|The font to use for this label|Arial|font="unifont1" or font="Tahoma"|
|font-height|int|The height of the font|0 |font-height="12"|

Cliloc and text are mutually exclusive. If a cliloc attribute is given, the text attribute is ignored.

If no height or width are given, the label tries to set its size automatically. Please note that alignment might not have the desired effect, if no explicit width is given.

```
<label x="80" y="75" width="100" height="20" text="Foobar" hue="13" />
<label x="10" y="20" width="150" height="30" cliloc="123456" font="unifont3" align="center" />
```

## clicklabel ##
A clicklabel is a label that can do something when clicked. It has all the attributes of a label, and additionally:
| **Attribute** | **[Type](#Attribute_Types.md)** | **Description** | **Default** | **Example** |
|:--------------|:--------------------------------|:----------------|:------------|:------------|
|buttonid|int|When set, sends this id back to the server when clicked|0 |buttonid="13"|
|page|int|When set, flips gump to the given page|0 |page="3"|
|action|action|When set, executes client side action when clicked. See GumpActions for more information|none|action="disconnect"|
|param, param[2-5]|string|Parameters for action|none|param="foo", param3="bar"|

The attributes buttonid, page and action are mutually exclusive. action takes precedence over buttonid and page, if more than one is given.

```
<clicklabel x="80" y="75" width="100" height="20" text="Foobar" hue="13" buttonid="13" />
```

## propertylabel ##
A propertylabel is a label that shows a certain property of a mobile. Some gumps (e.g. the paperdoll) are linked to one specific mobile, and show properties of this mobile, for example the name.

A propertylabel shares a lot of attributes with the label, but it does not have attributes for text or cliloc. Instead, it links to a certain property. For a list of properties, see MobileProperties.
| **Attribute** | **[Type](#Attribute_Types.md)** | **Description** | **Default** | **Example** |
|:--------------|:--------------------------------|:----------------|:------------|:------------|
|link | string | Property name that this label is displaying|none| link="paperdoll-name"|

```
<propertylabel x="20" y="270" width="180" height="20" link="paperdoll-name" />
```

## htmllabel ##
A hmtmllabel is a label that can display a handful of html tags. It can be created with a background and scrollbar. It has all attributes of a label, except for _text_.
| **Attribute** | **[Type](#Attribute_Types.md)** | **Description** | **Default** | **Example** |
|:--------------|:--------------------------------|:----------------|:------------|:------------|
|background|bool|Whether or not to draw the auto background|0 |background="1"|
|background-template|string (template)|The template to use for the background. See [GumpThemes](GumpThemes.md) for more details on templates|none|background-template="thingreybackground"|
|scrollbar|bool|Wheter or not to draw the auto scrollbar|0 |scrollbar="1"|
|scrollbar-template|string (template)|The template to use for the scrollbar. See [GumpThemes](GumpThemes.md) for more details on templates|none|scrollbar-template="uoscrollbar"|

The attributes of the background and scrollbar templates can not be overwritten.

The text between the opening and closing `<htmllabel>` tags is treated as the html text. Most likely, you will have to create a [CDATA](http://en.wikipedia.org/wiki/CDATA) tag:
```
<htmllabel x="5" y="200" width="300" height="180" background="1" scrollbar="1">
    <![CDATA[
This is some long text <br>with line <br /><basefont color="#ff0000">breaks</basefont>
and newlines<br />
<b>and even</b><i>italic</i>text!
    ]]>
</htmllabel>
```

## button ##
A button is basically a click-able image (with some extra features)
| **Attribute** | **[Type](#Attribute_Types.md)** | **Description** | **Default** | **Example** |
|:--------------|:--------------------------------|:----------------|:------------|:------------|
|buttonid|int|When set, sends this id back to the server when clicked|0 |buttonid="13"|
|page|int|When set, flips gump to the given page|0 |page="3"|
|action|action|When set, executes client side action when clicked. See GumpActions for more information|none|action="disconnect"|
|param, param[2-5]|string|Parameters for action|none|param="foo", param3="bar"|
|text|string|The text to display on this button|none|text="Foo"|
|font|string|The font to use for this button|Arial|font="unifont1" or font="Tahoma"|
|font-height|int|The height of the font|0 |font-height="12"|
|font-align|enum(left, right, center)|The alignment of the text|center|font-align="right"|

The width and height of the button are adjusted to the texture size if the attributes are left out.

The attributes buttonid, page and action are mutually exclusive. action takes precedence over buttonid and page, if more than one is given.

font-color and font-hue are mutually exclusive. As always, hue has higher priority.

The text and font properties are not mandatory. If no text is given, only the button images are displayed. Thus, the button component is used both for the default UO buttons as well as custom labelled buttons.

### button images ###
The button tag can include 3 tags: "normal", "mouseover" and "mousedown". They define the image to use in this state, and can have the following attributes in the same way as the ["image" tag](#Image.md):
  * source
  * id
  * color
  * hue
  * alpha
  * tiled

Additionally, they have the following font and text related attributes:
| **Entry** | **[Type](#Attribute_Types.md)** | **Description** | **Default** | **Example** |
|:----------|:--------------------------------|:----------------|:------------|:------------|
|text|string|The text to display in this state|none|text="Foo"|
|font-color|color|The color to use for the font|#000000 (black)|font-color="#ff0000"|
|font-hue|int|The hue to use for the font|hue="13"|font-hue="13"|

If a text is given as an attribute to the button tag, this text is used for all states. The text attribute inside the state tags (normal, mouseover, mousedown) can overwrite this text for the given state.

### button examples ###
```
<button x="100" y="20" action="disconnect">
    <normal source="gumpart" id="2021" />
    <mouseover source="gumpart" id="2023" />
    <mousedown source="gumpart" id="2022" />
</button>

<button x="20" y="40" width="100" height="30" text="click me!" page="2">
    <normal source="theme" id="Images/ButtonNormal.png" />
    <mouseover source="theme" id="Images/ButtonHot.png" color="#ff0000" />
    <mousedown source="theme" id="Images/ButtonPressed.png" font-color="#0000ff" text="down" />
</button>
```

## checkbox ##
A checkbox is a component that can take one of two states (checked/unchecked), and thus represents binary decisions.
| **Attribute** | **[Type](#Attribute_Types.md)** | **Description** | **Default** | **Example** |
|:--------------|:--------------------------------|:----------------|:------------|:------------|
|switchid|int|ID of this checkbox, used to identify on the server|0 |switchid="13"|
|checked|bool|If this checkbox is checked at the beginning|0 |checked="1"|

The width and height of the checkbox are adjusted to the texture size if the attributes are left out.

### images inside the checkbox tag ###
The checkbox tag can include 4 tags: "checked", "checked-mouseover", "unchecked" and "unchecked-mouseover". They define the image to use in this state, and can have the following attributes in the same way as the ["image" tag](#Image.md):
  * source
  * id
  * color
  * hue
  * alpha
  * tiled

### checkbox example ###
```
<checkbox x="300" y="200" switchid="23" checked="1">
    <unchecked source="gumpart" id="210" />
    <unchecked-mouseover source="gumpart" id="210" hue="13" alpha="0.5" />
    <checked source="gumpart" id="211" />
    <checked-mouseover source="gumpart" id="211" color="#ff0000" />
</checkbox>
```

## radiobutton ##
A radiobutton is much like a checkbox, but always appears in groups, of which only one radiobutton can be checked. Thus, they can be used to choose between multiple options.

The radiobutton shares all attributes and image tags of a checkbox. It only has one extra attribute that defines the group:

| **Attribute** | **[Type](#Attribute_Types.md)** | **Description** | **Default** | **Example** |
|:--------------|:--------------------------------|:----------------|:------------|:------------|
|group|int|ID of the radiobutton group|0 |group="13"|

```
<radiobutton x="170" y="65" group="1" >
    <unchecked source="gumpart" id="210" />
    <unchecked-mouseover source="gumpart" id="210" hue="13" alpha="0.5" />
    <checked source="gumpart" id="211" />
    <checked-mouseover source="gumpart" id="211" color="#ff0000" />
</radiobutton>
<radiobutton x="210" y="65" group="1" checked="1">
    <unchecked source="gumpart" id="210" />
    <unchecked-mouseover source="gumpart" id="210" hue="13" alpha="0.5" />
    <checked source="gumpart" id="211" />
    <checked-mouseover source="gumpart" id="211" color="#ff0000" />
</radiobutton>
<radiobutton x="250" y="65" group="1" >
    <unchecked source="gumpart" id="210" />
    <unchecked-mouseover source="gumpart" id="210" hue="13" alpha="0.5" />
    <checked source="gumpart" id="211" />
    <checked-mouseover source="gumpart" id="211" color="#ff0000" />
</radiobutton>
```

## lineedit ##
A lineedit component enables the user to input text.
| **Attribute** | **[Type](#Attribute_Types.md)** | **Description** | **Default** | **Example** |
|:--------------|:--------------------------------|:----------------|:------------|:------------|
|text | string|Initial text|none|text="Here is text" |
|maxlength|int|Maximum number of characters allowed|unlimited |maxlength="12"|
|entryid|int|ID to identify this lineedit when the gump result is sent to the server|0 |entryid="7"|
|password|bool|Text is displayed as  |0 |password="1"|
|numeric|bool|Only numeric inputs allowed|0 |numeric="1"|
|font|string|The font to use for this lineedit|Arial|font="unifont1" or font="Tahoma"|
|font-height|int|The height of the font|0 |font-height="12"|
|action|action|An action to execute when the enter key is pressed See GumpActions for more information|none|action="sendspeech"|
|editaction|action|An action to execute whenever the text is edited. See GumpActions for more information|none|action="updatevendorgump"|

```
<lineedit x="120" y="105" width="215" height="25" password="1" action="connect" />
<lineedit x="120" y="105" width="215" height="25" maxlength="12" entryid="8" font="Tahoma" />
```

## scrollarea ##
A scrollarea component holds several other components. It can hold components of arbitrary size, and provides scrollbars if necessary.

The scrollbar holds only a additional few attributes, the most important parts are its inner tags: "vscroll", "hscroll" and "content" (see below).

| **Attribute** | **[Type](#Attribute_Types.md)** | **Description** | **Default** | **Example** |
|:--------------|:--------------------------------|:----------------|:------------|:------------|
|marginleft |int|How much margin should be added on the left side|0 |marginleft="20" |
|marginright |int|How much margin should be added on the right side|0 |marginright="20" |
|margintop |int|How much margin should be added at the top|0 |margintop="20" |
|marginbottom | int|How much margin should be added at the bottom|0 |marginbottom="5" |

### Scrollbars ###
The scrollarea component can have two scrollbars, horizontal and vertical. The properties for the scrollbars are defined in the "vscroll" and "hscroll" tags, that have the following attributes:

| **Attribute** | **[Type](#Attribute_Types.md)** | **Description** | **Default** | **Example** |
|:--------------|:--------------------------------|:----------------|:------------|:------------|
|visible|bool|Wheter or not the scrollbar should be visible|0 |visible="1"|
|width|int|Vertical scrollbars need to define their width, but not their height|0 |width="20"|
|height|int|Horizontal scrollbars need to define their height, but not their width|0 |height="20"|

#### Scrollbar images ####
Each scrollbar is composed of several images: the track, increment and decrement buttons and the thumb. For each of those, you have to define one or more textures. The easiest way to explain this is with an example:
```
<scrollarea x="10" y="10" width="150" height="150">
    <vscroll width="15" visible="always">
        <thumb>
            <normal source="gumpart" id="254" color="#ff0000" />
            <mouseover source="gumpart" id="254" />
            <mousedown source="gumpart" id="254" />
        </thumb>
        <increment>
            <normal source="gumpart" id="253" />
            <mouseover source="gumpart" id="253" hue="13" />
            <mousedown source="gumpart" id="252" />
        </increment>
        <decrement>
            <normal source="gumpart" id="251" />
            <mouseover source="gumpart" id="251" />
            <mousedown source="gumpart" id="250" />
        </decrement>
        <track source="gumpart" id="255" />
    </vscroll>
    <hscroll visible="never" />
    <content>
        <!-- add other components here -->
    </content>
</scrollarea>
```

As you can see, there are lots of textures involved, all defined using the already known _source_, _id_, _color_ and _hue_ attributes. Namely:
  * track: Requires only one texture
  * thumb: Requires 3 textures for normal, mouseover and mousedown
  * increment: Requires 3 textures for normal, mouseover and mousedown
  * decrement: Requires 3 textures for normal, mouseover and mousedown


### Content ###
Additionally to the scrollbars, the scrollarea component has a third child tag, "content". It holds the child components contained in this scrollarea.

# Repeat #
In some gumps, you will see tags named "repeat". This does not represent one component, but is used for a list of similar items, that is given at runtime. The character list presented at login is a good example - there is no way to know in advance which characters will be displayed to the user.

The repeat tag repeats its child tags for every of these list entries (e.g. for every char in the character list). The x and y coordinates of the components are increased for every new item, and certain keywords are replaced with information from the current list entry. For example, the string "charactername" is replaced with the name of character N in the character selection gump.

Repeat tags do not have the common attributes described above. They also do not have the _template_ attribute.

| **Attribute** | **[Type](#Attribute_Types.md)** | **Description** | **Default** | **Example** |
|:--------------|:--------------------------------|:----------------|:------------|:------------|
|name|string|Identifies the repeat context by name|none|name="characterlist"|
|xincrease|int|Increase x by this value for the next iteration|0 |xincrease="10"|
|yincrease|int|Increase y by this value for the next iteration|0 |yincrease="10"|
|xlimit|int|How many entries should be displayed in a row|1 |xlimit="2"|
|ylimit|int|How many entries should be displayed in a column|infinite|ylimit="5"|

This might look a bit confusing at first sight, but the principle is very easy. Imagine a simple table layout with xlimit columns and ylimit rows. Every time the current x or y index overflows these boundaries, all component coordinates are increased by xincrease and yincrease, respectively. Then the next iteration is rendered.

Repeat contexts only appear in specific places, and offer only specific properties that can be replaced. If they are used in gumps, the properties are described in the gump. Let's take a look at the server list gump:
```
<repeat name="serverlist" yincrease="28" xlimit="1">
    <!--
    Each entry represents a server. Properties:
    - serverindex: An ID number, used to identify the selected server in selection packet
    - servername: The name of the server, as given in the server list packet
    -->
    <button x="0" y="0" width="190" height="25" name="selectserver" action="selectserver" param="serverindex" text="servername" />
</repeat>
```

If fluorescence receives a packet containing the servers (1/Homeshard) and (2/Test Center), this will expand to
```
<button x="0" y="0" width="190" height="25" name="selectserver" action="selectserver" param="1" text="Homeshard" />
<button x="0" y="28" width="190" height="25" name="selectserver" action="selectserver" param="2" text="Test Center" />
```
As you can see, the y coordinate of the second button is increased automatically by the given yincrease value. The x value is the same for both buttons, because xlimit is set to 1.

## Conditions ##
The repeat context also supports conditional inclusion on child tags. The _repeatif_ tag checks a condition before deciding if the child tags should be included for the current entry.

| **Attribute** | **[Type](#Attribute_Types.md)** | **Description** | **Default** | **Example** |
|:--------------|:--------------------------------|:----------------|:------------|:------------|
|xindex|int|Checks if the x index has the given value|- |xindex="1"|
|yindex|int|Checks if the y index has the given value|- |yindex="1"|
|test|int|Checks if the given value is 1|- |test="0"|

This is useful to include some gump components only on special occasions. The example is taken from the skills gump, where usable skills should have a button to use them:
```
<repeat name="skills" yincrease="22" xlimit="1">
    <repeatif test="skillusable">
        <tbutton x="1" y="1" width="20" height="20" text="X" action="useskill" param="skillid" />
    </repeatif>
    <tlabel x="25" y="1" width="180" height="20" text="skillname" />
</repeat>
```
skillusable is a property of the repeat context, either 0 or 1.

# Special components #
Additionally to the above components, there are a few specialized components that are only used in very rare cases.

  * worldview: Renders the world. Only accepts x/y/width/height attributes
  * paperdoll: Renders the paperdoll. Only accepts x/y/width/height attributes
  * container: Renders an open container. Only accepts x/y/width/height attributes
  * warmodebutton: A special button that is sensitive to warmode changes. Aside from x/y/width/height, it has six child tags _normal_, _mouseover_, _mousedown_, _warmode-normal_, _warmode-mouseover_, _warmode-mousedown_ defining one texture each
  * sysloglabel: A special label showing the system messages. Shares most properties with label, except _text_, _cliloc_ and 