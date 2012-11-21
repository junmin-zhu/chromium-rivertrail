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

#ifndef CONTENT_RENDERER_RIVERTRAIL_HANDLE_DATA_H_
#define CONTENT_RENDERER_RIVERTRAIL_HANDLE_DATA_H_

#include "base/supports_user_data.h"
#include "content_export.h"
/*
namespace WebKit {
class WebSocketStreamHandle;
}
*/
// User data stored in each WebSocketStreamHandleImpl.
class RivertrailHandleData : public base::SupportsUserData::Data {
 public:
  virtual ~RivertrailHandleData() {}

  int render_view_id() const { return render_view_id_; }

  // Creates RivertrailHandleData object with |render_view_id| and store it
  // to |handle|.
  static void AddToHandle(
      WebKit::WebRivertrailHandle* handle, int render_view_id);

  // Retrieves the stored user data from WebKit::WebRivertrailHandle object.
  // |handle| must actually be a WebRivertrailHandleImpl object.
  CONTENT_EXPORT static const RivertrailHandleData* ForHandle(
      WebKit::WebRivertrailHandle* handle);

 private:
  explicit RivertrailHandleData(int render_view_id)
      : render_view_id_(render_view_id) {
  }

  int render_view_id_;

  DISALLOW_COPY_AND_ASSIGN(RivertrailHandleData);
};

#endif  // CONTENT_RENDERER_RIVERTRAIL_HANDLE_DATA_H_
