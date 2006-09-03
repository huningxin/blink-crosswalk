/*
 * Copyright (C) 2004-2006 Apple Computer, Inc.  All rights reserved.
 * Copyright (C) 2006 Samuel Weinig (sam.weinig@gmail.com)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#import "config.h"
#import "DOMRGBColor.h"

#import "DOMInternal.h"
#import "DOMCSSInternal.h"
#import <wtf/GetPtr.h>
#import "Color.h"
#import "DOMCSSPrimitiveValue.h"
#import "CSSPrimitiveValue.h"

static CFMutableDictionaryRef wrapperCache = NULL;

id getWrapperForRGB(WebCore::RGBA32 value)
{
    if (!wrapperCache)
        return nil;
    return (id)CFDictionaryGetValue(wrapperCache, reinterpret_cast<const void*>(value));
}

void setWrapperForRGB(id wrapper, WebCore::RGBA32 value)
{
    if (!wrapperCache) {
        // No need to retain/free either impl key, or id value.  Items will be removed
        // from the cache in dealloc methods.
        wrapperCache = CFDictionaryCreateMutable(NULL, 0, NULL, NULL);
    }
    CFDictionarySetValue(wrapperCache, reinterpret_cast<const void*>(value), wrapper);
}

void removeWrapperForRGB(WebCore::RGBA32 value)
{
    if (!wrapperCache)
        return;
    CFDictionaryRemoveValue(wrapperCache, reinterpret_cast<const void*>(value));
}


@implementation DOMRGBColor

- (void)dealloc
{
    removeWrapperForRGB(reinterpret_cast<uintptr_t>(_internal));
    [super dealloc];
}

- (void)finalize
{
    removeWrapperForRGB(reinterpret_cast<uintptr_t>(_internal));
    [super finalize];
}

- (DOMCSSPrimitiveValue *)red
{
    WebCore::RGBA32 rgb = reinterpret_cast<uintptr_t>(_internal);
    int value = (rgb >> 16) & 0xFF;
    return [DOMCSSPrimitiveValue _valueWith:new WebCore::CSSPrimitiveValue(value, WebCore::CSSPrimitiveValue::CSS_NUMBER)];
}

- (DOMCSSPrimitiveValue *)green
{
    WebCore::RGBA32 rgb = reinterpret_cast<uintptr_t>(_internal);
    int value = (rgb >> 8) & 0xFF;
    return [DOMCSSPrimitiveValue _valueWith:new WebCore::CSSPrimitiveValue(value, WebCore::CSSPrimitiveValue::CSS_NUMBER)];
}

- (DOMCSSPrimitiveValue *)blue
{
    WebCore::RGBA32 rgb = reinterpret_cast<uintptr_t>(_internal);
    int value = rgb & 0xFF;
    return [DOMCSSPrimitiveValue _valueWith:new WebCore::CSSPrimitiveValue(value, WebCore::CSSPrimitiveValue::CSS_NUMBER)];
}

@end

@implementation DOMRGBColor (DOMRGBColorExtensions)

- (DOMCSSPrimitiveValue *)alpha
{
    WebCore::RGBA32 rgb = reinterpret_cast<uintptr_t>(_internal);
    float value = static_cast<float>(WebCore::Color(rgb).alpha()) / 0xFF;
    return [DOMCSSPrimitiveValue _valueWith:new WebCore::CSSPrimitiveValue(value, WebCore::CSSPrimitiveValue::CSS_NUMBER)];
    
}

- (NSColor *)color
{
    WebCore::RGBA32 rgb = reinterpret_cast<uintptr_t>(_internal);
    return nsColor(WebCore::Color(rgb));
}

@end

@implementation DOMRGBColor (WebCoreInternal)

- (id)_initWithRGB:(WebCore::RGBA32)value
{
    [super _init];
    _internal = reinterpret_cast<DOMObjectInternal*>(value);
    setWrapperForRGB(self, value);
    return self;
}

+ (DOMRGBColor *)_RGBColorWithRGB:(WebCore::RGBA32)value
{
    id cachedInstance;
    cachedInstance = getWrapperForRGB(value);
    if (cachedInstance)
        return [[cachedInstance retain] autorelease];
    
    return [[[self alloc] _initWithRGB:value] autorelease];
}

@end
