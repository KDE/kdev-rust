#include "rusthighlighting.h"

using namespace KDevelop;

namespace Rust
{

class HighlightingInstance : public CodeHighlightingInstance
{
public:
    HighlightingInstance(const Highlighting *highlighting);
    ~HighlightingInstance() override;
};

HighlightingInstance::HighlightingInstance(const Highlighting *highlighting)
    : CodeHighlightingInstance(highlighting)
{
}

HighlightingInstance::~HighlightingInstance()
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
