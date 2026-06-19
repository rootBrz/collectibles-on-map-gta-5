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
#include <array>
#include <functional>

enum class CollectibleKind : int {
  SpaceshipParts,
  LetterScraps,
  NuclearWaste,
  SubmarineParts,
  OceanPackages,
  MonkeyMosaics,
  PeyotePlants,
  EpsilonTracts,
  UnderTheBridge,
  KnifeFlights,
  StuntJumps,
  TotalCount // sentinel: number of collectible kinds
};

struct CollectibleInfo {
  const char *name;
  std::function<bool(int)> isCollected; // nullptr = Disabled (always show)
  std::function<bool()> isAccessible;   // nullptr = always unlocked
  int spriteId;
  int colorId;
  const float (*coords)[3];
  std::vector<Blip> blips;
  bool enabled = true;
};

extern std::array<CollectibleInfo, 11> allCollectibles;

void AddCollectibleBlips(CollectibleInfo &info);
void RemoveCollectibleBlips(CollectibleInfo &info);
void RemoveAllCollectibleBlips();
