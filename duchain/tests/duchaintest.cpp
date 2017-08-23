#include "duchaintest.h"

#include <QtTest/QtTest>

#include <language/duchain/ducontext.h>
#include <language/duchain/topducontext.h>
#include <tests/testcore.h>
#include <tests/autotestshell.h>
#include <tests/testhelpers.h>

#include "parsesession.h"
#include "declarationbuilder.h"
#include "usebuilder.h"

using namespace KDevelop;

QTEST_MAIN(DUChainTest)

ReferencedTopDUContext parseCode(QString code)
{
    using namespace Rust;

    IndexedString document("temp");
    ParseSessionData *sessionData = new ParseSessionData(document, code.toUtf8());
    ParseSession session(ParseSessionData::Ptr(nullptr));
    session.setData(ParseSessionData::Ptr(sessionData));
    session.parse();

    RustOwnedNode crateNode = RustOwnedNode(node_from_crate(session.crate()));
    RustNode node = RustNode(crateNode);
    DeclarationBuilder declarationBuilder;
    UseBuilder useBuilder(document);

    declarationBuilder.setParseSession(&session);
    useBuilder.setParseSession(&session);

    ReferencedTopDUContext context = declarationBuilder.build(document, &node);
    useBuilder.buildUses(&node);

    return context;
}

QString inMain(QString code)
{
    return QString("fn main() { %1 }").arg(code);
}

DUContext *getMainContext(ReferencedTopDUContext topContext)
{
    if (!topContext) {
        return nullptr;
    }

    DUChainReadLocker lock;
    QList<Declaration *> mainDeclaration = topContext->findDeclarations(QualifiedIdentifier("main"));

    if (mainDeclaration.size() != 1 || !mainDeclaration.first()) {
        return nullptr;
    }

    DUContext *mainContext = mainDeclaration.first()->internalContext();

    if (mainContext->childContexts().size() != 1 || !mainContext->childContexts().first()) {
        return nullptr;
    }

    return mainContext->childContexts().first();
}

void DUChainTest::initTestCase()
{
    AutoTestShell::init();
    TestCore::initialize(Core::NoUi);
}

void DUChainTest::sanityCheck()
{
    QString code("fn main() {}");
    ReferencedTopDUContext context = parseCode(code);
    QVERIFY(context.data());

    DUChainReadLocker lock;
    auto decls = context->localDeclarations();
    QCOMPARE(decls.size(), 1);
    Declaration *funcDeclaration = decls.first();
    QVERIFY(funcDeclaration);
    QCOMPARE(funcDeclaration->identifier().toString(), QString("main"));
}

void DUChainTest::cleanupTestCase()
{
    TestCore::shutdown();
}

void DUChainTest::testPatternBindings()
{
    QFETCH(QString, pattern);
    QFETCH(QStringList, bindings);

    QString code = inMain(QString("let %1 = unreachable!();").arg(pattern));
    ReferencedTopDUContext context = parseCode(code);
    DUContext *mainContext = getMainContext(context);

    QCOMPARE(mainContext->localDeclarations().size(), bindings.size());

    for (const QString &binding : bindings) {
        QCOMPARE(mainContext->findLocalDeclarations(Identifier(binding)).size(),  1);
    }
}

void DUChainTest::testPatternBindings_data()
{
    QTest::addColumn<QString>("pattern");
    QTest::addColumn<QStringList>("bindings");

    QTest::newRow("unit") << "()" << QStringList();
    QTest::newRow("wildcard") << "_" << QStringList();
    QTest::newRow("simple ident") << "x" << QStringList { "x" };
    QTest::newRow("simple ident underscore") << "_x" << QStringList { "_x" };
    QTest::newRow("mut ident") << "mut x" << QStringList { "x" };
    QTest::newRow("ref ident") << "ref x" << QStringList { "x" };
    QTest::newRow("ref mut ident") << "ref mut x" << QStringList { "x" };
    QTest::newRow("struct single field") << "Foo { x }" << QStringList { "x" };
    QTest::newRow("struct single field renamed") << "Foo { x: a }" << QStringList { "a" };
    QTest::newRow("struct multiple fields") << "Foo { x, y }" << QStringList { "x", "y" };
    QTest::newRow("struct multiple fields renamed") << "Foo { x: a, y: b }" << QStringList { "a", "b" };
    QTest::newRow("struct multiple fields single renamed") << "Foo { x: a, y }" << QStringList { "a", "y" };
    QTest::newRow("struct multiple fields with remaining") << "Foo { x, y, .. }" << QStringList { "x", "y" };
    QTest::newRow("tuple") << "(x, y)" << QStringList { "x", "y" };
    QTest::newRow("tuple with remaining") << "(x, y, .., z)" << QStringList { "x", "y", "z" };
    QTest::newRow("full with destructure tuple") << "t@(x, y)" << QStringList { "t", "x", "y" };
    QTest::newRow("full with destructure struct") << "t@Foo { x, y }" << QStringList { "t", "x", "y" };
}
