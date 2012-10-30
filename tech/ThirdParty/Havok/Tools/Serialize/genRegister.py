#
# Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
# prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
# Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
#

#! /usr/bin/env python

###
### This script is used to generate a list of hkClass names which
### can be used to register hkClasses and their typeinfos.
###

import os
import re
import headerToDom
import util

TKBMS = \
"""// TKBMS v1.0 -----------------------------------------------------
//
// PLATFORM       : ALL
// PRODUCT        : %s
// VISIBILITY : PUBLIC
//
// ------------------------------------------------------TKBMS v1.0

// Autogenerated by genRegister.py

"""

re_PREFIX = re.compile( 'setPrefix\(\"(?P<prefix>.*)\"\)' )
re_PRODUCT = re.compile( '//\s*PRODUCT\s*:\s*(?P<product>.*)\s*' )

def getModuleName( pathToModule ):
    settingsFile = os.path.join(pathToModule, 'settings.build')
    if os.path.isfile(settingsFile):
        settingsText = open( settingsFile ).read()
        modulePrefix = re_PREFIX.search(settingsText).group('prefix')
        return modulePrefix + os.path.basename(pathToModule)
    return os.path.basename(pathToModule)

def findClassesAndLocations(where):
    """Return list of class names and dict of Class.cpp locations for each class"""
    classes = []
    locations = {}

    def appendClasses( fileName, classList, prefix ):
        for c in classList:
            c.name = prefix + c.name
            classes.append( c )
            locations[ c.name ] = fileName
            appendClasses( fileName, c._class, c.name )

    def findCppHeaders(top):
        for dirname, subdirs, files in os.walk(top):
            if not re.search('\\bUnitTest\\b', dirname):
                for f in  [ f for f in files if f.endswith(".h") ]:
                    yield os.path.join(dirname, f)

    for fileName in findCppHeaders(where):
        if util.hasReflectionDeclaration( open(fileName).read() ):
            dom = headerToDom.headerToDom( fileName )
            if dom.file.product != "NONE" and dom.file.platform != "NONE":
                filename = dom.file.destinationFilename(dom.localfilename, "Class.cpp")
                appendClasses( filename, dom.file._class, '' )                    
        
    return classes, locations
    
def getClassList(classes, cdict):
    ret = []
    def hasVirtual(ctop):
        cname = ctop.name
        while cname:
            c = cdict[cname.replace("::","")]
            if c.vtable == 1:
                return True
            if c.vtable == "0":
                return False
            cname = c.parent
        return False
    
    for c in classes:
        assert c.reflected
        if c.abstract:
            ret.append("HK_ABSTRACT_CLASS(%s)" % c.name)
        elif hasVirtual(c):
            ret.append("HK_CLASS(%s)" % c.name)
        else:
            ret.append("HK_STRUCT(%s)" % c.name)
    ret.sort()
    return ret

def getIncludeList(classes, classcppFromClassName):
    ret = {}
    for c in classes:
        ret[ classcppFromClassName[c.name] ] = 1
    ret = ret.keys()
    ret.sort()
    return ["#include <%s>"% r for r in ret]
 
def writeIfDifferent(text, fname):
    option_quiet = False
    option_verbose = True
    try:
        if open(fname).read() == text:
            if option_verbose:
                print " same ", fname
            return
    except IOError:
        if not option_quiet:
            print "CREATE", fname
    else:
        if not option_quiet:
            print "UPDATE", fname
    open(fname,"w").write(text)

    
def main():
    import glob
    import sys
    try: top = sys.argv[1]
    except IndexError: top = "."
    projects = [ d for d in glob.glob("%s/Source/*/*"%top) if (os.path.isfile(os.path.join(d,'settings.build'))) ]
    if len(projects) == 0:
        print "USAGE: %s <path to top directory>" % os.path.basename(__file__)

    classListFromProject = {} # project -> [class]
    classFromClassName = {} # global
    classcppFromClassName = {} # global
    
    # scan for all reflected classes first, remembering where they came from
    for where in projects:
        classes, locations = findClassesAndLocations(where)
        for c in classes:
            classFromClassName[c.name] = c
        classcppFromClassName.update( locations )
        classListFromProject[where] = [ c for c in classes if c.reflected ]

    # go through again    
    for project, classes in classListFromProject.items():
        project = project.replace("\\","/")
        clist = getClassList( classes, classFromClassName )
        if len(clist):
            # Get tkbms and module name.
            settingsFile = os.path.join(project, 'settings.build')
            product = 'ALL'
            if os.path.isfile(settingsFile):
                try:
                    settingsText = open( settingsFile ).read()
                    product = re_PRODUCT.search(settingsText).group('product')
                except:
                    product = 'ALL'
            headerText = "%s%s\n" % (TKBMS % product, "\n".join(clist))
            headerName = "%sClasses.h" % ( getModuleName(project) )
            cxxName = "%sClasses.cxx" % ( getModuleName(project) )
            cxxText = "%s%s\n" % (TKBMS % product, "\n".join(getIncludeList(classes, classcppFromClassName)) )
            #write header and cxx
            folder = os.path.join( project, "Classes" )
            try: os.mkdir(folder)
            except OSError: pass
            writeIfDifferent( headerText, os.path.join(folder, headerName) )
            writeIfDifferent( cxxText, os.path.join(folder, cxxName) )

if __name__=="__main__":
    main()


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
