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
#include "content/browser/rivertrail/compute_unit.h"
#include "content/browser/rivertrail/debug_log.h"

namespace rivertrail {
	ComputeUnit::ComputeUnit()
		: buildLogSize(BUILDLOG_MAX) {
			buildLog = new char[buildLogSize];
	}


	/* Initailize the device_id, context, command queue by using 
	 * the platform and device type
	 */
	bool ComputeUnit::Init(const cl_platform_id& platform,
												 const int& devType) {
		cl_int err = 0;
		err = clGetDeviceIDs(platform, devType, 1, &device_id, NULL);
		if(err != CL_SUCCESS) {
			DEBUG_LOG_ERROR("Init: Get device IDs", err);
			return false;
		}
		
		const cl_context_properties properties[3] = {CL_CONTEXT_PLATFORM, 
																			(cl_context_properties)platform, 
																			NULL};
		context = clCreateContext(properties, 1, &device_id, NULL, NULL, &err);
		if(err != CL_SUCCESS) {
			DEBUG_LOG_ERROR("Init: Create Context", err);
			return false;
		}
		
		commands = clCreateCommandQueue(context, device_id, 0, &err);
		if(err != CL_SUCCESS) {
			DEBUG_LOG_ERROR("Init: Create Command queue", err);
			return false;
		}
		return true;
	}

	bool ComputeUnit::CompileKernel(const std::string& source,
																const std::string& kernelName,
																const std::string& options) {
		cl_int err = CL_SUCCESS;

		char* sourcePtr, *kernelNamePtr, *optionsPtr = NULL;

		if(source.empty()) {
			DEBUG_LOG_STATUS("Compile kernel", "Source code is empty");
			return false;
		}

		sourcePtr = const_cast<char*>(source.c_str());

		program = clCreateProgramWithSource(context, 1, 
																				(char**)&sourcePtr,
																				NULL, &err);
		if(err != CL_SUCCESS) {
			DEBUG_LOG_STATUS("Compile kernel", "Failed to create program: " << err);
			return false;
		}

		if(options.length() != 0) {
			optionsPtr = new char[options.length()];
			strcpy(optionsPtr, options.c_str());
		}

		err = clBuildProgram(program, 0, NULL, optionsPtr, NULL, NULL);
		if(err != CL_SUCCESS) {
			DEBUG_LOG_STATUS("Compile kernel", "Failed to build program: "<<err);
			return false;
		}

		if(kernelName.length() != 0) {
			kernelNamePtr = new char[kernelName.length()];
			strcpy(kernelNamePtr, kernelName.c_str());
		}
		else {
			kernelNamePtr = new char[DEFAULT_KERNEL_NAME_SIZE];
			std::string temp = DEFAULT_KERNEL_NAME;
			strcpy(kernelNamePtr, temp.c_str());
		}

		kernel = clCreateKernel(program, kernelNamePtr, &err);
		if(err != CL_SUCCESS) {
			DEBUG_LOG_STATUS("Compile kernel", "Failed to create kernel: "<<err);
			return false;
		}

		DEBUG_LOG_STATUS("Compile kernel", "Compile kernel success");
		return true;	
	}

	//Convert the Shared memory to the array which type is void*
	//and return the array
	void* ComputeUnit::ConvertMemoryToArray(base::SharedMemoryHandle& handle) {
		if(!base::SharedMemory::IsHandleValid(handle)) {
			DEBUG_LOG_STATUS("Compute shared memory", 
							         "Invalid handle");
			return NULL;
		}

		uint32 bytes = -1;
		base::SharedMemory* sharedmemory = new base::SharedMemory(handle, false);
		if(!sharedmemory->Map(bytes)) {
			DEBUG_LOG_STATUS("Compute shared memory",
										 "Failed to map the memory to current space");
			return NULL;
		}
		return sharedmemory->memory();
	}

	//Send the memory to the kernel to compute and return the compute result
	//and then assigns the result to the _retval
	//The paramenters is SharedMemory , the return array, the memory size,
	// and the type of the memory's element
	bool ComputeUnit::ComputeSharedMemory(base::SharedMemoryHandle& handle,
																			void* _retval,
																			const size_t& size,
																			Type& type) {
		if(size <= 0 ) {
			DEBUG_LOG_STATUS("Compute shared memory", 
							         "Invalid memory size");
			return false;
		}

		switch(type) {
		case INT:
			return ComputeIntegerMemory(handle, size, (int*)_retval);
		case FLOAT:
			return ComputeFloatMemory(handle, size, (float*)_retval);
		case DOUBLE:
			return ComputeDoubleMemory(handle, size, (double*)_retval);
		}

		DEBUG_LOG_STATUS("Compute shared memory", 
							         "Failed to compute the shared memory");
		return false;
	}


	//If the memory's element's type is INT/FLOAT/DOUBLE, when send to the 
	//compute kernel, it should be like a array which type is int/float/double
	//, the send the array to the kernel to compute.
	bool ComputeUnit::ComputeInteger(const int* data,
																 int* result,
																 const size_t& size) {
		if(data == NULL) {
			DEBUG_LOG_STATUS("Compute Integer", "Input data is null");
			return false;
		}
		int err_code;

		size_t global, local;

		cl_mem input;
		cl_mem output;

		// Create the device memory vectors
		input = clCreateBuffer(context, CL_MEM_READ_ONLY, 
													sizeof(int) * size, NULL, NULL);
		output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
														sizeof(int) * size, NULL, NULL);
		if(!input || !output) {
			DEBUG_LOG_STATUS("COMPUTE INTEGER", 
											"Failed to acclocate the input or output");
			return false;
		}

		// Transfer the input vector into device memory
		err_code = clEnqueueWriteBuffer(commands, input, CL_TRUE, 0, 
																		sizeof(int) * size,
																		data, 0, NULL, NULL);
		if(err_code != CL_SUCCESS) {
			DEBUG_LOG_STATUS("COMPUTE INTEGER", 
								"Failed to enqueue write buffer, error code is" << err_code);
			return false;
		}

		// Set the arguments to the compute kernel
		err_code = 0;
		err_code = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input);
		err_code |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &output);
		err_code |= clSetKernelArg(kernel, 2, sizeof(size_t), &size);

		if(err_code != CL_SUCCESS) {
			DEBUG_LOG_STATUS("COMPUTE INTEGER", 
				  "Failed to set kernel args, error code: " << err_code);
			return false;
		}

		global = size;

		// Get the maximum work group size for executing the kernel on the device
		err_code = clGetKernelWorkGroupInfo(kernel, device_id, 
																				CL_KERNEL_WORK_GROUP_SIZE, sizeof(local),
																				&local, NULL);
		if(err_code != CL_SUCCESS) {
			DEBUG_LOG_STATUS("COMPUTE INTEGER", 
					"Failed to get kernel work-group info, error code: " <<err_code);
			return false;
		}

		// Execute the kernel over the vector using the 
		// maximum number of work group items for this device
		err_code = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &global, &local,
																			0, NULL, NULL);
		if(err_code != CL_SUCCESS) {
			DEBUG_LOG_ERROR("COMPUTE INTEGER", err_code);
			return false;
		}
	
		// Wait for all commands to complete
		clFinish(commands);

		// Read back the results from the device to verify the output
		err_code = clEnqueueReadBuffer(commands, output, CL_TRUE, 0, 
															sizeof(int) * size, result, 0, NULL, NULL);	
		if(err_code != CL_SUCCESS) {
			DEBUG_LOG_ERROR("COMPUTE INTEGER", err_code);
			return false;
		}
		clReleaseMemObject(input);
		clReleaseMemObject(output);

		return true;
	}

	bool ComputeUnit::ComputeFloat(const float* data,
																 float* result,
																 const size_t& size) {
		if(data == NULL) {
			DEBUG_LOG_STATUS("Compute Integer", "Input data is null");
			return false;
		}
		int err_code;

		size_t global, local;

		cl_mem input;
		cl_mem output;

		// Create the device memory vectors
		input = clCreateBuffer(context, CL_MEM_READ_ONLY, 
													sizeof(float) * size, NULL, NULL);
		output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
														sizeof(float) * size, NULL, NULL);
		if(!input || !output) {
			DEBUG_LOG_STATUS("COMPUTE INTEGER", 
											"Failed to acclocate the input or output");
			return false;
		}

		// Transfer the input vector into device memory
		err_code = clEnqueueWriteBuffer(commands, input, CL_TRUE, 0, 
																		sizeof(float) * size,
																		data, 0, NULL, NULL);
		if(err_code != CL_SUCCESS) {
			DEBUG_LOG_STATUS("COMPUTE INTEGER", 
								"Failed to enqueue write buffer, error code is" << err_code);
			return false;
		}

		// Set the arguments to the compute kernel
		err_code = 0;
		err_code = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input);
		err_code |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &output);
		err_code |= clSetKernelArg(kernel, 2, sizeof(size_t), &size);

		if(err_code != CL_SUCCESS) {
			DEBUG_LOG_STATUS("COMPUTE INTEGER", 
				  "Failed to set kernel args, error code: " << err_code);
			return false;
		}

		global = size;

		// Get the maximum work group size for executing the kernel on the device
		err_code = clGetKernelWorkGroupInfo(kernel, device_id, 
																				CL_KERNEL_WORK_GROUP_SIZE, sizeof(local),
																				&local, NULL);
		if(err_code != CL_SUCCESS) {
			DEBUG_LOG_STATUS("COMPUTE INTEGER", 
					"Failed to get kernel work-group info, error code: " <<err_code);
			return false;
		}

		// Execute the kernel over the vector using the 
		// maximum number of work group items for this device
		err_code = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &global, &local,
																			0, NULL, NULL);
		if(err_code != CL_SUCCESS) {
			DEBUG_LOG_ERROR("COMPUTE INTEGER", err_code);
			return false;
		}
	
		// Wait for all commands to complete
		clFinish(commands);

		// Read back the results from the device to verify the output
		err_code = clEnqueueReadBuffer(commands, output, CL_TRUE, 0, 
															sizeof(float) * size, result, 0, NULL, NULL);	
		if(err_code != CL_SUCCESS) {
			DEBUG_LOG_ERROR("COMPUTE INTEGER", err_code);
			return false;
		}
		clReleaseMemObject(input);
		clReleaseMemObject(output);

		return true;
	}

	bool ComputeUnit::ComputeDouble(const double* data,
																	double* result,
																	const size_t& size) {
		if(data == NULL) {
			DEBUG_LOG_STATUS("Compute Integer", "Input data is null");
			return false;
		}
		int err_code;

		size_t global, local;

		cl_mem input;
		cl_mem output;

		// Create the device memory vectors
		input = clCreateBuffer(context, CL_MEM_READ_ONLY, 
													sizeof(double) * size, NULL, NULL);
		output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 
														sizeof(double) * size, NULL, NULL);
		if(!input || !output) {
			DEBUG_LOG_STATUS("COMPUTE INTEGER", 
											"Failed to acclocate the input or output");
			return false;
		}

		// Transfer the input vector into device memory
		err_code = clEnqueueWriteBuffer(commands, input, CL_TRUE, 0, 
																		sizeof(double) * size,
																		data, 0, NULL, NULL);
		if(err_code != CL_SUCCESS) {
			DEBUG_LOG_STATUS("COMPUTE INTEGER", 
								"Failed to enqueue write buffer, error code is" << err_code);
			return false;
		}

		// Set the arguments to the compute kernel
		err_code = 0;
		err_code = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input);
		err_code |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &output);
		err_code |= clSetKernelArg(kernel, 2, sizeof(size_t), &size);

		if(err_code != CL_SUCCESS) {
			DEBUG_LOG_STATUS("COMPUTE INTEGER", 
				  "Failed to set kernel args, error code: " << err_code);
			return false;
		}

		global = size;

		// Get the maximum work group size for executing the kernel on the device
		err_code = clGetKernelWorkGroupInfo(kernel, device_id, 
																				CL_KERNEL_WORK_GROUP_SIZE, sizeof(local),
																				&local, NULL);
		if(err_code != CL_SUCCESS) {
			DEBUG_LOG_STATUS("COMPUTE INTEGER", 
					"Failed to get kernel work-group info, error code: " <<err_code);
			return false;
		}

		// Execute the kernel over the vector using the 
		// maximum number of work group items for this device
		err_code = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &global, &local,
																			0, NULL, NULL);
		if(err_code != CL_SUCCESS) {
			DEBUG_LOG_ERROR("COMPUTE INTEGER", err_code);
			return false;
		}
	
		// Wait for all commands to complete
		clFinish(commands);

		// Read back the results from the device to verify the output
		err_code = clEnqueueReadBuffer(commands, output, CL_TRUE, 0, 
															sizeof(double) * size, result, 0, NULL, NULL);	
		if(err_code != CL_SUCCESS) {
			DEBUG_LOG_ERROR("COMPUTE INTEGER", err_code);
			return false;
		}
		clReleaseMemObject(input);
		clReleaseMemObject(output);

		return true;
	}


	//Get the memory's type and then compute the memory in different way
	//according different type
	//Compute##Memory is compute different data type memory
	bool ComputeUnit::ComputeIntegerMemory(base::SharedMemoryHandle& handle,
																				 const size_t& size,
																			   int* _retval) {
		int* data = static_cast<int*>(ConvertMemoryToArray(handle));
		_retval = new int[size];
		if(data == NULL) {
			DEBUG_LOG_STATUS("Compute shared memory", 
							         "Failed to get array");
			return false;
		}
		if(!ComputeInteger(data, _retval, size)) {
			DEBUG_LOG_STATUS("Compute shared memory", 
							         "Failed to compute the memory");
			return false;
		}
		return true;
	}

	bool ComputeUnit::ComputeFloatMemory(base::SharedMemoryHandle& handle,
																			 const size_t& size,
																			 float* _retval) {
		float* data = static_cast<float*>(ConvertMemoryToArray(handle));
		_retval = new float[size];
		if(data == NULL) {
			DEBUG_LOG_STATUS("Compute shared memory", 
							         "Failed to get array");
			return false;
		}
		if(!ComputeFloat(data, _retval, size)) {
			DEBUG_LOG_STATUS("Compute shared memory", 
							         "Failed to compute the memory");
			return false;
		}
		return true;
	}
	bool ComputeUnit::ComputeDoubleMemory(base::SharedMemoryHandle& handle,
																				 const size_t& size,
																			   double* _retval) {
		double* data = static_cast<double*>(ConvertMemoryToArray(handle));
		_retval = new double[size];
		if(data == NULL) {
			DEBUG_LOG_STATUS("Compute shared memory", 
							         "Failed to get array");
			return false;
		}
		if(!ComputeDouble(data, _retval, size)) {
			DEBUG_LOG_STATUS("Compute shared memory", 
							         "Failed to compute the memory");
			return false;
		}
		return true;
	}
}

