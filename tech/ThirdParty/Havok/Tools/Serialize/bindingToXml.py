#
# Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
# prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
# Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
#

#! /usr/bin/env python

###
### This file converts a python xml data binding back into xml.
### See hkcToDom for an example data binding.
###
__all__ = ["bindingToXml"]

from xml.sax.saxutils import escape

def _hierarchy(klass):
    ret = []
    todo = [klass]
    while todo:
        c = todo.pop(0)
        ret.append(c)
        todo.extend([b for b in c.__bases__ if b!=object])
    ret.reverse()
    return ret

def writeNode(out, name, node, level, seen):
    if not seen.has_key(node):
        seen[node] = 1
        out.append("%s<%s>" % ("\t"*level, name))
        level += 1
        for klass in _hierarchy(node.__class__):
            for slot in klass.__slots__:
                value = getattr(node,slot)
                writeItem(out, slot.lstrip("_"), value, level, seen)
        level -= 1
        out.append("%s</%s>" % ("\t"*level, name))

def writeItem(out, name, item, level, seen):
    if type(item) in (str, unicode, long, int, bool, tuple, dict ):
        if len(str(item)):
            out.append("%s<%s>%s</%s>" % ("\t"*level, name, escape(str(item)), name))
    elif type(item) == type([]):
        for i in item:
            writeItem(out, name, i, level, seen)
    elif item:
        writeNode(out, name, item, level, seen)

def bindingToXml(toplevel, binding):
    """Get the xml representation of "binding" as a string.
    The root element is named elementName.
    """
    out = []
    #class X: def append(self, x): print x # uncomment these two lines to print
    #out = X()
    already_seen = {} # prevent infinite recursion
    writeItem(out, toplevel, binding, 0, already_seen)
    return "\n".join(out)


#
# Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20091222)
# 
# Confidential Information of Havok.  (C) Copyright 1999-2009
# Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
# Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
# rights, and intellectual property rights in the Havok software remain in
# Havok and/or its suppliers.
# 
# Use of this software for evaluation purposes is subject to and indicates
# acceptance of the End User licence Agreement for this product. A copy of
# the license is included with this software and is also available at www.havok.com/tryhavok.
# 
#
