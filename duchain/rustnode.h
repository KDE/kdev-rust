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

#ifndef RUSTNODE_H
#define RUSTNODE_H

#include <language/duchain/ducontext.h>

#include "astredux.h"

#include "kdevrustduchain_export.h"

namespace Rust
{

template <typename RustObjectType, void (*RustDestructor)(RustObjectType *)>
class KDEVRUSTDUCHAIN_EXPORT RustAllocatedObject
{
public:
    RustAllocatedObject(RustObjectType *object);
    ~RustAllocatedObject();

    RustObjectType *data();
    RustObjectType *operator *();

private:
    RustObjectType *object;
};

template <typename T> void noop_destructor(T *) {}

using RustCrate = RustAllocatedObject<RSCrate, destroy_crate>;
using RustOwnedNode = RustAllocatedObject<RSNode, destroy_node>;
using RustString = RustAllocatedObject<const char, destroy_string>;
using RustDiagnostic = RustAllocatedObject<RSDiagnostic, destroy_diagnostic>;
using RustDiagnosticIterator = RustAllocatedObject<RSDiagnosticIterator, destroy_diagnostic_iterator>;

class KDEVRUSTDUCHAIN_EXPORT RustNode : public RustAllocatedObject<RSNode, noop_destructor>
{
public:
    RustNode(RSNode *node);
    RustNode(RustOwnedNode &node);
    ~RustNode();
};

class KDEVRUSTDUCHAIN_EXPORT RustPath
{
public:
    explicit RustPath(RustNode *node);

    QString value;
};

template class KDEVRUSTDUCHAIN_EXPORT RustAllocatedObject<const char, destroy_string>;
template class KDEVRUSTDUCHAIN_EXPORT RustAllocatedObject<RSCrate, destroy_crate>;
template class KDEVRUSTDUCHAIN_EXPORT RustAllocatedObject<RSNode, destroy_node>;
template class KDEVRUSTDUCHAIN_EXPORT RustAllocatedObject<RSDiagnostic, destroy_diagnostic>;
template class KDEVRUSTDUCHAIN_EXPORT RustAllocatedObject<RSDiagnosticIterator, destroy_diagnostic_iterator>;
template class KDEVRUSTDUCHAIN_EXPORT RustAllocatedObject<RSNode, noop_destructor>;

}

#endif // RUSTNODE_H
