// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_DOWNLOAD_DOWNLOAD_DANGER_PROMPT_H_
#define CHROME_BROWSER_DOWNLOAD_DOWNLOAD_DANGER_PROMPT_H_

#include "base/callback_forward.h"

class ConstrainedWindow;
class TabContents;

namespace content {
class DownloadItem;
}

// Prompts the user for whether to Keep a dangerous DownloadItem using native
// UI. This prompt is invoked by the DownloadsDOMHandler when the user wants to
// accept a dangerous download. Having a native dialog intervene during the this
// workflow means that the chrome://downloads page no longer has the privilege
// to accept a dangerous download from script without user intervention. This
// step is necessary to prevent a malicious script form abusing such a
// privilege.
class DownloadDangerPrompt {
 public:
  // Actions resulting from showing the danger prompt.
  enum Action {
    ACCEPT,
    CANCEL
  };

  // Return a new self-deleting DownloadDangerPrompt. |accepted| or |canceled|
  // will be run when the the respective action is invoked. |canceled| may also
  // be called when |item| is either no longer dangerous or no longer in
  // progress, or if the tab corresponding to |tab_contents| is
  // closing. The returned DownloadDangerPrompt* is only used for testing. The
  // caller does not own the object and receive no guarantees about lifetime.
  static DownloadDangerPrompt* Create(
      content::DownloadItem* item,
      TabContents* tab_contents,
      const base::Closure& accepted,
      const base::Closure& canceled);

 protected:
  friend class DownloadDangerPromptTest;

  // Only to be used by tests. Subclasses must override to manually call the
  // respective button click handler.
  virtual void InvokeActionForTesting(Action action) = 0;
};

#endif  // CHROME_BROWSER_DOWNLOAD_DOWNLOAD_DANGER_PROMPT_H_
