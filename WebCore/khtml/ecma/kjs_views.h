// -*- c-basic-offset: 2 -*-
/*
 *  This file is part of the KDE libraries
 *  Copyright (C) 2001 Peter Kelly (pmk@post.com)
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef KJS_VIEWS_H_
#define KJS_VIEWS_H_

#include "kjs_dom.h"

namespace WebCore {
    class AbstractView;
}

namespace KJS {

  class DOMAbstractView : public DOMObject {
  public:
    DOMAbstractView(ExecState *, WebCore::AbstractView *av);
    ~DOMAbstractView();
    virtual bool getOwnPropertySlot(ExecState *, const Identifier&, PropertySlot&);
    JSValue *getValueProperty(ExecState *exec, int token);
    // no put - all read-only
    virtual const ClassInfo* classInfo() const { return &info; }
    static const ClassInfo info;
    WebCore::AbstractView *impl() const { return m_impl.get(); }
    enum { Document_, GetComputedStyle, GetMatchedCSSRules };
  private:
    RefPtr<WebCore::AbstractView> m_impl;
  };

  JSValue* toJS(ExecState*, WebCore::AbstractView*);

  WebCore::AbstractView *toAbstractView(JSValue *);

} // namespace

#endif
