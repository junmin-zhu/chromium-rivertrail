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

#ifndef CONTENT_COMMON_RIVERTRAIL_CHILD_RIVERTRAIL_MESSAGE_FILTER_H_
#define CONTENT_COMMON_RIVERTRAIL_CHILD_RIVERTRAIL_MESSAGE_FILTER_H_

#include <vector>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "ipc/ipc_channel_proxy.h"
/*
namespace WebKit {
class WebSocketStreamHandle;
}

namespace webkit_glue {
class WebSocketStreamHandleBridge;
class WebSocketStreamHandleDelegate;
}
*/
// Dispatches Rivertrail related messages sent to a child process from the
// main browser process.  There is one instance per child process.  Messages
// are dispatched on the main child thread.  The RenderThread class
// creates an instance of RivertrailDispatcher and delegates calls to it.
class ChildRivertrailMessageFilter : public IPC::ChannelProxy::MessageFilter {
 public:
  ChildRivertrailMessageFilter();
  virtual ~ChildRivertrailMessageFilter() {}

  // IPC::ChannelProxy::MessageFilter implementation.
  virtual void OnFilterAdded(IPC::Channel* channel) OVERRIDE;
  virtual void OnFilterRemoved() OVERRIDE;
  virtual bool OnMessageReceived(const IPC::Message& message) OVERRIDE;
/*  static webkit_glue::WebSocketStreamHandleBridge* CreateBridge(
      WebKit::WebSocketStreamHandle* handle,
      webkit_glue::WebSocketStreamHandleDelegate* delegate);*/

  // IPC::Listener implementation.
  //virtual bool OnMessageReceived(const IPC::Message& msg) OVERRIDE;

 private:
  void OnInitialized(int socket_id);
/*  void OnSentData(int socket_id, int amount_sent);
  void OnReceivedData(int socket_id, const std::vector<char>& data);
  void OnClosed(int socket_id);*/

  DISALLOW_COPY_AND_ASSIGN(ChildRivertrailMessageFilter);
};

#endif  // CONTENT_COMMON_RIVERTRAIL_CHILD_RIVERTRAIL_MESSAGE_FILTER_H_
