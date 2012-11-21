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

#ifndef CONTENT_BROWSER_RIVERTRAIL_RIVERTRAIL_MESSAGE_FILTER_H_
#define CONTENT_BROWSER_RIVERTRAIL_RIVERTRAIL_MESSAGE_FILTER_H_

#include <map>

#include "content/browser/rivertrail/platform_data.h"
#include "content/public/browser/browser_message_filter.h"

namespace rivertrail {

class RivertrailMessageFilter : public content::BrowserMessageFilter {
 public:
  RivertrailMessageFilter();

  // content::BrowserMessageFilter implementation.
  virtual bool OnMessageReceived(const IPC::Message& message,
                                 bool* message_was_ok) OVERRIDE;
 protected:
  void OnInitializing(int render_view_id);

 private:
  virtual ~RivertrailMessageFilter();

  scoped_refptr<PlatformData> platform_data_;

  DISALLOW_COPY_AND_ASSIGN(RivertrailMessageFilter);
};

}  // namespace rivertrail

#endif  // CONTENT_BROWSER_RIVERTRAIL_RIVERTRAIL_MESSAGE_FILTER_H_
