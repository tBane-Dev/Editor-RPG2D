#pragma once
#include <SFML/Graphics.hpp>

enum class ObjectType { None, Terrain, Monster, Nature };

class Object {
public:
	ObjectType _type = ObjectType::None;

	Object();
	virtual ~Object();
};

std::wstring ObjectTypeToWString(ObjectType type);
ObjectType WStringToObjectType(std::wstring type);