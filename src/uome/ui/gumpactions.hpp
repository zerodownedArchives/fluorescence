#ifndef UOME_UI_GUMPACTIONS_HPP
#define UOME_UI_GUMPACTIONS_HPP

#include <ui/components/localbutton.hpp>
#include <ui/gumpmenu.hpp>

#include <boost/function.hpp>
#include <map>

namespace uome {
namespace ui {

typedef boost::function<bool (GumpMenu*, const UnicodeString&, unsigned int, const UnicodeString*)> GumpActionCallback;

class GumpActions {
public:
    static const unsigned int MAX_PARAMETER_COUNT = 5;

    static void buildBasicActionTable();
    static void buildFullActionTable();

    static void doAction(GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters);
    static void doAction(components::LocalButton* button);

private:

    struct GumpAction {
        GumpAction();

        GumpAction(bool closeGumpMenu, GumpActionCallback callback);

        bool closeGumpMenu_;
        GumpActionCallback callback_;
    };

    static std::map<UnicodeString, GumpAction> actionTable_;

    static bool closeHelper(GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters);
    static bool selectShardFirst(GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters);
    static bool selectServerFirst(GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters);
    static bool selectCharacterFirst(GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters);

    static bool sendSpeech(GumpMenu* menu, const UnicodeString& action, unsigned int parameterCount, const UnicodeString* parameters);
};

}
}

#endif
