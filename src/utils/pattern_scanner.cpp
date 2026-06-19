/*
        THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
                        (C) Alexander Blade 2015
*/

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <windows.h>

#include "config.h"
#include "pattern_scanner.h"
#include <iomanip>
#include <iostream>
#include <psapi.h>
#include <sstream>
#include <string>
#include <vector>

struct PatternByte {
  uint8_t value;
  bool isWildcard;
};

std::vector<PatternByte> ParsePattern(const std::string &patternStr) {
  std::vector<PatternByte> pattern;
  std::istringstream iss(patternStr);
  std::string token;

  while (iss >> token) {
    if (token == "??" || token == "?") {
      pattern.push_back({0, true});
    } else {
      uint8_t byteVal = static_cast<uint8_t>(std::stoul(token, nullptr, 16));
      pattern.push_back({byteVal, false});
    }
  }
  return pattern;
}

uint8_t *FindPattern(std::vector<PatternByte> &pattern) {
  if (pattern.empty())
    return nullptr;

  uint8_t *current = nullptr;
  SYSTEM_INFO sysInfo;
  GetSystemInfo(&sysInfo);
  uint8_t *maxAddr =
      static_cast<uint8_t *>(sysInfo.lpMaximumApplicationAddress);

  while (current < maxAddr) {
    MEMORY_BASIC_INFORMATION mbi;
    if (!VirtualQuery(current, &mbi, sizeof(mbi))) {
      break;
    }

    // Skip native modules (GTA5.exe, DLLs) to speed up Heap search
    if (mbi.Type != MEM_PRIVATE) {
      current = static_cast<uint8_t *>(mbi.BaseAddress) + mbi.RegionSize;
      continue;
    }

    if (mbi.Protect & PAGE_GUARD) {
      current = static_cast<uint8_t *>(mbi.BaseAddress) + mbi.RegionSize;
      continue;
    }

    // Strip safe flags, check if readable
    DWORD protect = mbi.Protect & ~(PAGE_NOCACHE | PAGE_WRITECOMBINE);
    bool isReadable =
        (mbi.State == MEM_COMMIT) &&
        (protect == PAGE_READONLY || protect == PAGE_READWRITE ||
         protect == PAGE_WRITECOPY || protect == PAGE_EXECUTE_READ ||
         protect == PAGE_EXECUTE_READWRITE ||
         protect == PAGE_EXECUTE_WRITECOPY);

    if (isReadable) {
      uint8_t *blockStart = current;
      uint8_t *blockEnd =
          static_cast<uint8_t *>(mbi.BaseAddress) + mbi.RegionSize;

      // Merge adjacent readable blocks
      while (blockEnd < maxAddr) {
        MEMORY_BASIC_INFORMATION nextMbi;
        if (!VirtualQuery(blockEnd, &nextMbi, sizeof(nextMbi)))
          break;

        if (nextMbi.Type == MEM_IMAGE)
          break;

        if (nextMbi.Protect & PAGE_GUARD)
          break;

        DWORD nextProtect =
            nextMbi.Protect & ~(PAGE_NOCACHE | PAGE_WRITECOMBINE);
        bool nextReadable =
            (nextMbi.State == MEM_COMMIT) &&
            (nextProtect == PAGE_READONLY || nextProtect == PAGE_READWRITE ||
             nextProtect == PAGE_WRITECOPY ||
             nextProtect == PAGE_EXECUTE_READ ||
             nextProtect == PAGE_EXECUTE_READWRITE ||
             nextProtect == PAGE_EXECUTE_WRITECOPY);

        if (!nextReadable)
          break;

        blockEnd =
            static_cast<uint8_t *>(nextMbi.BaseAddress) + nextMbi.RegionSize;
      }

      if (blockEnd > maxAddr)
        blockEnd = maxAddr;

      size_t regionSize = blockEnd - blockStart;

      if (regionSize >= pattern.size()) {
        const uint8_t firstByte = pattern[0].value;
        const bool firstWildcard = pattern[0].isWildcard;
        const size_t maxI = regionSize - pattern.size();

        for (size_t i = 0; i <= maxI; ++i) {
          if (!firstWildcard) {
            void *match = memchr(blockStart + i, firstByte, maxI - i + 1);
            if (!match)
              break;
            i = static_cast<size_t>(static_cast<uint8_t *>(match) - blockStart);
          }

          bool found = true;
          for (size_t j = 1; j < pattern.size(); ++j) {
            if (!pattern[j].isWildcard &&
                blockStart[i + j] != pattern[j].value) {
              found = false;
              break;
            }
          }

          if (found) {
            return blockStart + i;
          }
        }
      }
      current = blockEnd;
    } else {
      current = static_cast<uint8_t *>(mbi.BaseAddress) + mbi.RegionSize;
    }
  }
  return nullptr;
}

void FindGlobal() {
  std::string signature = "61 ?? ?? ?? 46 ?? ?? 42 ?? 5D ?? ?? ?? 2C";
  std::vector<PatternByte> pattern = ParsePattern(signature);
  uint8_t *match = FindPattern(pattern);
  uint32_t globalOffset = 0;
  uint32_t localOffset = 0;

  if (!match)
    goto not_found;

  globalOffset = (*reinterpret_cast<uint32_t *>(match + 1)) & 0x00FFFFFF;
  localOffset = *reinterpret_cast<uint16_t *>(match + 5);

  if (globalOffset == 0 || localOffset == 0)
    goto not_found;

  GetConfig().globalVar = globalOffset;
  GetConfig().offset = localOffset;
  GetConfig().computedOffset = globalOffset + localOffset;

  return;

not_found:
  MessageBoxA(NULL, "Pattern not found!\nFallback to offsets from ini file.",
              "Error", MB_OK | MB_ICONERROR);
}
