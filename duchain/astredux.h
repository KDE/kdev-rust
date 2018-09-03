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

#ifndef ASTREDUX_H
#define ASTREDUX_H

extern "C" {
enum RSNodeKind
{
    Crate,
    Module,
    StructDecl,
    EnumDecl,
    TraitDecl,
    ImplDecl,
    TypeAliasDecl,
    FieldDecl,
    EnumVariantDecl,
    FunctionDecl,
    ParmDecl,
    VarDecl,
    Path,
    PathSegment,
    Block,
    Arm,
    Unexposed
};

enum RSDiagnosticLevel
{
    Info,
    Note,
    Warning,
    Error,
    Fatal
};

enum RSVisitResult
{
    Break,
    Continue,
    Recurse
};

struct RSLocation
{
    int line;
    int column;
};

struct RSRange
{
    RSLocation start;
    RSLocation end;
};

struct RSCrate;
struct RSNode;
struct RSDiagnostic;
struct RSDiagnosticIterator;

typedef RSVisitResult (*CallbackFn)(RSNode *node, RSNode *parent, void *data);


RSCrate *parse_crate(const char *name, const char *source);
void destroy_crate(RSCrate *crate);

RSNode *node_from_crate(RSCrate *crate);
void destroy_node(RSNode *node);

unsigned int node_get_id(RSNode *node);
RSCrate *node_get_crate(RSNode *node);
RSNodeKind node_get_kind(RSNode *node);

const char *node_get_spelling_name(RSNode *node);
RSRange node_get_spelling_range(RSNode *node);
RSRange node_get_extent(RSNode *node);

void destroy_string(const char *str);

RSDiagnosticIterator *crate_get_diagnostics(RSCrate *crate);
void destroy_diagnostic_iterator(RSDiagnosticIterator *iterator);

RSDiagnostic *diagnostics_next(RSDiagnosticIterator *iterator);
void destroy_diagnostic(RSDiagnostic *diagnostic);

RSDiagnosticLevel diagnostic_get_level(RSDiagnostic *diagnostic);
const char *diagnostic_get_message(RSDiagnostic *diagnostic);
RSRange diagnostic_get_primary_range(RSDiagnostic *diagnostic);

void visit_children(RSNode *node, CallbackFn callback, void *data);

}

#endif // ASTREDUX_H
