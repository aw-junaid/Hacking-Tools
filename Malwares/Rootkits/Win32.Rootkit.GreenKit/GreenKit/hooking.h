#ifndef HOOKING_H
# define HOOKING_H

void hooking_addFunction(char* name, void* old);
void* hooking_getOldFunction(char* name);

#endif