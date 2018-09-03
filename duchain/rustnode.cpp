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

#include "rustnode.h"

namespace Rust
{

RustPath::RustPath(RustNode *node)
{
    RustString name = RustString(node_get_spelling_name(node->data()));

    if (*name == nullptr) {
        value = QString();
    } else {
        value = QString::fromUtf8(*name);
    }
}

RustNode::RustNode(RSNode *node)
    : RustAllocatedObject(node)
{
}

RustNode::RustNode(RustOwnedNode &node)
    : RustAllocatedObject(node.data())
{
}

RustNode::~RustNode()
{
}

template<typename RustObjectType, void (*RustDestructor)(RustObjectType *)>
RustAllocatedObject<RustObjectType, RustDestructor>::RustAllocatedObject(RustObjectType *object)
    : object(object)
{
}

template<typename RustObjectType, void (*RustDestructor)(RustObjectType *)>
RustAllocatedObject<RustObjectType, RustDestructor>::~RustAllocatedObject()
{
    RustDestructor(object);
    object = nullptr;
}

template<typename RustObjectType, void (*RustDestructor)(RustObjectType *)>
RustObjectType *RustAllocatedObject<RustObjectType, RustDestructor>::data()
{
    return object;
}

template<typename RustObjectType, void (*RustDestructor)(RustObjectType *)>
RustObjectType *RustAllocatedObject<RustObjectType, RustDestructor>::operator *()
{
    return object;
}

}
