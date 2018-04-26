#pragma once

#include <cstdio>
#include <stdlib.h>
#include <map>
#include <vector>
#include <set>
#include <assert.h>

struct TypeInfo {
	TypeInfo(std::string _name = "")
	{
		name = _name;
	}
	bool operator==(const TypeInfo& other) const
	{
		return name == other.name;
	}
	bool operator!=(const TypeInfo& other) const
	{
		return name != other.name;
	}
	std::string name;
	int GetHash()
	{
		return std::hash<std::string>()(name);
	}
};

static std::map<std::string, TypeInfo> typeInfoForClasses;
static std::map<std::string, std::set< std::string > > baseClasses;
static std::map<std::string, std::set< std::string > > derivedClasses;

struct registrator {
	registrator(std::string derived, std::string base, int offset)
	{
		if (baseClasses.find(derived) == baseClasses.end()) {
			baseClasses[derived] = std::set<std::string>();
			baseClasses[derived].insert(derived);
		}
		if (derivedClasses.find(derived) == derivedClasses.end()) {
			derivedClasses[derived] = std::set<std::string>();
			derivedClasses[derived].insert(derived);
		}
		if (baseClasses.find(base) == baseClasses.end()) {
			baseClasses[base] = std::set<std::string>();
			baseClasses[base].insert(base);
		}
		if (derivedClasses.find(base) == derivedClasses.end()) {
			derivedClasses[base] = std::set<std::string>();
			derivedClasses[base].insert(base);
		}
		for (auto ancestor : baseClasses[base]) {
			baseClasses[derived].insert(ancestor);
			derivedClasses[ancestor].insert(derived);
		}
	}
};

#define NEW(B, T, o) new T();                             \
TypeInfoForClasses[std::string(#o)] = TypeInfo(#T);


#define TYPEID(o) typeInfoForClasses[std::string(#o)]

#define EXTENDS(Derived, Base) Derived : Base { \
registrator r = registrator(#Derived, #Base, 0);

#define MEXTENDS(Derived, Base1, Base2) Derived : Base1, Base2 { \
registrator r1 = registrator(#Derived, #Base1, 0); \
registrator r2 = registrator(#Derived, #Base2, sizeof(Base1) );

#define _EXIST(collection, element) collection.find(element) != collection.end()

#define DYNAMIC_CAST(R, T, a) \
((_EXIST(baseClasses[#R], #T) || _EXIST(derivedClasses[#R], #T)) ? \
reinterpret_cast<T*>(reinterpret_cast<R*>(a)) : static_cast<T*>(nullptr))
