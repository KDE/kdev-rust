#ifndef RUSTHIGHLIGHTING_H
#define RUSTHIGHLIGHTING_H

#include <language/highlighting/codehighlighting.h>

namespace Rust
{

class Highlighting : public KDevelop::CodeHighlighting
{
    Q_OBJECT
public:
    Highlighting(QObject *parent);
    KDevelop::CodeHighlightingInstance *createInstance() const override;
};

}

#endif // RUSTHIGHLIGHTING_H
