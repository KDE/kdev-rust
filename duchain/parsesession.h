#ifndef PARSESESSION_H
#define PARSESESSION_H

#include <QMap>

#include <serialization/indexedstring.h>
#include <language/duchain/ducontext.h>
#include <language/interfaces/iastcontainer.h>

#include "astredux.h"
#include "rustnode.h"

#include "kdevrustduchain_export.h"

namespace Rust
{

class KDEVRUSTDUCHAIN_EXPORT ParseSessionData : public KDevelop::IAstContainer
{
public:
    typedef QExplicitlySharedDataPointer<ParseSessionData> Ptr;

    ParseSessionData(const KDevelop::IndexedString &document,
                     const QByteArray &contents);

    ~ParseSessionData() override;

private:
    friend class ParseSession;

    void parse();

    KDevelop::IndexedString m_document;
    QByteArray m_contents;
    RSCrate *m_crate;
    QMap<unsigned int, KDevelop::DUContext *> m_nodeContextMap;
};

class KDEVRUSTDUCHAIN_EXPORT ParseSession
{
public:
    explicit ParseSession(const ParseSessionData::Ptr &data);
    ~ParseSession();

    void parse();

    ParseSessionData::Ptr data() const;
    void setData(const ParseSessionData::Ptr data);

    KDevelop::IndexedString document() const;
    RSCrate *crate() const;

    void setContextOnNode(RustNode *node, KDevelop::DUContext *context);
    KDevelop::DUContext *contextFromNode(RustNode *node);

private:
    Q_DISABLE_COPY(ParseSession)

    ParseSessionData::Ptr d;
};

}

#endif // PARSESESSION_H
