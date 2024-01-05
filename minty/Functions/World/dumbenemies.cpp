﻿#include "DumbEnemies.h"

namespace cheat {
	static void VCMonsterAIController_TryDoSkill_Hook(void* __this, uint32_t skillID);

	DumbEnemies::DumbEnemies() {
		f_Enabled = config::getValue("functions:DumbEnemies", "enabled", false);

		f_Hotkey = Hotkey("functions:DumbEmenies");

		HookManager::install(app::MoleMole_VCMonsterAIController_TryDoSkill, VCMonsterAIController_TryDoSkill_Hook);
	}

	DumbEnemies& DumbEnemies::getInstance() {
		static DumbEnemies instance;
		return instance;
	}

	void DumbEnemies::GUI() {
		ConfigCheckbox(_("DUMB_ENEMIES_TITLE"), f_Enabled, _("DUMB_ENEMIES_DESCRIPTION"));

		if (f_Enabled.getValue()) {
			ImGui::Indent();
			f_Hotkey.Draw();
			ImGui::Unindent();
		}
	}

	void DumbEnemies::Status() {
		if (f_Enabled.getValue())
			ImGui::Text(_("DUMB_ENEMIES_TITLE"));
	}

	void DumbEnemies::Outer() {
		if (f_Hotkey.IsPressed())
			f_Enabled.setValue(!f_Enabled.getValue());
	}

	std::string DumbEnemies::getModule() {
		return _("MODULE_WORLD");
	}

	void VCMonsterAIController_TryDoSkill_Hook(void* __this, uint32_t skillID) {
		auto& dumbEnemies = DumbEnemies::getInstance();

		if (dumbEnemies.f_Enabled.getValue() && skillID != 101)
			return;

		CALL_ORIGIN(VCMonsterAIController_TryDoSkill_Hook, __this, skillID);
	}
}
