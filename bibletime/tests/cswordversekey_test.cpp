#include <cppunit/extensions/HelperMacros.h>

#include <iostream>

#include "backend/cswordversekey.h"

class CSwordVerseKeyTest : public CppUnit::TestFixture  {
CPPUNIT_TEST_SUITE( CSwordVerseKeyTest );
//CPPUNIT_TEST( testParameterValue );
CPPUNIT_TEST_SUITE_END();

public:
	void setUp() {
	}	
	void tearDown()  {
	}
	
/*	void testProtocol()
	{
		CPPUNIT_ASSERT( m_url3->getProtocol() && strlen( m_url3->getProtocol() ) == 0 );
	}*/
};

CPPUNIT_TEST_SUITE_REGISTRATION(CSwordVerseKeyTest);
