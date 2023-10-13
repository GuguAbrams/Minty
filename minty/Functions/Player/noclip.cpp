#include "noclip.h"

namespace cheat {
	app::Vector3 prevPos, newPos;
	app::Vector3 posCheck;

	void GameManager_Update_Hook(app::GameManager* __this);
	void HumanoidMoveFSM_LateTick_Hook(app::HumanoidMoveFSM* __this, float deltaTime, app::MethodInfo* method);

	Noclip::Noclip() {
		f_NoclipSpeed = 5;
		f_NoclipAltSpeed = 10;

		HookManager::install(app::GameManager_Update, GameManager_Update_Hook);
		HookManager::install(app::MoleMole_HumanoidMoveFSM_LateTick, HumanoidMoveFSM_LateTick_Hook);
	}

	void Noclip::GUI() {
		ImGui::Checkbox("Noclip", &ifNoclip);
		if (ifNoclip) {
			ImGui::Indent();
			ImGui::SliderFloat("Noclip speed", &f_NoclipSpeed, 1, 50);
			ImGui::Checkbox("Alt speed", &ifAltSpeed);
			if (ifAltSpeed) {
				ImGui::Indent();
				ImGui::SliderFloat("Alt Speed (HOLD HOTKEY)", &f_NoclipAltSpeed, 1, 50);
				noClipAltHotkey.Draw();
				ImGui::Unindent();
			}
			noClipHotkey.Draw();
			ImGui::Unindent();
		}
	}

	void Noclip::Outer() {
		if (noClipHotkey.IsPressed()) {
			ifNoclip = !ifNoclip;
		}
		if (ifAltSpeed && noClipAltHotkey.IsDown()) {
			f_finalSpeed = f_NoclipAltSpeed;
		}
		else {
			f_finalSpeed = f_NoclipSpeed;
		}
	}

	void Noclip::Status() {
		if (ifNoclip) {
			ImGui::Text(_("Noclip (%.1f U/s | %.1f U/s "), f_NoclipSpeed, f_NoclipAltSpeed);
		}
	}

	void OnNoclip() {
		static std::string ActiveHero;

		app::Rigidbody* rigidbody = nullptr;
		app::GameObject* AvatarRoot = nullptr;
		app::GameObject* nameAvatar = nullptr;
		app::Transform* avatarTransform = nullptr;
		app::GameObject* HeroGameObject = nullptr;

		AvatarRoot = app::UnityEngine__GameObject__Find(string_to_il2cppi("/EntityRoot/AvatarRoot"));
		//util::log(M_Info, "found avatar");
		if (!AvatarRoot) return;
		auto Transform = app::UnityEngine_GameObject_GetComponent(AvatarRoot, string_to_il2cppi("Transform"));
		//util::log(M_Info, "found transform");
		auto HeroCount = app::UnityEngine_Transform_GetChildCount(reinterpret_cast<app::Transform*>(Transform));
		//util::log(M_Info, "found count");
		for (int i = 0; i <= HeroCount - 1; i++)
		{
			auto HeroComponent = app::Transform_GetChild(reinterpret_cast<app::Transform*>(Transform), i);
			//util::log(M_Info, "found child");
			HeroGameObject = app::Component_1_get_gameObject(reinterpret_cast<app::Component_1*>(HeroComponent));
			//util::log(M_Info, "found gameobj");
			auto isActiveHero = app::GameObject_get_active(HeroGameObject);
			//util::log(M_Info, "found active");
			if (isActiveHero)
			{
				auto GameObjectName = app::Object_1_get_name(reinterpret_cast<app::Object_1*>(HeroGameObject));
				//util::log(M_Info, "found name");
				ActiveHero = il2cppi_to_string(GameObjectName);
				std::string Hero = ActiveHero.erase(ActiveHero.find("(Clone)"));
				std::string avatarNamestring = "/EntityRoot/AvatarRoot/" + il2cppi_to_string(GameObjectName) + "/OffsetDummy/" + Hero.c_str();
				nameAvatar = app::UnityEngine__GameObject__Find(string_to_il2cppi(avatarNamestring.c_str()));
				//util::log(M_Info, "found gameob2");
				avatarTransform = app::UnityEngine__Component__get__Transform(nameAvatar);
				//util::log(M_Info, "found transfor2");
				rigidbody = reinterpret_cast<app::Rigidbody*>(app::UnityEngine_GameObject_GetComponent(HeroGameObject, string_to_il2cppi("Rigidbody")));
				//util::log(M_Info, "found rb");
				if (rigidbody != 0) break;
			}
		}

		app::Rigidbody_set_collisionDetectionMode(rigidbody, app::CollisionDetectionMode__Enum::Continuous);
		//util::log(M_Info, "coli det")

		auto cameraEntity = reinterpret_cast<app::BaseEntity*>(app::UnityEngine__Component__get__Transform(app::UnityEngine__GameObject__Find(string_to_il2cppi("/EntityRoot/MainCamera(Clone)(Clone)"))));
		//util::log(M_Info, "found transfom cam");
		auto avatarEntity = reinterpret_cast<app::BaseEntity*>(avatarTransform);

		app::Vector3 dir = {};

		if (ImGui::IsKeyDown(ImGuiKey_W))
			dir = dir + app::MoleMole_BaseEntity_GetForward(cameraEntity);

		if (ImGui::IsKeyDown(ImGuiKey_S))
			dir = dir - app::MoleMole_BaseEntity_GetForward(cameraEntity);

		if (ImGui::IsKeyDown(ImGuiKey_D))
			dir = dir + app::MoleMole_BaseEntity_GetRight(cameraEntity);

		if (ImGui::IsKeyDown(ImGuiKey_A))
			dir = dir - app::MoleMole_BaseEntity_GetRight(cameraEntity);

		if (ImGui::IsKeyDown(ImGuiKey_Space))
			dir = dir + app::MoleMole_BaseEntity_GetUp(avatarEntity);

		if (ImGui::IsKeyDown(ImGuiKey_ModShift))
			dir = dir - app::MoleMole_BaseEntity_GetUp(avatarEntity);

		prevPos = app::Rigidbody_get_position(rigidbody);
		//util::log(M_Info, "rb go pos");
		if (prevPos.x == 0 && prevPos.y == 0 && prevPos.z == 0)
			return;

		float deltaTime = app::Time_get_deltaTime() * 1.5F;
		//util::log(M_Info, "got delt");
		newPos = prevPos + dir * Noclip::f_finalSpeed * deltaTime;
		// if (iNoClipMode == 0) 
		app::Rigidbody_set_velocity(rigidbody, { 0,0,0 });
		//util::log(M_Info, "set vel");
		app::Rigidbody_MovePosition(rigidbody, newPos);
		//util::log(M_Info, "mov pos");
		//app::Rigidbody_set_position(rigidbody, newPos);
	}

	void GameManager_Update_Hook(app::GameManager* __this)
	{
		__try {
			if (Noclip::ifNoclip) OnNoclip();
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {
			util::log(M_Error, "Exception 0x%08x.", _exception_code());
		}

		CALL_ORIGIN(GameManager_Update_Hook, __this);
	}

	void HumanoidMoveFSM_LateTick_Hook(app::HumanoidMoveFSM* __this, float deltaTime, app::MethodInfo* method)
	{
		if (Noclip::ifNoclip) {
			if (app::Vector3_Distance(posCheck, newPos) > 3.f) {
				posCheck = newPos;
			}
			else {
				return;
			}
		}

		CALL_ORIGIN(HumanoidMoveFSM_LateTick_Hook, __this, deltaTime, method);
	}
}