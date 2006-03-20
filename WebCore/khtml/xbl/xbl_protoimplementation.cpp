#ifndef KHTML_NO_XBL

#include "config.h"
#include "xbl_protoimplementation.h"
#include "xbl_protobinding.h"

using WebCore::String;

namespace XBL
{

XBLPrototypeImplementation::XBLPrototypeImplementation(const WebCore::String& name, XBLPrototypeBinding* binding)
:m_name(name), m_binding(binding), m_member(0), m_compiled(false)
{

}


XBLPrototypeMember::XBLPrototypeMember(const String& name)
:m_name(name), m_next(0)
{
}

void XBLPrototypeMember::appendData(const WebCore::String& data)
{
    m_data += data;
}

XBLPrototypeMethod::XBLPrototypeMethod(const WebCore::String& name)
:XBLPrototypeMember(name)
{
}

bool XBLPrototypeMethod::isConstructor() const
{
    return false;
}

bool XBLPrototypeMethod::isDestructor() const
{
    return false;
}

void XBLPrototypeMethod::addParameter(const WebCore::String& name)
{
    XBLPrototypeParameter* last = 0;
    for (XBLPrototypeParameter* curr = m_parameter; curr; curr = curr->next())
        last = curr;
    if (last)
        m_parameter = new XBLPrototypeParameter(name);
    else
        last->setNext(new XBLPrototypeParameter(name));
}

XBLPrototypeConstructor::XBLPrototypeConstructor()
:XBLPrototypeMethod("_constructor")
{}

bool XBLPrototypeConstructor::isConstructor() const
{
    return true;
}

XBLPrototypeDestructor::XBLPrototypeDestructor()
:XBLPrototypeMethod("_destructor")
{}

bool XBLPrototypeDestructor::isDestructor() const
{
    return true;
}

XBLPrototypeField::XBLPrototypeField(const WebCore::String& name, bool readonly)
:XBLPrototypeMember(name), m_readonly(readonly)
{
}

XBLPrototypeProperty::XBLPrototypeProperty(const WebCore::String& name, bool readonly, 
                                           const WebCore::String& onget,
                                           const WebCore::String& onset)
:XBLPrototypeMember(name), m_setter(onset), m_readonly(readonly)
{
    m_data = onget;
}

void XBLPrototypeProperty::appendGetterText(const WebCore::String& text)
{
    return appendData(text);
}

void XBLPrototypeProperty::appendSetterText(const WebCore::String& text)
{
    m_setter += text;
}

}

#endif // KHTML_NO_XBL
