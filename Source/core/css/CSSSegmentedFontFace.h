/*
 * Copyright (C) 2008 Apple Inc. All rights reserved.
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

#ifndef CSSSegmentedFontFace_h
#define CSSSegmentedFontFace_h

#include "core/platform/graphics/FontTraitsMask.h"
#include "wtf/HashMap.h"
#include "wtf/PassRefPtr.h"
#include "wtf/RefCounted.h"
#include "wtf/Vector.h"

namespace WebCore {

class CSSFontFace;
class CSSFontSelector;
class FontData;
class FontDescription;
class FontFace;
class SegmentedFontData;

class CSSSegmentedFontFace : public RefCounted<CSSSegmentedFontFace> {
public:
    static PassRefPtr<CSSSegmentedFontFace> create(CSSFontSelector* selector, FontTraitsMask traitsMask, bool isLocalFallback) { return adoptRef(new CSSSegmentedFontFace(selector, traitsMask, isLocalFallback)); }
    ~CSSSegmentedFontFace();

    CSSFontSelector* fontSelector() const { return m_fontSelector; }
    FontTraitsMask traitsMask() const { return m_traitsMask; }
    bool isLocalFallback() const { return m_isLocalFallback; }

    void fontLoaded(CSSFontFace*);

    void appendFontFace(PassRefPtr<CSSFontFace>);

    PassRefPtr<FontData> getFontData(const FontDescription&);

#if ENABLE(SVG_FONTS)
    bool hasSVGFontFaceSource() const;
#endif

    class LoadFontCallback : public RefCounted<LoadFontCallback> {
    public:
        virtual ~LoadFontCallback() { }
        virtual void notifyLoaded(CSSSegmentedFontFace*) = 0;
        virtual void notifyError(CSSSegmentedFontFace*) = 0;
    };

    bool checkFont() const;
    void loadFont(const FontDescription&, PassRefPtr<LoadFontCallback> loadCallback);
    Vector<RefPtr<FontFace> > fontFaces() const;
    void willUseFontData(const FontDescription&);

private:
    CSSSegmentedFontFace(CSSFontSelector*, FontTraitsMask, bool isLocalFallback);

    void pruneTable();
    bool isValid() const;
    bool isLoading() const;

    CSSFontSelector* m_fontSelector;
    FontTraitsMask m_traitsMask;
    bool m_isLocalFallback;
    HashMap<unsigned, RefPtr<SegmentedFontData> > m_fontDataTable;
    Vector<RefPtr<CSSFontFace>, 1> m_fontFaces;
    Vector<RefPtr<LoadFontCallback> > m_callbacks;
};

} // namespace WebCore

#endif // CSSSegmentedFontFace_h
