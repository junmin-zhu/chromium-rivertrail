// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_DESKTOP_PROCESS_H_
#define REMOTING_HOST_DESKTOP_PROCESS_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "ipc/ipc_listener.h"

namespace IPC {
class ChannelProxy;
}  // namespace IPC

namespace remoting {

class AutoThreadTaskRunner;
class DesktopSessionAgent;

class DesktopProcess : public IPC::Listener {
 public:
  DesktopProcess(scoped_refptr<AutoThreadTaskRunner> caller_task_runner,
                 const std::string& daemon_channel_name);
  virtual ~DesktopProcess();

  // IPC::Listener implementation.
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void OnChannelConnected(int32 peer_pid) OVERRIDE;
  virtual void OnChannelError() OVERRIDE;

  // Creates the desktop agent and required threads and IPC channels. Returns
  // true on success.
  bool Start();

 private:
  // Crashes the process in response to a daemon's request. The daemon passes
  // the location of the code that detected the fatal error resulted in this
  // request. See the declaration of ChromotingDaemonDesktopMsg_Crash message.
  void OnCrash(const std::string& function_name,
               const std::string& file_name,
               const int& line_number);

  // Task runner on which public methods of this class should be called.
  scoped_refptr<AutoThreadTaskRunner> caller_task_runner_;

  // Name of the IPC channel connecting the desktop process with the daemon
  // process.
  std::string daemon_channel_name_;

  // IPC channel connecting the desktop process with the daemon process.
  scoped_ptr<IPC::ChannelProxy> daemon_channel_;

  // Provides screen/audio capturing and input injection services for
  // the network process.
  scoped_ptr<DesktopSessionAgent> desktop_agent_;

  DISALLOW_COPY_AND_ASSIGN(DesktopProcess);
};

}  // namespace remoting

#endif  // REMOTING_HOST_DESKTOP_PROCESS_H_
