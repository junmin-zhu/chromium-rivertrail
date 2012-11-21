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

// Multiply-included message file, hence no include guard.
#include <vector>

#include "content/common/content_export.h"
#include "googleurl/src/gurl.h"
#include "ipc/ipc_message_macros.h"
#include "ipc/ipc_param_traits.h"

#undef IPC_MESSAGE_EXPORT
#define IPC_MESSAGE_EXPORT CONTENT_EXPORT
#define IPC_MESSAGE_START RivertrailMsgStart


// RiverTrail messages sent from the renderer to the browser.
IPC_MESSAGE_CONTROL0(RivertrailHostMsg_SendInit)

//IPC_MESSAGE_CONTROL3(RivertrailHostMsg_SendArray, 
//                     std::vector<type> /* array */, 
//                     TYPE /* data type */,  
//                     operator /* like map, combine, and so on */,
//                     ValueList /* The type list of the operator */)

//IPC_MESSAGE_CONTROL3(RivertrailHostMsg_SendBuffer,
//                     SharedMemoryHandle /* data */,
//                     int /* size */,
//                     operator /* like map, combine, and so on */,
//                     ValueList /* The type list of the operator */)


// RiverTrail messages sent from the browser to the renderer.

IPC_MESSAGE_CONTROL1(RivertrailMsg_SendInfo,
                     char*	/*The information of platform, context and so on*/)

//IPC_MESSAGE_CONTROL4(RivertrailMsg_SendArray,
//                     std::vector<char>,
//                     TYPE,
//                     operator,
//                     ValueList )

//IPC_MESSAGE_CONTROL2(RivertrailMsg_SendBuffer,
//                     SharedMemoryHandle,  /*data*/
//                     int /*size*/)
