/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2009 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_BASE_UNITTEST_H
#define HK_BASE_UNITTEST_H

#include <Common/Base/Config/hkConfigVersion.h>
#include <Common/Base/System/Error/hkDefaultError.h>

#if defined( HK_COMPILER_MWERKS ) && (HAVOK_BUILD_NUMBER != 0)

// Disable on CodeWarrior platforms
#define HK_TEST(CONDITION) /* nothing */
#define HK_TEST2(CONDITION,DESCRIPTION) /* nothing */
#define HK_TEST_REGISTER(func, category, menu, path) /* nothing */

#elif defined(HK_PLATFORM_SPU)
extern hkBool HK_CALL hkTestReport(hkBool32 cond, int line);
#	define HK_TEST(CONDITION)  hkTestReport( (CONDITION), __LINE__)
#	define HK_TEST2(CONDITION,DESCRIPTION) HK_TEST(CONDITION)
#	define HK_TEST_REGISTER(func, category, menu, path) 

#else

extern hkBool HK_CALL hkTestReport(hkBool32 cond, const char* desc, const char* file, int line);

/************* PUBLIC *******************/
#define HK_TEST(CONDITION)  hkTestReport( (CONDITION), #CONDITION, __FILE__, __LINE__)

#define HK_TEST2(CONDITION,DESCRIPTION)  do {	\
		char msgBuf[512];								\
		hkOstream msg(msgBuf, sizeof(msgBuf), true);	\
		msg << #CONDITION  << " (" << DESCRIPTION << ')'; \
		hkTestReport( (CONDITION), msgBuf, __FILE__, __LINE__); \
	} while(0)

#define HK_TEST_REGISTER(func, category, menu, path) hkTestEntry register ## func (func, #func, category, menu path)

#endif //Codewarrior

/************* INTERNAL USE ONLY *******************/


extern struct hkTestEntry* hkUnitTestDatabase;
struct hkTestEntry
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE_CLASS, hkTestEntry );
	typedef int (*hkTestFunction)();

	inline hkTestEntry(hkTestFunction func, const char* name, const char* cat, const char* path)
		: m_func(func), m_name(name), m_category(cat), m_path(path)
	{
		m_next = hkUnitTestDatabase;
		hkUnitTestDatabase = this;
	}


	~hkTestEntry() { }

	hkTestFunction m_func;
	const char* m_name;
	const char* m_category;
	const char* m_path;
	hkTestEntry* m_next;
};

class TestDemo;

#define HK_REGRESSION_REPORT(NAME, TYPE, VALUE)	HK_MULTILINE_MACRO_BEGIN															\
												char reportBuf[512];																\
												hkErrStream ostr(reportBuf,sizeof(reportBuf));										\
												ostr << "[REGRESSION:" << NAME << ":" << TYPE << ":" << VALUE << "]";			\
												hkError::getInstance().message(hkError::MESSAGE_REPORT, 0, reportBuf, HK_NULL, 0);	\
												HK_MULTILINE_MACRO_END

#define HK_REGRESSION_CHECK_RANGE(NAME, TYPE, VALUE, MINVALUE, MAXVALUE)	HK_MULTILINE_MACRO_BEGIN																			\
																			char reportBuf[512];																				\
																			hkErrStream ostr(reportBuf,sizeof(reportBuf));														\
																			if ((VALUE >= MINVALUE) && ( VALUE <= MAXVALUE ))													\
																			{																									\
																				ostr << "[REGRESSION:" << NAME << ":" << TYPE << ":" << VALUE << "]";						\
																				hkError::getInstance().message(hkError::MESSAGE_REPORT, 0, reportBuf, HK_NULL, 0);				\
																			}																									\
																			else																								\
																			{																									\
																				ostr << "[REGRESSION FAILED:" << NAME << ":" << TYPE << ":" << VALUE << ": Range(" << MINVALUE << "," << MAXVALUE << ") ]"; \
																				hkError::getInstance().message(hkError::MESSAGE_ERROR, -1, reportBuf, __FILE__, __LINE__); 		\
																			}																									\
																			HK_MULTILINE_MACRO_END


namespace hkMath
{
	hkReal HK_CALL rand01();
	inline hkReal HK_CALL randRange(hkReal minv, hkReal maxv)
	{
		return minv + rand01() * (maxv-minv);
	}
}


#endif // HK_TEST_UNITTEST_REGISTERTEST_H

/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20091222)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2009
* Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
* Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
* rights, and intellectual property rights in the Havok software remain in
* Havok and/or its suppliers.
* 
* Use of this software for evaluation purposes is subject to and indicates
* acceptance of the End User licence Agreement for this product. A copy of
* the license is included with this software and is also available at www.havok.com/tryhavok.
* 
*/
