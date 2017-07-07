#ifndef PARSESESSION_H
#define PARSESESSION_H

#include <serialization/indexedstring.h>
#include <language/interfaces/iastcontainer.h>

#include "astredux.h"

namespace Rust
{

class ParseSessionData : public KDevelop::IAstContainer
{
public:
    typedef QExplicitlySharedDataPointer<ParseSessionData> Ptr;

    ParseSessionData(const KDevelop::IndexedString &document,
                     const QByteArray &contents);

    virtual ~ParseSessionData();

private:
    friend class ParseSession;

    void parse();

    KDevelop::IndexedString m_document;
    QByteArray m_contents;
    RSCrate *m_crate;
};

class ParseSession
{
public:
    explicit ParseSession(const ParseSessionData::Ptr &data);
    ~ParseSession();

    void parse();

    ParseSessionData::Ptr data() const;
    void setData(const ParseSessionData::Ptr data);

    KDevelop::IndexedString document() const;
    RSCrate *crate() const;

private:
    Q_DISABLE_COPY(ParseSession)

    ParseSessionData::Ptr d;
};

}

#endif // PARSESESSION_H
