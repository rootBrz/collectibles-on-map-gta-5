/*
        THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
                                http://dev-c.com
                        (C) Alexander Blade 2015
*/

#include "collectibles/collectibles.h"
#include "config.h"
#include "script.h"

void AddCollectibleBlips(CollectibleInfo &info) {
  if (info.isAccessible && !info.isAccessible()) {
    RemoveCollectibleBlips(info);
    return;
  }

  for (int i = 0; i < info.blips.size(); i++) {
    bool collected = info.isCollected && info.isCollected(i);

    if (!collected && info.enabled) {
      if (info.blips[i] == 0) {
        Blip blip = UI::ADD_BLIP_FOR_COORD(info.coords[i][0], info.coords[i][1],
                                           info.coords[i][2]);
        UI::SET_BLIP_SPRITE(blip, info.spriteId);
        UI::SET_BLIP_COLOUR(blip, info.colorId);
        UI::SET_BLIP_SCALE(blip, 0.75f);
        UI::SET_BLIP_AS_SHORT_RANGE(blip, TRUE);
        UI::BEGIN_TEXT_COMMAND_SET_BLIP_NAME(info.name);
        UI::END_TEXT_COMMAND_SET_BLIP_NAME(blip);
        info.blips[i] = blip;
      }
    } else if (info.blips[i] != 0) {
      UI::REMOVE_BLIP(&info.blips[i]);
      info.blips[i] = 0;
    }
  }
}

void RemoveCollectibleBlips(CollectibleInfo &info) {
  for (int i = 0; i < info.blips.size(); i++) {
    if (info.blips[i] != 0) {
      UI::REMOVE_BLIP(&info.blips[i]);
      info.blips[i] = 0;
    }
  }
}

void RemoveAllCollectibleBlips() {
  for (int i = 0; i < allCollectibles.size(); i++) {
    RemoveCollectibleBlips(allCollectibles[i]);
  }
}
