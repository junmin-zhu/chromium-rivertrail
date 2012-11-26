/*************************************************************************
Copyright (c) 2011, Intel Corporation
All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:
- Redistributions of source code must retain the above copyright notice, 
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice, 
  this list of conditions and the following disclaimer in the documentation 
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
THE POSSIBILITY OF SUCH DAMAGE.
*************************************************************************/

#ifndef CONTENT_BROWSER_RENDERER_HOST_RIVERTRAIL_HOST_H_
#define CONTENT_BROWSER_RENDERER_HOST_RIVERTRAIL_HOST_H_

#include <vector>

#include "base/callback.h"
#include "base/memory/scoped_ptr.h"
#include "base/shared_memory.h"
#include "base/process.h"


#include "content/browser/rivertrail/compute_unit.h"
#include "content/browser/rivertrail/platform_env.h"
#include "content/public/browser/browser_child_process_host_delegate.h"
#include "net/socket_stream/socket_stream.h"
#include "ipc/ipc_sender.h"

class BrowserChildProcessHostImpl;

namespace IPC {
	struct ChannelHandle;
}

class RivertrailHost : public content::BrowserChildProcessHostDelegate,
											 public IPC::Sender {
 public:
  virtual bool Send(IPC::Message* msg) OVERRIDE;

  static RivertrailHost* FromID(int host_id);
  int host_id() const { return host_id_; }


 private:
  static bool HostIsValid(RivertrailHost* host);

  RivertrailHost(int host_id);
  virtual ~RivertrailHost();

  bool Init();

  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
  virtual void OnChannelConnected(int32 peer_pid) OVERRIDE;
  virtual void OnProcessLaunched() OVERRIDE;
  virtual void OnProcessCrashed(int exit_code) OVERRIDE;

  virtual bool OnSendSourceToOCL(std::string& kernelSource,
                                 std::string& kernelName,
                                 std::string& options);
  virtual bool OnSendMemoryToOCL(base::SharedMemoryHandle& handle, 
                                 const size_t& size,
                                 rivertrail::Type& type);
	
 private:
  int host_id_;
  std::queue<IPC::Message*> queued_messages_;
  //scoped_ptr<rivertrail::ComputeUnit> compute_unit_;
  scoped_ptr<rivertrail::PlatformEnv> platform_env_;
  scoped_ptr<BrowserChildProcessHostImpl> process_;
	
};


#endif  // CONTENT_BROWSER_RENDERER_HOST_RIVERTRAIL_HOST_H_
