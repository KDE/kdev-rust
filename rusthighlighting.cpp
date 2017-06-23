#include "rusthighlighting.h"

using namespace KDevelop;

namespace Rust
{

class HighlightingInstance : public CodeHighlightingInstance
{
public:
    HighlightingInstance(const Highlighting *highlighting);
};

HighlightingInstance::HighlightingInstance(const Highlighting *highlighting)
    : CodeHighlightingInstance(highlighting)
{
}

Highlighting::Highlighting(QObject* parent)
    : CodeHighlighting(parent)
{
}

CodeHighlightingInstance* Highlighting::createInstance() const
{
    return new HighlightingInstance(this);
}

}
