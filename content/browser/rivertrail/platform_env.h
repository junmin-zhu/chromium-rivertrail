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
#include "base/memory/scoped_ptr.h"
#include "base/shared_memory.h"
#include "content/browser/rivertrail/compute_unit.h"
#include "content/common/content_export.h"
#include "opencl_config.h"
#include "opencl_compat.h"

namespace IPC {
class Message;
}

namespace rivertrail {

class CONTENT_EXPORT PlatformEnv :
    public base::RefCountedThreadSafe<PlatformEnv> {
 public:
  virtual IPC::Message* CreateIPCMessage(int render_view_id) const;
  PlatformEnv();
  virtual ~PlatformEnv();

  inline cl_platform_id GetPlatform() {
	cl_int err = CL_SUCCESS;
	err = clGetPlatformIDs(1, &platform, NULL);
	if(err != CL_SUCCESS)
	  return NULL;
	  return platform;
  }

  inline bool PlatformCompileKernel(const std::string& kernelSource,
                                    const std::string& kernelName,
                                    const std::string& options) {
	return compute_unit_->CompileKernel(kernelSource, kernelName, options);
  }

  bool PlatformCompute(base::SharedMemoryHandle& handle,
                       const Type& type,
                       const size_t& size);


 private:
  friend class base::RefCountedThreadSafe<PlatformEnv>;

  int getPlatformPropertyHelper(cl_platform_info param, char* & out);

  int numberOfDevices_;
  char* extensions_;
  char* profile_;
  char* vendor_;
  char* name_;
  char* version_;

  cl_platform_id platform;

  scoped_ptr<rivertrail::ComputeUnit> compute_unit_;

  DISALLOW_COPY_AND_ASSIGN(PlatformEnv);
};

}  // namespace rivertrail

#endif  // CONTENT_BROWSER_RIVERTRAIL_PLATFORM_DATA_H_
