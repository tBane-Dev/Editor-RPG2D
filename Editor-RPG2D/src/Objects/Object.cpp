#pragma once
#include "Objects/Object.hpp"

Object::Object() {
	_type = ObjectType::None;
}

Object::~Object() {

}

std::wstring ObjectTypeToWString(ObjectType type) {
	switch (type) {

		case ObjectType::None:
			return L"";
		case ObjectType::Terrain:
			return L"Terrain";
		case ObjectType::Monster:
			return L"Monster";
		case ObjectType::Nature:
			return L"Nature";
		default:
			return L"Unknown";
	};
}

ObjectType WStringToObjectType(std::wstring type) {
	if (type == L"Terrain")
		return ObjectType::Terrain;
	else if (type == L"Monster")
		return ObjectType::Monster;
	else if (type == L"Nature")
		return ObjectType::Nature;
	else
		return ObjectType::None;
}