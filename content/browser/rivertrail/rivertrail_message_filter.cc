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

#include "content/browser/rivertrail/rivertrail_message_filter.h"

#include "content/browser/rivertrail/platform_env.h"
#include "content/common/rivertrail/rivertrail_messages.h"
#include "content/public/browser/browser_thread.h"

using content::BrowserThread;
using content::BrowserMessageFilter;

namespace rivertrail {

RivertrailMessageFilter::RivertrailMessageFilter() {
  platform_data_ = new PlatformEnv();
}

RivertrailMessageFilter::~RivertrailMessageFilter() {
  platform_data_.release();
}

bool RivertrailMessageFilter::OnMessageReceived(const IPC::Message& message,
                                                 bool* message_was_ok) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP_EX(RivertrailMessageFilter, message, *message_was_ok)
   // IPC_MESSAGE_HANDLER(RivertrailHostMsg_Initializing, OnInitializing)
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP()
  return handled;
}

void RivertrailMessageFilter::OnInitializing(int render_view_id) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

//  handlers_map_[render_view_id] = new RivertrailHandler(render_view_id, this);
}

}  // namespace rivertrail
