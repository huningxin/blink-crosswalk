/*
 * Copyright (C) 2001 Apple Computer, Inc.  All rights reserved.
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

#ifndef KCHARSETS_H_
#define KCHARSETS_H_

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <qfont.h>
#include <qstring.h>

class QTextCodec;

// class KCharsets =============================================================

class KCharsets {
public:

    // constructors, copy constructors, and destructors ------------------------

    KCharsets();
    ~KCharsets();

    // member functions --------------------------------------------------------

    QTextCodec *codecForName(const QString &) const;
    QTextCodec *codecForName(const QString &, bool &) const;

    QFont::CharSet charsetForEncoding(const QString &) const;
    QFont::CharSet charsetForEncoding(const QString &, bool) const;

    void setQFont(QFont &, QFont::CharSet) const;
    void setQFont(QFont &, QString) const;

    QString name(QFont::CharSet);
    QString xCharsetName(QFont::CharSet) const;

    bool supportsScript(const QFont &, QFont::CharSet);

}; // class KCharsets ==========================================================

#endif
