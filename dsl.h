#ifndef DSL_H
#define DSL_H

#include <stdlib.h>

#ifndef DSL_DATATYPE
typedef void *DSL_DATATYPE;
#endif

#ifndef DSL_NULL
#define DSL_NULL 0
#endif

typedef struct {
	size_t con;
	double prop;
} dsl_overallocation;

dsl_overallocation *dsl_new_overalloc(void);
void dsl_free_overalloc(dsl_overallocation *);
void dsl_init_overalloc(dsl_overallocation *);
void dsl_overalloc_set_const(dsl_overallocation *, size_t);
void dsl_overalloc_set_prop(dsl_overallocation *, double);

extern dsl_overallocation DSL_DEFAULT_OA;

typedef void (*dsl_destructor)(DSL_DATATYPE);
typedef DSL_DATATYPE (*dsl_copier)(DSL_DATATYPE);

typedef struct {
	dsl_destructor destr;
	dsl_copier copy;
} dsl_object_funcs;

void dsl_null_destruct(DSL_DATATYPE);
DSL_DATATYPE dsl_null_copy(DSL_DATATYPE);

extern dsl_object_funcs DSL_DEFAULT_OF;

typedef struct {
	DSL_DATATYPE *data;
	size_t len;
	size_t cap;
	dsl_overallocation oa;
	dsl_object_funcs of;
} dsl_array;

dsl_array *dsl_new_array(dsl_overallocation *, dsl_object_funcs *);
void dsl_free_array(dsl_array *);
void dsl_array_alloc(dsl_array *, size_t);
void dsl_array_fill(dsl_array *, size_t, DSL_DATATYPE);
size_t dsl_array_len(dsl_array *);
DSL_DATATYPE dsl_array_get(dsl_array *, size_t);
void dsl_array_set(dsl_array *, size_t, DSL_DATATYPE);
void dsl_array_insert(dsl_array *, size_t, DSL_DATATYPE);
DSL_DATATYPE dsl_array_remove(dsl_array *, size_t);
void dsl_array_delete(dsl_array *, size_t);
dsl_array *dsl_array_copy(dsl_array *);
dsl_array *dsl_array_append(dsl_array *, dsl_array *);

typedef struct {
	dsl_array *array;
	ssize_t idx;
} dsl_array_iter;

dsl_array_iter *dsl_new_array_iter(dsl_array *);
dsl_array *dsl_array_iter_array(dsl_array_iter *);
void dsl_free_array_iter(dsl_array_iter *);
ssize_t dsl_array_iter_seek(dsl_array_iter *, size_t);
ssize_t dsl_array_iter_index(dsl_array_iter *);
DSL_DATATYPE dsl_array_iter_at(dsl_array_iter *);
void dsl_array_iter_set_at(dsl_array_iter *, DSL_DATATYPE);
void dsl_array_iter_insert_at(dsl_array_iter *, DSL_DATATYPE);
void dsl_array_iter_insert_after(dsl_array_iter *, DSL_DATATYPE);
DSL_DATATYPE dsl_array_iter_remove_at(dsl_array_iter *);
void dsl_array_iter_delete_at(dsl_array_iter *);
int dsl_array_iter_next(dsl_array_iter *);
int dsl_array_iter_prev(dsl_array_iter *);
int dsl_array_iter_at_end(dsl_array_iter *);
int dsl_array_iter_at_begin(dsl_array_iter *);
int dsl_array_iter_is_invalid(dsl_array_iter *);

typedef struct tag_dsl_listnode {
	DSL_DATATYPE data;
	struct tag_dsl_listnode *prev;
	struct tag_dsl_listnode *next;
} dsl_listnode;

typedef struct {
	dsl_listnode *first;
	dsl_object_funcs of;
} dsl_list;

dsl_list *dsl_new_list(dsl_object_funcs *);
void dsl_free_list(dsl_list *);
dsl_listnode *dsl_list_node(dsl_list *, size_t);
size_t dsl_list_len(dsl_list *);
DSL_DATATYPE dsl_list_get(dsl_list *, size_t);
void dsl_list_set(dsl_list *, size_t, DSL_DATATYPE);
void dsl_list_insert(dsl_list *, size_t, DSL_DATATYPE);
DSL_DATATYPE dsl_list_remove(dsl_list *, size_t);
void dsl_list_delete(dsl_list *, size_t);
dsl_list *dsl_list_copy(dsl_list *);
dsl_list *dsl_list_append(dsl_list *, dsl_list *);

typedef struct {
	dsl_list *list;
	dsl_listnode *cur;
} dsl_list_iter;

dsl_list_iter *dsl_new_list_iter(dsl_list *);
dsl_list *dsl_list_iter_list(dsl_list_iter *);
void dsl_free_list_iter(dsl_list_iter *);
ssize_t dsl_list_iter_seek(dsl_list_iter *, size_t);
ssize_t dsl_list_iter_index(dsl_list_iter *);
DSL_DATATYPE dsl_list_iter_at(dsl_list_iter *);
void dsl_list_iter_set_at(dsl_list_iter *, DSL_DATATYPE);
void dsl_list_iter_insert_at(dsl_list_iter *, DSL_DATATYPE);
void dsl_list_iter_insert_after(dsl_list_iter *, DSL_DATATYPE);
DSL_DATATYPE dsl_list_iter_remove_at(dsl_list_iter *);
void dsl_list_iter_delete_at(dsl_list_iter *);
int dsl_list_iter_next(dsl_list_iter *);
int dsl_list_iter_prev(dsl_list_iter *);
int dsl_list_iter_at_end(dsl_list_iter *);
int dsl_list_iter_at_begin(dsl_list_iter *);
int dsl_list_iter_is_invalid(dsl_list_iter *);

typedef enum {DSL_SEQ_ARRAY, DSL_SEQ_LIST} dsl_seqtype;

typedef struct {
	dsl_seqtype type;
	union {
		dsl_array *array;
		dsl_list *list;
	};
} dsl_seq;

dsl_seq *dsl_seq_new_array(dsl_overallocation *, dsl_object_funcs *);
dsl_seq *dsl_seq_new_list(dsl_object_funcs *);
dsl_seq *dsl_seq_from_array(dsl_array *);
dsl_seq *dsl_seq_from_list(dsl_list *);
dsl_array *dsl_seq_to_array(dsl_seq *);
dsl_list *dsl_seq_to_list(dsl_seq *);
void dsl_free_seq(dsl_seq *);
size_t dsl_seq_len(dsl_seq *);
DSL_DATATYPE dsl_seq_get(dsl_seq *, size_t);
void dsl_seq_set(dsl_seq *, size_t, DSL_DATATYPE);
void dsl_seq_insert(dsl_seq *, size_t, DSL_DATATYPE);
DSL_DATATYPE dsl_seq_remove(dsl_seq *, size_t);
void dsl_seq_delete(dsl_seq *, size_t);
dsl_seq *dsl_seq_copy(dsl_seq *);
dsl_seq *dsl_seq_append(dsl_seq *, dsl_seq *);

typedef struct {
	dsl_seqtype type;
	dsl_seq *seq;
	union {
		dsl_array_iter *array;
		dsl_list_iter *list;
	};
} dsl_seq_iter;

dsl_seq_iter *dsl_new_seq_iter(dsl_seq *);
dsl_seq *dsl_seq_iter_seq(dsl_seq_iter *);
void dsl_free_seq_iter(dsl_seq_iter *);
ssize_t dsl_seq_iter_seek(dsl_seq_iter *, size_t);
ssize_t dsl_seq_iter_index(dsl_seq_iter *);
DSL_DATATYPE dsl_seq_iter_at(dsl_seq_iter *);
void dsl_seq_iter_set_at(dsl_seq_iter *, DSL_DATATYPE);
void dsl_seq_iter_insert_at(dsl_seq_iter *, DSL_DATATYPE);
void dsl_seq_iter_insert_after(dsl_seq_iter *, DSL_DATATYPE);
DSL_DATATYPE dsl_seq_iter_remove_at(dsl_seq_iter *);
void dsl_seq_iter_delete_at(dsl_seq_iter *);
int dsl_seq_iter_next(dsl_seq_iter *);
int dsl_seq_iter_prev(dsl_seq_iter *);
int dsl_seq_iter_at_end(dsl_seq_iter *);
int dsl_seq_iter_at_begin(dsl_seq_iter *);
int dsl_seq_iter_is_invalid(dsl_seq_iter *);

typedef DSL_DATATYPE (*dsl_map_func)(DSL_DATATYPE);
typedef int (*dsl_filter_func)(DSL_DATATYPE);
typedef int (*dsl_cmp_func)(DSL_DATATYPE, DSL_DATATYPE);

void dsl_seq_map(dsl_seq *, dsl_map_func);
void dsl_seq_filter(dsl_seq *, dsl_filter_func);
void dsl_seq_sort(dsl_seq *, dsl_cmp_func);

#endif
