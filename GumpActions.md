As described in [XMLGumpMenus](XMLGumpMenus.md), some components can initiate actions, e.g. a button click. This page lists the available actions along with their parameters and a short description.

# General #

| **Action** | **Desription** | **Parameter 1**| **Parameter 2** | **Parameter 3**| **Parameter 4** | **Parameter X**|
|:-----------|:---------------|:---------------|:----------------|:---------------|:----------------|:---------------|
|close|Close the current gump|- |- |- |- |- |
|disconnect|Disconnect from the current server and return to login gump|- |- |- |- |- |
|opengump|Open a new gump menu with the given name|Gump name|- |- |- |- |
|shutdown|Close fluorescence|- |- |- |- |- |
|yesnogump|Show yes/no dialog before executing a given action|Question to display|Delayed action name|Delayed action param|Delayed action param2|etc|

# Pre-Login #
| **Action** | **Desription** | **Parameter 1**| **Parameter 2** | **Parameter 3**|
|:-----------|:---------------|:---------------|:----------------|:---------------|
|connect|Connect to the given server IP/port and login. Needs information from the login gump|- |- |- |
|createdummychar|Create a simple character with default values. Only makes sense in the char selection gump|- |- |- |
|createshard|Opens the shard creation dialog|- |- |- |
|deletecharacter|Delete a character. Only makes sense in the char selection gump|Char index (int)|Char password|- |
|selectcharacter|Select a character from the char list. Only makes sense in the char selection gump|Char index (int)|Char password|Char name|
|selectcharacter-first|Select first character in the list. See selectcharacter|- |- |- |
|selectserver|Select an entry from the server list. Do not confuse with shard selection, this is about game server selection. Only makes sense directly after the login|Server index (int)|- |- |
|selectserver-first|Select the first game server. See selectserver|- |- |- |
|selectshard|Selects a shard to initialize. Only useful in the very first gump|Shard name|- | -|
|selectshard-first|Select the first shard in the list. Only useful in the very first gump|- |- |- |

# In Game #
| **Action** | **Desription** | **Parameter 1**| **Parameter 2** | **Parameter 3**| **Parameter 4**| **Parameter 5**|
|:-----------|:---------------|:---------------|:----------------|:---------------|:---------------|:---------------|
|castspell|Cast spell|Spell ID (int)|- |- |- |- |
|helprequest|Send help request packet|- |- |- |- |- |
|openprofile|Open char profile|- |- |- |- |- |
|openskills|Open skills gump|- |- |- |- |- |
|openstatus|Open status gump|- |- |- |- |- |
|sendspeech|Send a speech request. Needs a lineedit named "speechtext" in the same gump|- |- |- |- |- |
|useskill|Use skill|Skill ID (int)|- |- |- |- |

# In Game - Context required #
These actions can also be performed in game, but they need a special context to make sense. For example, you can use a skill from any gump, but to send the selection for a context menu, you need information about the context menu.
| **Action** | **Desription** | **Parameter 1**| **Parameter 2** | **Parameter 3**| **Parameter 4**| **Parameter 5**|
|:-----------|:---------------|:---------------|:----------------|:---------------|:---------------|:---------------|
|contextmenureply|Reply after selecting a context menu entry|Menu serial (int)|Button ID (int)|- |- |- |
|pickerresponse|Send a response to an object picker packet (0x7c)|Serial (int)|Menu ID (int)|Answer ID (int)|Art ID (int)|Hue (int)|