#include "parsesession.h"

namespace Rust
{

ParseSessionData::ParseSessionData(const KDevelop::IndexedString &document,
                                   const QByteArray &contents)
    : m_document(document),
      m_contents(contents),
      m_crate(nullptr)
{
}

ParseSessionData::~ParseSessionData()
{
    if (m_crate != nullptr) {
        destroy_crate(m_crate);
    }
}

void ParseSessionData::parse()
{
    m_crate = parse_crate(m_document.c_str(), m_contents);
}

ParseSession::ParseSession(const ParseSessionData::Ptr &data)
    : d(data)
{
}

ParseSession::~ParseSession()
{
}

void ParseSession::parse()
{
    d->parse();
}

ParseSessionData::Ptr ParseSession::data() const
{
    return d;
}

void ParseSession::setData(const ParseSessionData::Ptr data)
{
    this->d = data;
}

KDevelop::IndexedString ParseSession::document() const
{
    return d->m_document;
}

RSCrate *ParseSession::crate() const
{
    return d->m_crate;
}

void ParseSession::setContextOnNode(RustNode *node, KDevelop::DUContext *context)
{
    node->setContext(context);
}

KDevelop::DUContext *ParseSession::contextFromNode(RustNode *node)
{
    return node->getContext();
}

}
