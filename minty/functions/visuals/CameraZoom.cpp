﻿#include "CameraZoom.h"

namespace cheat {
	static void SCameraModuleInitialize_SetWarningLocateRatio_Hook(app::SCameraModuleInitialize* __this, double deltaTime, app::CameraShareData* data);

	CameraZoom::CameraZoom() {
		f_Enabled = config::getValue("functions:CameraZoom", "enabled", false);
		f_CameraZoom = config::getValue("functions:CameraZoom", "cameraZoom", 1.0f);

		f_Hotkey = Hotkey("functions:CameraZoom");

		HookManager::install(app::MoleMole_SCameraModuleInitialize_SetWarningLocateRatio, SCameraModuleInitialize_SetWarningLocateRatio_Hook);
	}

	CameraZoom& CameraZoom::getInstance() {
		static CameraZoom instance;
		return instance;
	}

	void CameraZoom::GUI() {
		ConfigCheckbox(_("CAMERA_ZOOM_TITLE"), f_Enabled, _("CAMERA_ZOOM_DESCRIPTION"));

		if (f_Enabled.getValue()) {
			ImGui::Indent();
			ConfigSliderFloat(_("ZOOM_TITLE"), f_CameraZoom, 0.1f, 500.0f);
			f_Hotkey.Draw();
			ImGui::Unindent();
		}
	}

	void CameraZoom::Outer() {
		if (f_Hotkey.IsPressed())
			f_Enabled.setValue(!f_Enabled.getValue());
	}

	void CameraZoom::Status() {
		if (f_Enabled.getValue())
			ImGui::Text("%s (%.1f)", _("CAMERA_ZOOM_TITLE"), f_CameraZoom);
	}

	std::string CameraZoom::getModule() {
		return _("MODULE_VISUALS");
	}

	void SCameraModuleInitialize_SetWarningLocateRatio_Hook(app::SCameraModuleInitialize* __this, double deltaTime, app::CameraShareData* data) {
		auto& cameraZoom = CameraZoom::getInstance();

		data->currentWarningLocateRatio = static_cast<double>(cameraZoom.f_Enabled.getValue() ? cameraZoom.f_CameraZoom.getValue() : 1.0f);
		CALL_ORIGIN(SCameraModuleInitialize_SetWarningLocateRatio_Hook, __this, deltaTime, data);
	}
}
