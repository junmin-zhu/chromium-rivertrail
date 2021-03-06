// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_SYSTEM_CHROMEOS_TRAY_DISPLAY_H_
#define ASH_SYSTEM_CHROMEOS_TRAY_DISPLAY_H_

#include "ash/system/tray/system_tray_item.h"
#include "base/memory/scoped_ptr.h"
#include "ui/aura/display_observer.h"

#include "chromeos/display/output_configurator.h"

namespace views {
class View;
}

namespace ash {

namespace internal {
class DisplayView;

class TrayDisplay : public SystemTrayItem,
                    public chromeos::OutputConfigurator::Observer,
                    public aura::DisplayObserver {
 public:
  TrayDisplay();
  virtual ~TrayDisplay();

 private:
  // Overridden from SystemTrayItem.
  virtual views::View* CreateDefaultView(user::LoginStatus status) OVERRIDE;
  virtual void DestroyDefaultView() OVERRIDE;

  // Overridden from aura::DisplayObserver
  virtual void OnDisplayBoundsChanged(const gfx::Display& display) OVERRIDE;
  virtual void OnDisplayAdded(const gfx::Display& new_display) OVERRIDE;
  virtual void OnDisplayRemoved(const gfx::Display& old_display) OVERRIDE;

  // Overridden from chromeos::OutputConfigurator::Observer
  virtual void OnDisplayModeChanged() OVERRIDE;

  DisplayView* default_;

  DISALLOW_COPY_AND_ASSIGN(TrayDisplay);
};

}  // namespace internal
}  // namespace ash

#endif  // ASH_SYSTEM_CHROMEOS_TRAY_DISPLAY_H_
