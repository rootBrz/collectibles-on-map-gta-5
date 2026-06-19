/*
        THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
                                http://dev-c.com
                        (C) Alexander Blade 2015
*/

#pragma once

#include "main.h"

struct Config {
  bool spaceshipParts = true;
  bool letterScraps = true;
  bool nuclearWaste = true;
  bool submarineParts = true;
  bool hiddenPackages = true;
  bool monkeyMosaics = true;
  bool peyotePlants = true;
  bool epsilonTracts = true;
  bool underTheBridge = true;
  bool knifeFlights = true;
  bool stuntJumps = true;

  DWORD toggleKey = VK_F11;
  bool showAll = true;

  int globalVar = 114931;
  int offset = 10056;

  int computedOffset = globalVar + offset;
};

void LoadConfig(HMODULE hModule);

Config &GetConfig();
