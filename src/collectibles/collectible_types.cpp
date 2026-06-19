/*
        THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
                                http://dev-c.com
                        (C) Alexander Blade 2015
*/

#include "collectibles/collectibles.h"
#include "collectibles/coords.h"
#include "config.h"
#include "script.h"

static bool GetGlobalBit(int collectibleOffset, int i, int bitsPerWord = 32) {
  int wordIndex = i / bitsPerWord;
  int bitIndex = i % bitsPerWord;

  uint32_t *word = reinterpret_cast<uint32_t *>(getGlobalPtr(
      GetConfig().computedOffset + collectibleOffset + 1 + wordIndex));
  return word && ((*word >> bitIndex) & 1U);
}

// Monkey Mosaics — 50 items, 32 bits per word.
static bool IsMosaicCollected(int index) { return GetGlobalBit(135, index); }

// Knife Flights — 15 items, 32 bits per word, separate storage block.
static bool IsFlightCollected(int index) { return GetGlobalBit(95, index); }

// Peyote Plants — 27 items, 6 bits per category × 3 separate global words.
static bool IsPeyoteCollected(int index) {
  int base = GetConfig().computedOffset + 138;

  int cat = (index >= 21) ? 2 : (index >= 15) ? 1 : 0; // water/highland/land
  int bitIndex = index - cat * 6;                      // 0..5 within category

  uint32_t *word = reinterpret_cast<uint32_t *>(getGlobalPtr(base + 1 + cat));
  return word && ((*word >> bitIndex) & 1U);
}

static bool IsUSJCollected(int index) {
  if (index < 0 || index >= CollectibleCoords::USJ_TOTAL)
    return false;

  Hash statHash = GAMEPLAY::GET_HASH_KEY("USJS_COMPLETED_MASK");
  int isCompleted = 0;
  STATS::STAT_GET_MASKED_INT(statHash, &isCompleted, index, 1, -1);
  return isCompleted != 0;
}

std::array<CollectibleInfo, 11> allCollectibles{{
    {"NUM_HIDDEN_PACKAGES_1",
     [](int i) { return (bool)STATS::GET_PACKED_STAT_BOOL_CODE(755 + i, -1); },
     []() { return IsScriptRunning("spaceshipparts"); }, 752, 52,
     CollectibleCoords::spaceship,
     std::vector<Blip>(CollectibleCoords::SPACESHIP_TOTAL, 0)},
    {"NUM_HIDDEN_PACKAGES_0",
     [](int i) { return (bool)STATS::GET_PACKED_STAT_BOOL_CODE(705 + i, -1); },
     []() { return IsScriptRunning("letterscraps"); }, 764, 21,
     CollectibleCoords::letterScraps,
     std::vector<Blip>(CollectibleCoords::LETTER_SCRAPS_TOTAL, 0)},
    {"NUM_HIDDEN_PACKAGES_3",
     [](int i) { return (bool)STATS::GET_PACKED_STAT_BOOL_CODE(815 + i, -1); },
     []() { return IsScriptRunning("ambient_sonar"); }, 654, 46,
     CollectibleCoords::nuclearWaste,
     std::vector<Blip>(CollectibleCoords::NUCLEAR_WASTE_TOTAL, 0)},
    {"NUM_HIDDEN_PACKAGES_4",
     [](int i) { return (bool)STATS::GET_PACKED_STAT_BOOL_CODE(845 + i, -1); },
     []() { return IsScriptRunning("ambient_diving"); }, 308, 54,
     CollectibleCoords::submarine,
     std::vector<Blip>(CollectibleCoords::SUBMARINE_TOTAL, 0)},
    {"BLIP_501",
     nullptr, // Disabled — always show
     nullptr, 408, 30, CollectibleCoords::oceanPackage,
     std::vector<Blip>(CollectibleCoords::OCEAN_PACKAGE_TOTAL, 0)},
    {"NUM_HIDDEN_PACKAGES_6", [](int i) { return IsMosaicCollected(i); },
     []() { return IsScriptRunning("photographymonkey"); }, 671, 50,
     CollectibleCoords::mosaic,
     std::vector<Blip>(CollectibleCoords::MOSAIC_TOTAL, 0)},
    {"NUM_HIDDEN_PACKAGES_5", [](int i) { return IsPeyoteCollected(i); },
     nullptr, 469, 69, CollectibleCoords::peyote,
     std::vector<Blip>(CollectibleCoords::PEYOTE_TOTAL, 0)},
    {"NUM_HIDDEN_PACKAGES_2",
     [](int i) { return (bool)STATS::GET_PACKED_STAT_BOOL_CODE(805 + i, -1); },
     []() { return IsScriptRunning("epsilontract"); }, 206, 12,
     CollectibleCoords::epsilonTract,
     std::vector<Blip>(CollectibleCoords::EPSILON_TRACT_TOTAL, 0)},
    {"SC_MSC_BRG",
     [](int i) {
       return IsFlightCollected(CollectibleCoords::bridgeIndices[i]);
     },
     nullptr, 64, 39, CollectibleCoords::bridge,
     std::vector<Blip>(CollectibleCoords::BRIDGE_TOTAL, 0)},
    {"SC_MSC_KFT",
     [](int i) {
       return IsFlightCollected(CollectibleCoords::flightIndices[i]);
     },
     nullptr, 424, 3, CollectibleCoords::flight,
     std::vector<Blip>(CollectibleCoords::FLIGHT_TOTAL, 0)},
    {"SC_MSC_STJ", [](int i) { return IsUSJCollected(i); }, nullptr, 559, 39,
     CollectibleCoords::usj,
     std::vector<Blip>(CollectibleCoords::USJ_TOTAL, 0)},
}};
