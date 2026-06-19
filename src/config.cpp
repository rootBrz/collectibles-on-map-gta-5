/*
        THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
                                http://dev-c.com
                        (C) Alexander Blade 2015
*/

#include "config.h"
#include "collectibles/collectibles.h"

#include <string>

static Config g_config{};

static std::string getIniPath(HMODULE hModule) {
  char path[MAX_PATH];
  GetModuleFileNameA(hModule, path, MAX_PATH);
  std::string s(path);
  return s.substr(0, s.rfind('.')) + ".ini";
}

static bool readBool(const char *key, const std::string &path) {
  char val[16]{};
  GetPrivateProfileStringA("Collectibles", key, "true", val, sizeof(val),
                           path.c_str());
  return _strcmpi(val, "true") == 0;
}

static int readInt(const char *section, const char *key, int defaultVal,
                   const std::string &path) {
  return static_cast<int>(
      GetPrivateProfileIntA(section, key, defaultVal, path.c_str()));
}

void LoadConfig(HMODULE hModule) {
  const std::string path = getIniPath(hModule);

  // Collectible toggles
  g_config.spaceshipParts = readBool("SpaceshipPart", path);
  g_config.letterScraps = readBool("LetterScrap", path);
  g_config.nuclearWaste = readBool("NuclearWaste", path);
  g_config.submarineParts = readBool("SubmarinePart", path);
  g_config.hiddenPackages = readBool("HiddenPackage", path);
  g_config.monkeyMosaics = readBool("MonkeyMosaic", path);
  g_config.peyotePlants = readBool("PeyotePlant", path);
  g_config.epsilonTracts = readBool("EpsilonTract", path);
  g_config.underTheBridge = readBool("UnderTheBridge", path);
  g_config.knifeFlights = readBool("KnifeFlight", path);
  g_config.stuntJumps = readBool("StuntJump", path);

  g_config.toggleKey =
      static_cast<DWORD>(readInt("Keybinds", "ToggleKey", VK_F11, path));
  g_config.showAll = true;

  g_config.globalVar = readInt("Compatibility", "GlobalVar", 114931, path);
  g_config.offset = readInt("Compatibility", "Offset", 10056, path);

  g_config.computedOffset = g_config.globalVar + g_config.offset;

  auto set = [](CollectibleKind kind, bool value) {
    allCollectibles[static_cast<int>(kind)].enabled = value;
  };

  set(CollectibleKind::SpaceshipParts, g_config.spaceshipParts);
  set(CollectibleKind::LetterScraps, g_config.letterScraps);
  set(CollectibleKind::NuclearWaste, g_config.nuclearWaste);
  set(CollectibleKind::SubmarineParts, g_config.submarineParts);
  set(CollectibleKind::OceanPackages, g_config.hiddenPackages);
  set(CollectibleKind::MonkeyMosaics, g_config.monkeyMosaics);
  set(CollectibleKind::PeyotePlants, g_config.peyotePlants);
  set(CollectibleKind::EpsilonTracts, g_config.epsilonTracts);
  set(CollectibleKind::UnderTheBridge, g_config.underTheBridge);
  set(CollectibleKind::KnifeFlights, g_config.knifeFlights);
  set(CollectibleKind::StuntJumps, g_config.stuntJumps);
}

Config &GetConfig() { return g_config; }
