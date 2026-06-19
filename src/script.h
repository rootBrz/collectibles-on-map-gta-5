/*
        THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
                                http://dev-c.com
                        (C) Alexander Blade 2015
*/

#pragma once

#include "enums.h"
#include "main.h"
#include "natives.h"
#include "types.h"

// Main script entry point — called from DllMain
void ScriptMain();
bool IsScriptRunning(const char *scriptName);
