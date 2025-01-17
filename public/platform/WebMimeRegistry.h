/*
 * Copyright (C) 2009 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef WebMimeRegistry_h
#define WebMimeRegistry_h

#include "WebCommon.h"

namespace WebKit {
class WebString;

class WebMimeRegistry {
public:
    enum SupportsType { IsNotSupported, IsSupported, MayBeSupported };

    virtual SupportsType supportsMIMEType(const WebKit::WebString& mimeType) = 0;
    virtual SupportsType supportsImageMIMEType(const WebKit::WebString& mimeType) = 0;
    virtual SupportsType supportsJavaScriptMIMEType(const WebKit::WebString& mimeType) = 0;
    // FIXME(82983): Remove this method and make the next one pure-virtual after committing the Chromium code.
    virtual SupportsType supportsMediaMIMEType(const WebKit::WebString& mimeType,
                                               const WebKit::WebString& codecs) = 0;
    virtual SupportsType supportsMediaMIMEType(const WebKit::WebString& mimeType,
                                               const WebKit::WebString& codecs,
                                               const WebKit::WebString& keySystem) {
        return supportsMediaMIMEType(mimeType, codecs);
    }

    virtual bool supportsMediaSourceMIMEType(const WebKit::WebString& mimeType, const WebKit::WebString& codecs) = 0;

    virtual SupportsType supportsNonImageMIMEType(const WebKit::WebString& mimeType) = 0;

    virtual WebKit::WebString mimeTypeForExtension(const WebKit::WebString& fileExtension) = 0;
    virtual WebKit::WebString wellKnownMimeTypeForExtension(const WebKit::WebString& fileExtension) = 0;
    virtual WebKit::WebString mimeTypeFromFile(const WebKit::WebString& filePath) = 0;

protected:
    ~WebMimeRegistry() { }
};

} // namespace WebKit

#endif
