// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef CHROME_BROWSER_EXTENSIONS_API_SYSTEM_INFO_STORAGE_STORAGE_INFO_PROVIDER_H_
#define CHROME_BROWSER_EXTENSIONS_API_SYSTEM_INFO_STORAGE_STORAGE_INFO_PROVIDER_H_

#include "chrome/browser/extensions/system_info_provider.h"
#include "chrome/common/extensions/api/experimental_system_info_storage.h"

namespace extensions {
namespace systeminfo {

// TODO(hongbo): Here the storage type is declared as string type due to the
// IDL doesn't support enum type yet. Once http://crbug.com/141940 is fixed,
// we could reuse enum values generated by IDL compiler.

// Unknown storage type.
extern const char kStorageTypeUnknown[];
// Hard disk.
extern const char kStorageTypeHardDisk[];
// Removable storage, e.g. USB device, flash card reader.
extern const char kStorageTypeRemovable[];

}  // namespace systeminfo

typedef std::vector<linked_ptr<
    api::experimental_system_info_storage::StorageUnitInfo> > StorageInfo;

class StorageInfoProvider
    : public SystemInfoProvider<StorageInfo> {
 public:
  virtual ~StorageInfoProvider() {}

  // Get the single shared instance of StorageInfoProvider.
  static StorageInfoProvider* Get();

  // Get the information for the storage unit specified by the |id| parameter,
  // and output the result to the |info|.
  virtual bool QueryUnitInfo(const std::string& id,
      api::experimental_system_info_storage::StorageUnitInfo* info) = 0;
};

}  // namespace extensions

#endif  // CHROME_BROWSER_EXTENSIONS_API_SYSTEM_INFO_STORAGE_STORAGE_INFO_PROVIDER_H_
