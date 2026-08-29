#pragma once
#include "Components/Palette/Palette.hpp"

namespace BuildingsEditor {
	class Palette : public Components::Palette {
	public:

		Palette();
		~Palette();

		virtual void addTools();
		virtual void loadAll(ObjectType type);
		virtual void draw();

	};

}