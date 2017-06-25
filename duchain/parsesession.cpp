#include "parsesession.h"

namespace Rust
{

ParseSessionData::ParseSessionData(const KDevelop::IndexedString &document,
                                   const QByteArray &contents,
                                   RSIndex *index)
    : m_document(document),
      m_contents(contents),
      m_crate(nullptr)
{
    m_crate = parse_crate(m_document.c_str(), m_contents, index);
}

ParseSessionData::~ParseSessionData()
{
    if (m_crate != nullptr) {
        destroy_crate(m_crate);
    }
}

ParseSession::ParseSession(const ParseSessionData::Ptr &data)
    : d(data)
{
}

ParseSession::~ParseSession()
{
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

}
