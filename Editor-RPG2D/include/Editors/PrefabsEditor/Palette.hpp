#pragma once
#include "Components/Palette/Palette.hpp"

namespace PrefabsEditor {
	class Palette : public Components::Palette {
	public:

		Palette();
		~Palette();

		virtual void loadAll(ObjectType type);
	};

}