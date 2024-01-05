﻿#pragma once

#include "../utils/util.h"
#include "../il2cpp/il2cpp-appdata.h"
#include "../il2cpp/il2cppUtils.h"

namespace cheat::game {
	class Entity {
	public:
		Entity(app::BaseEntity* rawEntity);

		//std::string& name();
		app::BaseEntity* raw();
		uint32_t runtimeID();
		uint32_t configID();
		app::EntityType__Enum_1 type();

		app::Vector3 relativePosition() const;

		bool isGadget() const;
		bool isAvatar() const;
		bool isChest();
		//bool isItem();

		bool isLoaded() const;

		void setRelativePosition(const app::Vector3& value);
		app::GameObject* gameObject();

		app::Animator* animator();
		app::Rigidbody* rigidbody();

		float distance(Entity* entity);
		float distance(app::BaseEntity* rawEntity);
		float distance(const app::Vector3& point);
		float distance(const app::Vector2& levelPoint);
		app::Vector3 absolutePosition();
		app::Vector2 levelPosition();
		app::LCBaseCombat* combat();
		std::string& name();

		app::Vector3 forward() const;
		app::Vector3 back() const;
		app::Vector3 right() const;
		app::Vector3 left() const;
		app::Vector3 up() const;
		app::Vector3 down() const;

		app::GameObject* gameObject();
	private:
		app::BaseEntity* m_RawEntity;
		bool m_HasName;
		std::string m_Name;

		mutable bool m_IsLoaded;
	};
}
