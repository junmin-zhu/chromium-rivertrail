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
/*#include "content/common/socket_stream_handle_data.h"

#include "webkit/glue/websocketstreamhandle_impl.h"

using webkit_glue::WebSocketStreamHandleImpl;
using WebKit::WebSocketStreamHandle;

// static
void SocketStreamHandleData::AddToHandle(
    WebSocketStreamHandle* handle, int render_view_id) {
  if (!handle) {
    NOTREACHED();
    return;
  }
  WebSocketStreamHandleImpl* impl =
      static_cast<WebSocketStreamHandleImpl*>(handle);
  impl->SetUserData(handle, new SocketStreamHandleData(render_view_id));
}

// static
const SocketStreamHandleData* SocketStreamHandleData::ForHandle(
    WebSocketStreamHandle* handle) {
  if (!handle)
    return NULL;
  WebSocketStreamHandleImpl* impl =
      static_cast<WebSocketStreamHandleImpl*>(handle);
  return static_cast<SocketStreamHandleData*>(impl->GetUserData(handle));
}
*/