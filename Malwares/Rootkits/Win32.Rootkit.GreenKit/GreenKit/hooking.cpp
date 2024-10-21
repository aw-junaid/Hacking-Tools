#include "stdafx.h"
#include "hooking.h"
#include <map>

std::map<std::string, void*> map_OldFunctions;

void hooking_addFunction(char* name, void* old) {
	std::pair<std::string, void*> *pair_ntFunction = new std::pair<std::string, void*>(name, old);
	map_OldFunctions.insert(*pair_ntFunction);
}

void* hooking_getOldFunction(char* name) {
	return map_OldFunctions[name];
}