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

#ifndef CONTENT_BROWSER_RIVERTRAIL_COMPUTE_UNIT_H_
#define CONTENT_BROWSER_RIVERTRAIL_COMPUTE_UNIT_H_

#include <string>
#include <map>

#include "base/shared_memory.h"
#include "base/memory/ref_counted.h"
#include "content/browser/rivertrail/opencl_compat.h"
#include "content/browser/rivertrail/opencl_config.h"
#include "content/browser/rivertrail/platform_env.h"

namespace rivertrail {
enum Type {
  INT = 0,
  FLOAT,
  DOUBLE
};

class ComputeUnit
	: public base::RefCountedThreadSafe<ComputeUnit> {
 public:
  ComputeUnit();
  ComputeUnit(const cl_platform_id& platform,
              const int& devType);
  virtual ~ComputeUnit();
  
  //Initialize the platform, context, and so on
  bool Init(const cl_platform_id& platform,
            const int& devType);
  
  //Use the source, name and options to create and build the kernel
  bool CompileKernel(const std::string& source, 
                     const std::string& kernelName,
                     const std::string& options);

  inline bool IsKernelValid() { return kernel != NULL; }
	
  //Get the memory by the handle and then convert the memory to the
  //array(which type is not determined)
  void* ConvertMemoryToArray(base::SharedMemoryHandle& handle);

  bool ComputeSharedMemory(base::SharedMemoryHandle& handle, 
                           const size_t& size,
                           const Type& type);

  bool ComputeInteger(int* data,
                      //int* result,
                      const size_t& size);
  bool ComputeDouble(double* data,
                     // double* result,
                     const size_t& size);
  bool ComputeFloat(float* data,
                    //float* result,
                    const size_t& size);


 private:
  bool ComputeIntegerMemory(base::SharedMemoryHandle& handle, 
                            const size_t& size);
  bool ComputeFloatMemory(base::SharedMemoryHandle& handle, 
                          const size_t& size);
  bool ComputeDoubleMemory(base::SharedMemoryHandle& handle, 
                           const size_t& size);

  cl_kernel FindKernelBySource(const std::string& source);

 private:
  cl_platform_id platform;
  cl_kernel kernel;
  cl_context context;
  cl_command_queue commands;
  cl_device_id device_id;
  cl_program program;
  char* buildLog;
  size_t buildLogSize;
  
  std::map<std::string, cl_kernel> kernels;
};
}

#endif //CONTENT_BROWSER_RIVERTRAIL_COMPUTE_UNIT_H_s
