/*
        THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
                                http://dev-c.com
                        (C) Alexander Blade 2015
*/

#include "script.h"
#include "collectibles/collectibles.h"
#include "config.h"
#include "input/keyboard.h"
#include "utils/pattern_scanner.h"

#include <cstdlib>

#pragma warning(disable : 4244 4305) // double <-> float conversions

static constexpr int BLIP_UPDATE_MS = 250;

void ScriptMain() {
  srand(static_cast<unsigned int>(GetTickCount()));

  while (!IsScriptRunning("main")) {
    WAIT(10);
  }

  static bool initialized = [] {
    FindGlobal();
    return true;
  }();

  // Clear any orphaned blips left over from a previous loadout.
  RemoveAllCollectibleBlips();

  int lastBlipTick = 0;

  while (true) {
    WAIT(1);
    Config &cfg = GetConfig();
    int now = static_cast<int>(GetTickCount());

    if (IsKeyJustUp(cfg.toggleKey)) {
      cfg.showAll = !cfg.showAll;

      if (!cfg.showAll) {
        RemoveAllCollectibleBlips();
      }
    }

    // Update blips on a slower cadence to avoid excessive stat/blip calls
    if (now - lastBlipTick < BLIP_UPDATE_MS || !cfg.showAll)
      continue;

    lastBlipTick = now;
    for (int i = 0; i < allCollectibles.size(); i++) {
      CollectibleInfo &info = allCollectibles[i];

      if (!info.enabled)
        continue;

      AddCollectibleBlips(info);
    }
  }
}

bool IsScriptRunning(const char *scriptName) {
  return SCRIPT::_GET_NUMBER_OF_INSTANCES_OF_STREAMED_SCRIPT(
             GAMEPLAY::GET_HASH_KEY(scriptName)) > 0;
}
