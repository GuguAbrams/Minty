﻿#include "CutsceneSkip.h"

namespace cheat {
    static void CriwareMediaPlayer_Update(app::CriwareMediaPlayer* __this, app::MethodInfo* method);

    CutsceneSkip::CutsceneSkip() {
        f_Enabled = config::getValue("functions:CutsceneSkip", "enabled", false);

        f_Hotkey = Hotkey("functions:CutsceneSkip");

        HookManager::install(app::CriwareMediaPlayer_Update, CriwareMediaPlayer_Update);
    }

    CutsceneSkip& CutsceneSkip::getInstance() {
        static CutsceneSkip instance;
        return instance;
    }

    void CutsceneSkip::GUI() {
        ConfigCheckbox(_("SKIP_CUTSCENE_TITLE"), f_Enabled, _("SKIP_CUTSCENE_DESCRIPTION"));

        if (f_Enabled.getValue()) {
            ImGui::Indent();
            f_Hotkey.Draw();
            ImGui::Unindent();
        }
    }

    void CutsceneSkip::Status() {
        if (f_Enabled.getValue())
            ImGui::Text(_("SKIP_CUTSCENE_TITLE"));
    }

    void CutsceneSkip::Outer() {
        if (f_Hotkey.IsPressed())
            f_Enabled.setValue(!f_Enabled.getValue());
    }

    std::string CutsceneSkip::getModule() {
        return _("MODULE_WORLD");
    }

    void CriwareMediaPlayer_Update(app::CriwareMediaPlayer* __this, app::MethodInfo* method) {
        auto& cutsceneSkip = CutsceneSkip::getInstance();

        if (cutsceneSkip.f_Enabled.getValue())
            app::CriwareMediaPlayer_Skip(__this, method);
        return CALL_ORIGIN(CriwareMediaPlayer_Update, __this, method);
    }
}
