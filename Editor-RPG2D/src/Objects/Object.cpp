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
		case ObjectType::Wall:
			return L"Wall";
		case ObjectType::Floor:
			return L"Floor";
		case ObjectType::Door:
			return L"Door";
		case ObjectType::Window:
			return L"Window";
		case ObjectType::WallMounted:
			return L"WallMounted";
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
	else if(type == L"Wall")
		return ObjectType::Wall;
	else if(type == L"Floor")
		return ObjectType::Floor;
	else if(type == L"Door")
		return ObjectType::Door;
	else if(type == L"Window")
		return ObjectType::Window;
	else if (type == L"WallMounted")
		return ObjectType::WallMounted;
	else
		return ObjectType::None;
}