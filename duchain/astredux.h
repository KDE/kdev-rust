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

void visit_children(RSNode *node, CallbackFn callback, void *data);

}

#endif // ASTREDUX_H
