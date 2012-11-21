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

#ifndef CONTENT_BROWSER_RIVERTRAIL_PLATFORM_DATA_H_
#define CONTENT_BROWSER_RIVERTRAIL_PLATFORM_DATA_H_

//#include <wtf/text/WTFString.h>

#include "base/memory/ref_counted.h"
#include "content/common/content_export.h"
#include "opencl_config.h"
#include "opencl_compat.h"

namespace IPC {
class Message;
}

namespace rivertrail {

class CONTENT_EXPORT PlatformData :
    public base::RefCountedThreadSafe<PlatformData> {
 public:
  virtual IPC::Message* CreateIPCMessage(int render_view_id) const;
  PlatformData();
  virtual ~PlatformData();

 private:
  friend class base::RefCountedThreadSafe<PlatformData>;

  int getPlatformPropertyHelper(cl_platform_info param, char* & out);

  int numberOfDevices_;
  char* extensions_;
  char* profile_;
  char* vendor_;
  char* name_;
  char* version_;

  DISALLOW_COPY_AND_ASSIGN(PlatformData);
};

}  // namespace rivertrail

#endif  // CONTENT_BROWSER_RIVERTRAIL_PLATFORM_DATA_H_
