/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2020 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#ifndef XENIA_PATCHER_H_
#define XENIA_PATCHER_H_

#include <map>
#include <regex>
#include <vector>

#include "third_party/cpptoml/include/cpptoml.h"
#include "third_party/xxhash/xxhash.h"

#include "xenia/memory.h"

struct patchDataEntry {
  const uint8_t alloc_size_;
  const uint32_t memory_address_;
  const uint64_t new_value_;

  patchDataEntry(const uint8_t alloc_size, const uint32_t memory_address,
                 const uint64_t new_value)
      : alloc_size_(alloc_size),
        memory_address_(memory_address),
        new_value_(new_value){};
};

struct patchInfoEntry {
  uint32_t id;
  XXH64_state_t hash_state;  // Unsupported. For now.
  std::string patch_name;
  std::string patch_desc;
  std::string patch_author;
  std::vector<patchDataEntry> patch_data;
  bool is_enabled;
};

struct patchFileEntry {
  uint32_t title_id;
  std::string title_name;
  std::vector<patchInfoEntry> patch_info;
};

namespace xe {
namespace patcher {

class PatchingSystem {
 public:
  PatchingSystem();
  ~PatchingSystem();

  void LoadPatches();
  void ApplyPatch(Memory* memory, const patchInfoEntry* patch);
  void ApplyPatchesForTitle(Memory* memory, const uint32_t title_id);

  patchFileEntry ReadPatchFile(const std::filesystem::path& file_path);
  std::vector<patchDataEntry>* ReadPatchData(
      const std::string size_type,
      const std::shared_ptr<cpptoml::table>& patch_table);

  std::vector<patchFileEntry>& GetAllPatches() { return loaded_patch_files; }

  bool IsAnyPatchApplied() { return is_any_patch_applied_; }

 private:
  std::vector<patchFileEntry> loaded_patch_files;

  bool is_any_patch_applied_;

  uint8_t GetAllocSize(const std::string provided_size);
};

}  // namespace patcher
}  // namespace xe
#endif
