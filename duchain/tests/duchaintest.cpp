#include "duchaintest.h"

#include <QtTest/QtTest>

#include <tests/testcore.h>
#include <tests/autotestshell.h>
#include <tests/testhelpers.h>

using namespace KDevelop;

QTEST_MAIN(DUChainTest)

void DUChainTest::initTestCase()
{
    AutoTestShell::init();
    TestCore::initialize(Core::NoUi);
}

void DUChainTest::sanityCheck()
{

}

void DUChainTest::cleanupTestCase()
{
    TestCore::shutdown();
}
