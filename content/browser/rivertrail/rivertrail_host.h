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

#ifndef CONTENT_BROWSER_RENDERER_HOST_RIVERTRAIL_HOST_H_
#define CONTENT_BROWSER_RENDERER_HOST_RIVERTRAIL_HOST_H_

#include <vector>

#include "base/memory/ref_counted.h"
#include "base/shared_memory.h"
#include "net/socket_stream/socket_stream.h"

class GURL;

namespace net {
class SocketStreamJob;
class URLRequestContext;
class SSLInfo;
}  // namespace net
*/
// Host of RivertrailHandle.
// Each RivertrailHandle will have an unique socket_id assigned by
// RivertrailHost constructor. If socket id is content::kNoSocketId,
// there is no RivertrailHost.
// The lifetime of an instance of this class is completely controlled by the
// RivertrailDispatcherHost.
class SocketStreamHost {
 public:
  SocketStreamHost(net::SocketStream::Delegate* delegate,
                   int render_view_id,
                   int socket_id);
  ~SocketStreamHost();

  // Gets socket_id associated with |socket|.
  static int SocketIdFromSocketStream(net::SocketStream* socket);

  int render_view_id() const { return render_view_id_; }
  int socket_id() const { return socket_id_; }

  // Starts to open connection to |url|.
  void Connect(const GURL& url, net::URLRequestContext* request_context);

  // Sends |data| over the socket stream.
  // socket stream must be open to send data.
  // Returns true if the data is put in transmit buffer in socket stream.
  // Returns false otherwise (transmit buffer exceeds limit, or socket
  // stream is closed).
  bool SendData(const std::vector<char>& data);
	bool SendData(const base::SharedMemoryHandle& handle,
								

  // Closes the socket stream.
  void Close();

  // Following CancelWithError, CancelWithSSLError, and ContinueDespiteError
  // will be called by net::SocketStream::Delegate in OnSSLCertificateError.
  // CancelWithError Cancels the connection because of an error.
  // |error| is net::Error which represents the error.
  void CancelWithError(int error);

  // Cancels the connection because of receiving a certificate with an error.
  void CancelWithSSLError(const net::SSLInfo& ssl_info);

  // Continue to establish the connection in spite of an error.
  void ContinueDespiteError();

 private:
  net::SocketStream::Delegate* delegate_;
  int render_view_id_;
  int socket_id_;

  scoped_refptr<net::SocketStreamJob> socket_;

  DISALLOW_COPY_AND_ASSIGN(SocketStreamHost);
};

#endif  // CONTENT_BROWSER_RENDERER_HOST_RIVERTRAIL_HOST_H_
