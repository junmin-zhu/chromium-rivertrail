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
/*
#include "content/browser/renderer_host/socket_stream_host.h"

#include "base/logging.h"
#include "content/common/socket_stream.h"
#include "net/socket_stream/socket_stream_job.h"

static const char* kSocketIdKey = "socketId";

class SocketStreamId : public net::SocketStream::UserData {
 public:
  explicit SocketStreamId(int socket_id) : socket_id_(socket_id) {}
  virtual ~SocketStreamId() {}
  int socket_id() const { return socket_id_; }

 private:
  int socket_id_;
};

SocketStreamHost::SocketStreamHost(
    net::SocketStream::Delegate* delegate,
    int render_view_id,
    int socket_id)
    : delegate_(delegate),
      render_view_id_(render_view_id),
      socket_id_(socket_id) {
  DCHECK_NE(socket_id_, content::kNoSocketId);
  VLOG(1) << "SocketStreamHost: render_view_id=" << render_view_id
          << " socket_id=" << socket_id_;
}

/* static *//*
int SocketStreamHost::SocketIdFromSocketStream(net::SocketStream* socket) {
  net::SocketStream::UserData* d = socket->GetUserData(kSocketIdKey);
  if (d) {
    SocketStreamId* socket_stream_id = static_cast<SocketStreamId*>(d);
    return socket_stream_id->socket_id();
  }
  return content::kNoSocketId;
}

SocketStreamHost::~SocketStreamHost() {
  VLOG(1) << "SocketStreamHost destructed socket_id=" << socket_id_;
  socket_->DetachDelegate();
}

void SocketStreamHost::Connect(const GURL& url,
                               net::URLRequestContext* request_context) {
  VLOG(1) << "SocketStreamHost::Connect url=" << url;
  socket_ = net::SocketStreamJob::CreateSocketStreamJob(
      url, delegate_, request_context->transport_security_state(),
      request_context->ssl_config_service());
  socket_->set_context(request_context);
  socket_->SetUserData(kSocketIdKey, new SocketStreamId(socket_id_));
  socket_->Connect();
}

bool SocketStreamHost::SendData(const std::vector<char>& data) {
  VLOG(1) << "SocketStreamHost::SendData";
  return socket_ && socket_->SendData(&data[0], data.size());
}

void SocketStreamHost::Close() {
  VLOG(1) << "SocketStreamHost::Close";
  if (!socket_)
    return;
  socket_->Close();
}

void SocketStreamHost::CancelWithError(int error) {
  VLOG(1) << "SocketStreamHost::CancelWithError: error=" << error;
  if (!socket_)
    return;
  socket_->CancelWithError(error);
}

void SocketStreamHost::CancelWithSSLError(const net::SSLInfo& ssl_info) {
  VLOG(1) << "SocketStreamHost::CancelWithSSLError";
  if (!socket_)
    return;
  socket_->CancelWithSSLError(ssl_info);
}

void SocketStreamHost::ContinueDespiteError() {
  VLOG(1) << "SocketStreamHost::ContinueDespiteError";
  if (!socket_)
    return;
  socket_->ContinueDespiteError();
}*/
