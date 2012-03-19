
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
    ui/doubleclickhandler.hpp
    ui/gumpmenus.hpp
    ui/gumpactions.hpp
    ui/fontengine.hpp
    ui/uofont.hpp
    ui/uofontprovider.hpp
    ui/cliprectmanager.hpp
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
    ui/doubleclickhandler.cpp
    ui/gumpmenus.cpp
    ui/gumpactions.cpp
    ui/fontengine.cpp
    ui/uofont.cpp
    ui/uofontprovider.cpp
    ui/cliprectmanager.cpp
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
    ui/components/gumpview.hpp
    ui/components/containerview.hpp
    ui/components/image.hpp
    ui/components/background.hpp
    ui/components/uocheckbox.hpp
    ui/components/textentry.hpp
    ui/components/uoradiobutton.hpp
    ui/components/sysloglabel.hpp
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
    ui/components/gumpview.cpp
    ui/components/containerview.cpp
    ui/components/image.cpp
    ui/components/background.cpp
    ui/components/uocheckbox.cpp
    ui/components/textentry.cpp
    ui/components/uoradiobutton.cpp
    ui/components/sysloglabel.cpp
    )

set (RENDER_HPP
    ui/render/renderqueue.hpp
    ui/render/ingameobjectrenderer.hpp
    ui/render/worldrenderdata.hpp
    ui/render/worldviewrenderer.hpp
    ui/render/gumprenderer.hpp
    ui/render/gumprenderqueue.hpp
    ui/render/containerrenderer.hpp
    ui/render/containerrenderqueue.hpp
    ui/render/shadermanager.hpp
    )

set (RENDER_CPP
    ui/render/renderqueue.cpp
    ui/render/ingameobjectrenderer.cpp
    ui/render/worldrenderdata.cpp
    ui/render/worldviewrenderer.cpp
    ui/render/gumprenderer.cpp
    ui/render/gumprenderqueue.cpp
    ui/render/containerrenderer.cpp
    ui/render/containerrenderqueue.cpp
    ui/render/shadermanager.cpp
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
    ui/particles/emittable.hpp
    ui/particles/emitter.hpp
    ui/particles/particle.hpp
    ui/particles/particleemitter.hpp
    ui/particles/emitteremitter.hpp
    ui/particles/startpositionprovider.hpp
    ui/particles/motionmodel.hpp
    ui/particles/xmlloader.hpp
    )

set (PARTICLES_CPP
    ui/particles/emittable.cpp
    ui/particles/emitter.cpp
    ui/particles/particle.cpp
    ui/particles/particleemitter.cpp
    ui/particles/emitteremitter.cpp
    ui/particles/startpositionprovider.cpp
    ui/particles/motionmodel.cpp
    ui/particles/xmlloader.cpp
    )


SET(UI_FILES ${UI_HPP} ${UI_CPP} ${GUMPMENU_HPP} ${GUMPMENU_CPP} ${RENDER_HPP} ${RENDER_CPP} ${TARGETING_HPP} ${TARGETING_CPP} ${PARTICLES_HPP} ${PARTICLES_CPP} PARENT_SCOPE)

