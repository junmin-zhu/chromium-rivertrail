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

#include "content/browser/rivertrail/platform_env.h"

#include <cmath>
#include "content/common/rivertrail/rivertrail_messages.h"

namespace rivertrail {
cl_platform_id s_platforms = NULL;

IPC::Message* PlatformEnv::CreateIPCMessage(int render_view_id) const {
/*  RivertrailMsg_Updated_Params params;
  params.extensions = extensions_;
  params.name = name_;
  params.profile = profile_;
  params.vendor = vendor_;
  params.version = version_;
  params.numberOfDevices = numberOfDevices_;

  return new RivertrailMsg_Updated(render_view_id, params);*/
  return NULL;
}

PlatformEnv::PlatformEnv() {
  cl_uint devices;
  cl_int err_code = clGetDeviceIDs(s_platforms, CL_DEVICE_TYPE_ALL, 0, NULL, &devices);
  numberOfDevices_ = devices;

	platform = GetPlatform();

  getPlatformPropertyHelper(CL_PLATFORM_EXTENSIONS, extensions_);
  getPlatformPropertyHelper(CL_PLATFORM_NAME, name_);
  getPlatformPropertyHelper(CL_PLATFORM_PROFILE, profile_);
  getPlatformPropertyHelper(CL_PLATFORM_VENDOR, vendor_);
  getPlatformPropertyHelper(CL_PLATFORM_VERSION, version_);
}

PlatformEnv::~PlatformEnv() {
  delete extensions_;
  delete name_;
  delete profile_;
  delete vendor_;
  delete version_;
}

int PlatformEnv::getPlatformPropertyHelper(cl_platform_info param, char* & out) {
  char* rString = NULL;
  size_t length;
  cl_int err;

  err = clGetPlatformInfo(s_platforms, param, 0, NULL, &length);
  if (err == CL_SUCCESS) {
    rString = new char[length+1];
    err = clGetPlatformInfo(s_platforms, param, length, rString, NULL);
    out = rString;
  }

  return err;
}

bool PlatformEnv::PlatformCompute(base::SharedMemoryHandle& handle,
                                  const Type& type,
                                  const size_t& size) {
  if(!compute_unit_->IsKernelValid())
    return false;
  return compute_unit_->ComputeSharedMemory(handle, size, type);
}

}  // namespace rivertrail
