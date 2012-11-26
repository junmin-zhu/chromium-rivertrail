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
#include "base/logging.h"
#include "base/debug/trace_event.h"
#include "content/browser/browser_child_process_host_impl.h"
#include "content/browser/rivertrail/platform_env.h"
#include "content/browser/rivertrail/rivertrail_host.h"
#include "content/browser/rivertrail/rivertrail_message_filter.h"
#include "content/common/rivertrail/rivertrail_messages.h"
#include "content/public/browser/browser_thread.h"

using content::BrowserThread;

RivertrailHost::RivertrailHost(int host_id)
	: host_id_(host_id) {
}

bool RivertrailHost::OnSendSourceToOCL(std::string& kernelSource,
                                       std::string& kernelName,
                                       std::string& options) {
  if(!platform_env_->PlatformCompileKernel(kernelSource, kernelName, options)) {
    return false;
  }
  return true;
}

bool RivertrailHost::OnSendMemoryToOCL(base::SharedMemoryHandle& handle,
                                       const size_t& size,
                                       rivertrail::Type& type) {
  if(!base::SharedMemory::IsHandleValid(handle)) 
    return false;	
  return platform_env_->PlatformCompute(handle, type, size);
}


bool RivertrailHost::OnMessageReceived(const IPC::Message& msg) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  bool msg_is_ok = true;
  bool handle = true;
  IPC_BEGIN_MESSAGE_MAP_EX(RivertrailHost, msg, msg_is_ok)
	IPC_MESSAGE_HANDLER(RivertrailHostMsg_SendSourceToOCL, 
						OnSendSourceToOCL)
	IPC_MESSAGE_HANDLER(RivertrailHostMsg_SendMemoryToOCL,
						OnSendMemoryToOCL)
		//?
	IPC_MESSAGE_UNHANDLED(handle = false)
	IPC_END_MESSAGE_MAP_EX()
  return handle;
}

bool RivertrailHost::Send(IPC::Message* msg) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  
  bool result = process_->Send(msg);
  return result;
}


void RivertrailHost::OnChannelConnected(int32 peer_pid) {
  while(!queued_messages_.empty()) {
    Send(queued_messages_.front());
	queued_messages_.pop();
  }
}


void RivertrailHost::OnProcessLaunched() {
	//This need to discuss
}




