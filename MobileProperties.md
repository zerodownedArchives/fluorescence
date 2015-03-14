Mobile properties allow you to create gumps that change the displayed textual information based on server updates without resending the whole gump. Some examples for gumps utilizing mobile properties are the paperdoll (name) or the stats window (current/max hits, mana, ...)

Currently, fluorescence only provides several predefined properties that you can use in your gumps. A future version will implement a custom property update packet, allowing you to e.g. use additional stats in the stats display.

The available properties are:

# Stats and Combat #
  * strength
  * dexterity
  * intelligence
  * statcap

  * hitpoints
  * hitpoints-max
  * mana
  * mana-max
  * stamina
  * stamina-max

  * armor-rating
  * damage-min
  * damage-max
  * resist-cold
  * resist-energy
  * resist-fire
  * resist-physical
  * resist-poison

# Misc #
  * followers
  * followers-max
  * gold
  * luck
  * name
  * paperdoll-name
  * tithing-points
  * weight
  * weight-max

# Skills #
For all skills:
  * skills.<skill name from skills.mul>.base
  * skills.<skill name from skills.mul>.cap
  * skills.<skill name from skills.mul>.lock
  * skills.<skill name from skills.mul>.value