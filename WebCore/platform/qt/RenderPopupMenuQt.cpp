/*
 * This file is part of the popup menu implementation for <select> elements in WebCore.
 *
 * Copyright (C) 2006 Apple Computer, Inc.
 * Copyright (C) 2006 Michael Emmel mike.emmel@gmail.com 
 * Coypright (C) 2006 Nikolas Zimmermann <zimmermann@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */

#include "config.h"
#include "RenderPopupMenuQt.h"

#define notImplemented() do { fprintf(stderr, "FIXME: UNIMPLEMENTED: %s:%d\n", __FILE__, __LINE__); } while(0)

namespace WebCore {

RenderPopupMenuQt::RenderPopupMenuQt(Node* element, RenderMenuList* menuList)
    : RenderPopupMenu(element, menuList)
{
}

RenderPopupMenuQt::~RenderPopupMenuQt()
{
}

void RenderPopupMenuQt::clear()
{
    notImplemented();
}

void RenderPopupMenuQt::populate()
{
    notImplemented();
}

void RenderPopupMenuQt::showPopup(const IntRect&, FrameView*, int)
{
    notImplemented();
}

void RenderPopupMenuQt::hidePopup()
{
    notImplemented();
}

void RenderPopupMenuQt::addSeparator()
{
    notImplemented();
}

void RenderPopupMenuQt::addGroupLabel(HTMLOptGroupElement*)
{
    notImplemented();
}

void RenderPopupMenuQt::addOption(HTMLOptionElement*)
{
    notImplemented();
}

}

// vim: ts=4 sw=4 et
