
set (UI_HPP
    ui/manager.hpp
    ui/texture.hpp
    ui/animation.hpp
    ui/textureprovider.hpp
    ui/singletextureprovider.hpp
    ui/animdatatextureprovider.hpp
    ui/animtextureprovider.hpp
    ui/bitmask.hpp
    ui/cursormanager.hpp
    ui/cursorimage.hpp
    ui/gumpmenus.hpp
    ui/gumpactions.hpp
    ui/fontengine.hpp
    ui/uofont.hpp
    ui/uofontprovider.hpp
    ui/cliprectmanager.hpp
    ui/audiomanager.hpp
    ui/commandmanager.hpp
    ui/macromanager.hpp
    )

set (UI_CPP
    ui/manager.cpp
    ui/texture.cpp
    ui/animation.cpp
    ui/singletextureprovider.cpp
    ui/animdatatextureprovider.cpp
    ui/animtextureprovider.cpp
    ui/bitmask.cpp
    ui/cursormanager.cpp
    ui/cursorimage.cpp
    ui/gumpmenus.cpp
    ui/gumpactions.cpp
    ui/fontengine.cpp
    ui/uofont.cpp
    ui/uofontprovider.cpp
    ui/cliprectmanager.cpp
    ui/audiomanager.cpp
    ui/commandmanager.cpp
    ui/macromanager.cpp
    )

set (GUMPMENU_HPP
    ui/xmlparser.hpp
    ui/stringparser.hpp
    ui/gumpmenu.hpp
    ui/gumpelement.hpp

    ui/components/multitextureimage.hpp
    ui/components/basebutton.hpp
    ui/components/templatebutton.hpp
    ui/components/uobutton.hpp
    ui/components/scrollarea.hpp
    ui/components/lineedit.hpp
    ui/components/label.hpp
    ui/components/worldview.hpp
    ui/components/propertylabel.hpp
    ui/components/paperdollview.hpp
    ui/components/containerview.hpp
    ui/components/image.hpp
    ui/components/background.hpp
    ui/components/uocheckbox.hpp
    ui/components/textentry.hpp
    ui/components/uoradiobutton.hpp
    ui/components/sysloglabel.hpp
    ui/components/tbackground.hpp
    ui/components/warmodebutton.hpp
    ui/components/clicklabel.hpp
    )

set (GUMPMENU_CPP
    ui/xmlparser.cpp
    ui/stringparser.cpp
    ui/gumpmenu.cpp
    ui/gumpelement.cpp

    ui/components/basebutton.cpp
    ui/components/templatebutton.cpp
    ui/components/uobutton.cpp
    ui/components/scrollarea.cpp
    ui/components/lineedit.cpp
    ui/components/label.cpp
    ui/components/worldview.cpp
    ui/components/propertylabel.cpp
    ui/components/paperdollview.cpp
    ui/components/containerview.cpp
    ui/components/image.cpp
    ui/components/background.cpp
    ui/components/uocheckbox.cpp
    ui/components/textentry.cpp
    ui/components/uoradiobutton.cpp
    ui/components/sysloglabel.cpp
    ui/components/tbackground.cpp
    ui/components/warmodebutton.cpp
    ui/components/clicklabel.cpp
    )

set (RENDER_HPP
    ui/render/renderqueue.hpp
    ui/render/worldrenderdata.hpp
    ui/render/worldrenderer.hpp
    ui/render/paperdollrenderer.hpp
    ui/render/paperdollrenderqueue.hpp
    ui/render/containerrenderer.hpp
    ui/render/containerrenderqueue.hpp
    ui/render/shadermanager.hpp
    ui/render/material.hpp
    )

set (RENDER_CPP
    ui/render/renderqueue.cpp
    ui/render/worldrenderdata.cpp
    ui/render/worldrenderer.cpp
    ui/render/paperdollrenderer.cpp
    ui/render/paperdollrenderqueue.cpp
    ui/render/containerrenderer.cpp
    ui/render/containerrenderqueue.cpp
    ui/render/shadermanager.cpp
    ui/render/material.cpp
    )

set (TARGETING_HPP
    ui/targeting/target.hpp
    ui/targeting/servertarget.hpp
    )

set (TARGETING_CPP
    ui/targeting/target.cpp
    ui/targeting/servertarget.cpp
    )

set (PARTICLES_HPP
    ui/particles/emitter.hpp
    ui/particles/particle.hpp
    ui/particles/particleemitter.hpp
    ui/particles/startlocationprovider.hpp
    ui/particles/motionmodel.hpp
    ui/particles/xmlloader.hpp
    ui/particles/particleemitterstate.hpp
    ui/particles/timeline.hpp
    ui/particles/timelineelement.hpp
    ui/particles/timelinestatic.hpp
    ui/particles/timelinepause.hpp
    ui/particles/timelineimmediate.hpp
    ui/particles/timelineblend.hpp
    ui/particles/timelineinterpolate.hpp
    ui/particles/timelineforever.hpp
    ui/particles/timelineevent.hpp
    )

set (PARTICLES_CPP
    ui/particles/emitter.cpp
    ui/particles/particleemitter.cpp
    ui/particles/startlocationprovider.cpp
    ui/particles/motionmodel.cpp
    ui/particles/xmlloader.cpp
    ui/particles/particleemitterstate.cpp
    ui/particles/timeline.cpp
    ui/particles/timelinestatic.cpp
    ui/particles/timelinepause.cpp
    ui/particles/timelineimmediate.cpp
    ui/particles/timelineblend.cpp
    ui/particles/timelineinterpolate.cpp
    ui/particles/timelineforever.cpp
    ui/particles/timelineevent.cpp
    )

set (CLIENTCOMMANDS_HPP
    ui/commands/clientcommand.hpp
    ui/commands/say.hpp
    ui/commands/emote.hpp
    ui/commands/yell.hpp
    ui/commands/whisper.hpp
    ui/commands/command.hpp
    ui/commands/opengump.hpp
    ui/commands/disconnect.hpp
    ui/commands/effect.hpp
    ui/commands/osieffect.hpp
    ui/commands/togglewarmode.hpp
    ui/commands/directionlight.hpp
    ui/commands/speechentry.hpp
    ui/commands/zoom.hpp
    ui/commands/property.hpp
    )

set (CLIENTCOMMANDS_CPP
    ui/commands/say.cpp
    ui/commands/emote.cpp
    ui/commands/yell.cpp
    ui/commands/whisper.cpp
    ui/commands/command.cpp
    ui/commands/opengump.cpp
    ui/commands/disconnect.cpp
    ui/commands/effect.cpp
    ui/commands/osieffect.cpp
    ui/commands/togglewarmode.cpp
    ui/commands/directionlight.cpp
    ui/commands/speechentry.cpp
    ui/commands/zoom.cpp
    ui/commands/property.cpp
    )


SET(UI_FILES ${UI_HPP} ${UI_CPP} ${GUMPMENU_HPP} ${GUMPMENU_CPP} ${RENDER_HPP} ${RENDER_CPP} ${TARGETING_HPP} ${TARGETING_CPP}
        ${PARTICLES_HPP} ${PARTICLES_CPP} ${CLIENTCOMMANDS_HPP} ${CLIENTCOMMANDS_CPP} PARENT_SCOPE)
