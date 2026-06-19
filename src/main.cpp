/*
        THIS FILE IS A PART OF GTA V SCRIPT HUNK SDK
                                http://dev-c.com
                        (C) Alexander Blade 2015
*/

#include "config.h"
#include "collectibles/collectibles.h"
#include "input/keyboard.h"
#include "script.h"

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD reason, LPVOID /*lpReserved*/) {
  switch (reason) {
  case DLL_PROCESS_ATTACH:
    LoadConfig(hInstance);
    scriptRegister(hInstance, ScriptMain);
    keyboardHandlerRegister(OnKeyboardMessage);
    break;

  case DLL_PROCESS_DETACH:
    RemoveAllCollectibleBlips();
    scriptUnregister(hInstance);
    keyboardHandlerUnregister(OnKeyboardMessage);
    break;
  }

  return TRUE;
}
