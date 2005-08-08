/*
    Copyright (C) 2004 Nikolas Zimmermann <wildfox@kde.org>
                  2004 Rob Buis <buis@kde.org>

    This file is part of the KDE project

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/

#include <kdom/ecma/Ecma.h>

#include "SVGException.h"
#include "SVGExceptionImpl.h"
#include "SVGElementImpl.h"
#include "SVGAnimatedAngle.h"
#include "SVGAnimatedAngleImpl.h"

#include "SVGConstants.h"
#include "SVGAnimatedAngle.lut.h"
using namespace KSVG;

/*
@begin SVGAnimatedAngle::s_hashTable 3
 baseVal	SVGAnimatedAngleConstants::BaseVal	DontDelete|ReadOnly
 animVal	SVGAnimatedAngleConstants::AnimVal	DontDelete|ReadOnly
@end
*/

ValueImp *SVGAnimatedAngle::getValueProperty(ExecState *exec, int token) const
{
	KDOM_ENTER_SAFE

	switch(token)
	{
		case SVGAnimatedAngleConstants::BaseVal:
			return KDOM::safe_cache<SVGAngle>(exec, baseVal());
		case SVGAnimatedAngleConstants::AnimVal:
			return KDOM::safe_cache<SVGAngle>(exec, animVal());
		default:
			kdWarning() << "Unhandled token in " << k_funcinfo << " : " << token << endl;
	}

	KDOM_LEAVE_SAFE(SVGException)
	return Undefined();
}

SVGAnimatedAngle SVGAnimatedAngle::null;

SVGAnimatedAngle::SVGAnimatedAngle() : impl(0)
{
}

SVGAnimatedAngle::SVGAnimatedAngle(SVGAnimatedAngleImpl *i) : impl(i)
{
	if(impl)
		impl->ref();
}

SVGAnimatedAngle::SVGAnimatedAngle(const SVGAnimatedAngle &other) : impl(0)
{
	(*this) = other;
}

KSVG_IMPL_DTOR_ASSIGN_OP(SVGAnimatedAngle)

SVGAngle SVGAnimatedAngle::baseVal() const
{
	if(!impl)
		return SVGAngle::null;

	return SVGAngle(impl->baseVal());
}

SVGAngle SVGAnimatedAngle::animVal() const
{
	if(!impl)
		return SVGAngle::null;

	return SVGAngle(impl->animVal());
}

// vim:ts=4:noet
