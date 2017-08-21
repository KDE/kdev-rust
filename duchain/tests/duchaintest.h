#ifndef DUCHAINTEST_H
#define DUCHAINTEST_H

#include <QObject>

class DUChainTest : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void sanityCheck();
    void cleanupTestCase();
};

#endif // DUCHAINTEST_H
