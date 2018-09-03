/*
 * Copyright 2017  Emma Gospodinova <emma.gospodinova@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

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
    d->m_nodeContextMap.insert(node_get_id(node->data()), context);
}

KDevelop::DUContext *ParseSession::contextFromNode(RustNode *node)
{
    return d->m_nodeContextMap.value(node_get_id(node->data()));
}

}
