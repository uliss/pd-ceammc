/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#ifndef MLIST_PARSER_IMPL_H
#define MLIST_PARSER_IMPL_H

#include "stddef.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct mlist_node mlist_node;
typedef struct mlist_ast mlist_ast;

typedef enum traverse_action {
    TRAVERSE_PUSH = 0,
    TRAVERSE_POP = 1,
    TRAVERSE_ADD = 2
} traverse_action;

typedef void (*traverse_fn)(mlist_node*, void*, void*, traverse_action, const char* txt);

mlist_ast* mlist_ast_create();
void mlist_ast_free(mlist_ast* ast);
void mlist_ast_clear(mlist_ast* ast);
void mlist_ast_dump(mlist_ast* ast);
void mlist_ast_traverse(mlist_ast* ast, void* data, void* stack, traverse_fn cb);

/**
 * @return to pointer to allocated value
 * @note caller should free result with free()
 */
char* mlist_ast_to_string(mlist_ast* ast);

void mlist_ast_set_root(mlist_ast* ast, mlist_node* n);
void mlist_ast_push(mlist_ast* ast, mlist_node* n);
mlist_node* mlist_ast_pop(mlist_ast* ast);
mlist_node* mlist_ast_top(mlist_ast* ast);

void mlist_node_append(mlist_node* dest, mlist_node* src);
mlist_node* mlist_node_text_create(const char* txt, int quoted);
mlist_node* mlist_node_create();
int mlist_node_is_quoted(mlist_node* n);

int mlist_parse_string(mlist_ast* tree, const char* s);

#ifdef __cplusplus
}
#endif

#endif // MLIST_PARSER_IMPL_H
