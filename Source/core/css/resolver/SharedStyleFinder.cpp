/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 2004-2005 Allan Sandfeld Jensen (kde@carewolf.com)
 * Copyright (C) 2006, 2007 Nicholas Shanks (webkit@nickshanks.com)
 * Copyright (C) 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013 Apple Inc. All rights reserved.
 * Copyright (C) 2007 Alexey Proskuryakov <ap@webkit.org>
 * Copyright (C) 2007, 2008 Eric Seidel <eric@webkit.org>
 * Copyright (C) 2008, 2009 Torch Mobile Inc. All rights reserved. (http://www.torchmobile.com/)
 * Copyright (c) 2011, Code Aurora Forum. All rights reserved.
 * Copyright (C) Research In Motion Limited 2011. All rights reserved.
 * Copyright (C) 2013 Google Inc. All rights reserved.
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
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "config.h"
#include "core/css/resolver/SharedStyleFinder.h"

#include "HTMLNames.h"
#include "XMLNames.h"
#include "core/css/resolver/StyleResolver.h"
#include "core/css/resolver/StyleResolverState.h"
#include "core/dom/ContainerNode.h"
#include "core/dom/Document.h"
#include "core/dom/Element.h"
#include "core/dom/ElementTraversal.h"
#include "core/dom/FullscreenElementStack.h"
#include "core/dom/Node.h"
#include "core/dom/NodeRenderStyle.h"
#include "core/dom/QualifiedName.h"
#include "core/dom/SpaceSplitString.h"
#include "core/dom/shadow/ElementShadow.h"
#include "core/html/HTMLElement.h"
#include "core/html/HTMLInputElement.h"
#include "core/html/HTMLOptGroupElement.h"
#include "core/html/track/WebVTTElement.h"
#include "core/rendering/style/RenderStyle.h"
#include "core/svg/SVGElement.h"
#include "wtf/HashSet.h"
#include "wtf/text/AtomicString.h"

namespace WebCore {

using namespace HTMLNames;

static inline bool parentElementPreventsSharing(const Element* parentElement)
{
    return parentElement->hasFlagsSetDuringStylingOfChildren();
}

bool SharedStyleFinder::canShareStyleWithControl(const ElementResolveContext& context, Element* element) const
{
    if (!element->hasTagName(inputTag) || !context.element()->hasTagName(inputTag))
        return false;

    HTMLInputElement* thisInputElement = toHTMLInputElement(element);
    HTMLInputElement* otherInputElement = toHTMLInputElement(context.element());
    if (thisInputElement->elementData() != otherInputElement->elementData()) {
        if (thisInputElement->fastGetAttribute(typeAttr) != otherInputElement->fastGetAttribute(typeAttr))
            return false;
        if (thisInputElement->fastGetAttribute(readonlyAttr) != otherInputElement->fastGetAttribute(readonlyAttr))
            return false;
    }

    if (thisInputElement->isAutofilled() != otherInputElement->isAutofilled())
        return false;
    if (thisInputElement->shouldAppearChecked() != otherInputElement->shouldAppearChecked())
        return false;
    if (thisInputElement->shouldAppearIndeterminate() != otherInputElement->shouldAppearIndeterminate())
        return false;
    if (thisInputElement->isRequired() != otherInputElement->isRequired())
        return false;

    if (element->isDisabledFormControl() != context.element()->isDisabledFormControl())
        return false;

    if (element->isDefaultButtonForForm() != context.element()->isDefaultButtonForForm())
        return false;

    if (context.document().containsValidityStyleRules()) {
        bool willValidate = element->willValidate();

        if (willValidate != context.element()->willValidate())
            return false;

        if (willValidate && (element->isValidFormControlElement() != context.element()->isValidFormControlElement()))
            return false;

        if (element->isInRange() != context.element()->isInRange())
            return false;

        if (element->isOutOfRange() != context.element()->isOutOfRange())
            return false;
    }

    return true;
}

bool SharedStyleFinder::classNamesAffectedByRules(const SpaceSplitString& classNames) const
{
    for (unsigned i = 0; i < classNames.size(); ++i) {
        if (m_features.classesInRules.contains(classNames[i].impl()))
            return true;
    }
    return false;
}

static inline bool elementHasDirectionAuto(Element* element)
{
    // FIXME: This line is surprisingly hot, we may wish to inline hasDirectionAuto into StyleResolver.
    return element->isHTMLElement() && toHTMLElement(element)->hasDirectionAuto();
}

bool SharedStyleFinder::sharingCandidateHasIdenticalStyleAffectingAttributes(const ElementResolveContext& context, Element* sharingCandidate) const
{
    if (context.element()->elementData() == sharingCandidate->elementData())
        return true;
    if (context.element()->fastGetAttribute(XMLNames::langAttr) != sharingCandidate->fastGetAttribute(XMLNames::langAttr))
        return false;
    if (context.element()->fastGetAttribute(langAttr) != sharingCandidate->fastGetAttribute(langAttr))
        return false;

    if (!m_elementAffectedByClassRules) {
        if (sharingCandidate->hasClass() && classNamesAffectedByRules(sharingCandidate->classNames()))
            return false;
    } else if (sharingCandidate->hasClass()) {
        // SVG elements require a (slow!) getAttribute comparision because "class" is an animatable attribute for SVG.
        if (context.element()->isSVGElement()) {
            if (context.element()->getAttribute(classAttr) != sharingCandidate->getAttribute(classAttr))
                return false;
        } else if (context.element()->classNames() != sharingCandidate->classNames()) {
            return false;
        }
    } else {
        return false;
    }

    if (context.element()->presentationAttributeStyle() != sharingCandidate->presentationAttributeStyle())
        return false;

    if (context.element()->hasTagName(progressTag)) {
        if (context.element()->shouldAppearIndeterminate() != sharingCandidate->shouldAppearIndeterminate())
            return false;
    }

    return true;
}

bool SharedStyleFinder::canShareStyleWithElement(const ElementResolveContext& context, Element* element) const
{
    if (context.element() == element)
        return false;
    Element* parent = element->parentElement();
    RenderStyle* style = element->renderStyle();
    if (!style)
        return false;
    if (!parent)
        return false;
    if (context.element()->parentElement()->renderStyle() != parent->renderStyle())
        return false;
    if (style->unique())
        return false;
    if (style->hasUniquePseudoStyle())
        return false;
    if (element->tagQName() != context.element()->tagQName())
        return false;
    if (element->inlineStyle())
        return false;
    if (element->needsStyleRecalc())
        return false;
    if (element->isSVGElement() && toSVGElement(element)->animatedSMILStyleProperties())
        return false;
    if (element->isLink() != context.element()->isLink())
        return false;
    if (element->hovered() != context.element()->hovered())
        return false;
    if (element->active() != context.element()->active())
        return false;
    if (element->focused() != context.element()->focused())
        return false;
    if (element->shadowPseudoId() != context.element()->shadowPseudoId())
        return false;
    if (element == element->document().cssTarget())
        return false;
    if (!sharingCandidateHasIdenticalStyleAffectingAttributes(context, element))
        return false;
    if (element->additionalPresentationAttributeStyle() != context.element()->additionalPresentationAttributeStyle())
        return false;

    if (element->hasID() && m_features.idsInRules.contains(element->idForStyleResolution().impl()))
        return false;
    if (element->hasScopedHTMLStyleChild())
        return false;
    if (isShadowHost(element) && element->shadow()->containsActiveStyles())
        return 0;

    // FIXME: We should share style for option and optgroup whenever possible.
    // Before doing so, we need to resolve issues in HTMLSelectElement::recalcListItems
    // and RenderMenuList::setText. See also https://bugs.webkit.org/show_bug.cgi?id=88405
    if (element->hasTagName(optionTag) || isHTMLOptGroupElement(element))
        return false;

    bool isControl = element->isFormControlElement();

    if (isControl != context.element()->isFormControlElement())
        return false;

    if (isControl && !canShareStyleWithControl(context, element))
        return false;

    if (style->transitions() || style->animations())
        return false;

    // Turn off style sharing for elements that can gain layers for reasons outside of the style system.
    // See comments in RenderObject::setStyle().
    if (element->hasTagName(iframeTag) || element->hasTagName(frameTag) || element->hasTagName(embedTag) || element->hasTagName(objectTag) || element->hasTagName(appletTag) || element->hasTagName(canvasTag))
        return false;

    if (elementHasDirectionAuto(element))
        return false;

    if (element->isLink() && context.elementLinkState() != style->insideLink())
        return false;

    if (element->isUnresolvedCustomElement() != context.element()->isUnresolvedCustomElement())
        return false;

    // Deny sharing styles between WebVTT and non-WebVTT nodes.
    if (element->isWebVTTElement() != context.element()->isWebVTTElement())
        return false;

    if (element->isWebVTTElement() && context.element()->isWebVTTElement() && toWebVTTElement(element)->isPastNode() != toWebVTTElement(context.element())->isPastNode())
        return false;

    if (FullscreenElementStack* fullscreen = FullscreenElementStack::fromIfExists(&context.document())) {
        if (element == fullscreen->webkitCurrentFullScreenElement() || context.element() == fullscreen->webkitCurrentFullScreenElement())
            return false;
    }

    if (context.element()->parentElement() != parent) {
        if (!parent->isStyledElement())
            return false;
        if (parent->hasScopedHTMLStyleChild())
            return false;
        if (parent->inlineStyle())
            return false;
        if (parent->isSVGElement() && toSVGElement(parent)->animatedSMILStyleProperties())
            return false;
        if (parent->hasID() && m_features.idsInRules.contains(parent->idForStyleResolution().impl()))
            return false;
        if (parentElementPreventsSharing(parent))
            return false;
    }

    return true;
}

#ifdef STYLE_STATS
Element* SharedStyleFinder::searchDocumentForSharedStyle(const ElementResolveContext& context) const
{
    for (Element* element = context.element()->document().documentElement(); element; element = ElementTraversal::next(element)) {
        if (canShareStyleWithElement(context, element))
            return element;
    }
    return 0;
}
#endif

inline Element* SharedStyleFinder::findElementForStyleSharing(const ElementResolveContext& context) const
{
    StyleSharingList& styleSharingList = m_styleResolver->styleSharingList();
    for (StyleSharingList::iterator it = styleSharingList.begin(); it != styleSharingList.end(); ++it) {
        if (!canShareStyleWithElement(context, it->get()))
            continue;
        Element* element = it->get();
        if (it != styleSharingList.begin()) {
            // Move the element to the front of the LRU
            styleSharingList.remove(it);
            styleSharingList.prepend(element);
        }
        return element;
    }
    m_styleResolver->addToStyleSharingList(context.element());
    return 0;
}

RenderStyle* SharedStyleFinder::locateSharedStyle(const ElementResolveContext& context, RenderStyle* newStyle)
{
    STYLE_STATS_ADD_SEARCH();

    if (!m_styleResolver->supportsStyleSharing(context.element()))
        return 0;

    STYLE_STATS_ADD_ELEMENT_ELIGIBLE_FOR_SHARING();

    // Cache whether context.element() is affected by any known class selectors.
    // FIXME: This should be an explicit out parameter, instead of a member variable.
    m_elementAffectedByClassRules = context.element() && context.element()->hasClass() && classNamesAffectedByRules(context.element()->classNames());

    Element* shareElement = findElementForStyleSharing(context);

#ifdef STYLE_STATS
    // FIXME: these stats don't to into account whether or not sibling/attribute
    // rules prevent these nodes from actually sharing
    if (shareElement) {
        STYLE_STATS_ADD_SEARCH_FOUND_SIBLING_FOR_SHARING();
    } else {
        shareElement = searchDocumentForSharedStyle(context);
        if (shareElement)
            STYLE_STATS_ADD_SEARCH_MISSED_SHARING();
        shareElement = 0;
    }
#endif

    // If we have exhausted all our budget or our cousins.
    if (!shareElement)
        return 0;

    // Can't share if sibling rules apply. This is checked at the end as it should rarely fail.
    if (m_styleResolver->styleSharingCandidateMatchesRuleSet(context, newStyle, m_siblingRuleSet))
        return 0;
    // Can't share if attribute rules apply.
    if (m_styleResolver->styleSharingCandidateMatchesRuleSet(context, newStyle, m_uncommonAttributeRuleSet))
        return 0;
    // Tracking child index requires unique style for each node. This may get set by the sibling rule match above.
    if (parentElementPreventsSharing(context.element()->parentElement()))
        return 0;
    STYLE_STATS_ADD_STYLE_SHARED();
    return shareElement->renderStyle();
}

}
