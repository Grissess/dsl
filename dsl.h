#ifndef DSL_H
#define DSL_H

// TODO: Make DSL_DATA re-typeable (akin to templating?)
typedef void *DSL_DATA;

#ifndef DSL_NULL
#define DSL_NULL ((void *) 0)
#endif

typedef unsigned long hash_t;

// TODO: Type-check this (statically); maybe some other code analyzer?
#define dsl_retnull(ptr, val) if(!ptr) return val;

#define dsl_as(type, obj) ((type *) (obj))
#define dsl_vt(type, obj, memb) ((type *) (obj)->memb)
#define dsl_dthis(obj, func, ...) ((obj).func(&(obj) , ##__VA_ARGS__))
#define dsl_ithis(obj, func, ...) ((obj)->func(&(obj) , ##__VA_ARGS__))
#define dsl_dast(type, obj, func, ...) dsl_dthis(dsl_as(type, obj), func , ##__VA_ARGS__)
#define dsl_iast(type, obj, func, ...) dsl_ithis(dsl_as(type, obj), func , ##__VA_ARGS__)
#define dsl_dvt(type, obj, memb, func, ...) dsl_dthis(dsl_vt(type, obj, memb), func , ##__VA_ARGS__)
#define dsl_ivt(type, obj, memb, func, ...) dsl_ithis(dsl_vt(type, obj, memb), func , ##__VA_ARGS__)

typedef struct {
    //Pure Virtual
    DSL_DATA (*link)(dsl_incorp *, DSL_DATA);
    void (*unlink)(dsl_incorp *, DSL_DATA);
    hash_t (*hash)(dsl_incorp *, DSL_DATA);
    int (*equal)(dsl_incorp *, DSL_DATA, DSL_DATA);
    int (*less)(dsl_incorp *, DSL_DATA, DSL_DATA);
    //Virtual
    int (*greater)(dsl_incorp *, DSL_DATA, DSL_DATA);
    int (*lesseq)(dsl_incorp *, DSL_DATA, DSL_DATA);
    int (*greatereq)(dsl_incorp *, DSL_DATA, DSL_DATA);
    int (*noteq)(dsl_incorp *, DSL_DATA, DSL_DATA);
} dsl_incorp;

DSL_DATA dsl_incorp_link_void(dsl_incorp *, DSL_DATA);
void dsl_incorp_unlink_void(dsl_incorp *, DSL_DATA);

int dsl_incorp_greater(dsl_incorp *, DSL_DATA, DSL_DATA);
int dsl_incorp_lesseq(dsl_incorp *, DSL_DATA, DSL_DATA);
int dsl_incorp_greatereq(dsl_incorp *, DSL_DATA, DSL_DATA);
int dsl_incorp_noteq(dsl_incorp *, DSL_DATA, DSL_DATA);

void dsl_default_incorp(dsl_incorp *);

typedef struct {
    //Pure Virtual
    void *(*alloc)(size_t);
    void (*free)(void *);
    //Virtual
    void *(*realloc)(void *, size_t);
} dsl_allocator;

void *dsl_allocator_realloc(dsl_allocator *, void *, size_t *);

void dsl_default_allocator(dsl_allocator *);

typedef struct dsl_tag_object dsl_object;

typedef struct {
    dsl_incorp incorp;
    dsl_allocator alloc;
    size_t szobj;
    size_t sztp;
    //Pure Virtual
    void (*construct)(dsl_type *, dsl_object *);
    void (*destruct)(dsl_object *);
    //Virtual
    dsl_object *(*create)(dsl_type *);
    void (*destroy)(dsl_object *);
    void (*copy)(dsl_type *, dsl_type *);
    size_t (*size)(dsl_type *);
} dsl_type;

void dsl_default_type(dsl_type *);

dsl_object *dsl_type_create(dsl_type *);
void dsl_type_destroy(dsl_object *);
void dsl_type_copy(dsl_type *, dsl_type *);
size_t dsl_type_size(dsl_type *);

void dsl_type_init(dsl_type *, size_t, size_t, void (*)(dsl_type *, dsl_object *), void (*)(dsl_object *));

typedef struct dsl_tag_object {
    dsl_type *type;
    dsl_incorp incorp;
    dsl_allocator alloc;
} dsl_object;

void dsl_object_construct(dsl_type *, dsl_object *);
void dsl_object_destruct(dsl_object *);

typedef struct dsl_tag_iter dsl_iter;

typedef struct dsl_tag_iter {
    dsl_object _base;
    //Pure Virtual
    dsl_object *(*owner)(dsl_iter *);
    int (*next)(dsl_iter *);
    int (*prev)(dsl_iter *);
    int (*valid)(dsl_iter *);
} dsl_iter;

void dsl_iter_delete_at(dsl_iter *);

typedef struct dsl_tag_sequence dsl_sequence;

typedef struct dsl_tag_sequence_iter dsl_sequence_iter;

typedef struct dsl_tag_sequence {
    dsl_object _base;
    //Pure virtual
    size_t (*len)(dsl_sequence *);
    DSL_DATA (*get)(dsl_sequence *, size_t);
    void (*set)(dsl_sequence *, size_t, DSL_DATA);
    void (*insert)(dsl_sequence *, size_t, DSL_DATA);
    DSL_DATA (*remove)(dsl_sequence *, size_t);
    void (*iter_start)(dsl_sequence *, dsl_sequence_iter *);
    void (*iter_end)(dsl_sequence *, dsl_sequence_iter *);
    //Virtual
    void (*delete)(dsl_sequence *, size_t);
    void (*fill)(dsl_sequence *, DSL_DATA);
    void (*alloc)(dsl_sequence *, size_t);
    dsl_sequence *(*copy)(dsl_sequence *, dsl_type *);
    void (*append)(dsl_sequence *, dsl_sequence *);
    void (*map)(dsl_sequence *, DSL_DATA (*)(DSL_DATA));
    void (*filter)(dsl_sequence *, int (*)(DSL_DATA));
    void (*sort)(dsl_sequence *, int (*)(DSL_DATA, DSL_DATA));
    ssize_t (*search)(dsl_sequence *, DSL_DATA);
} dsl_sequence;

void dsl_sequence_construct(dsl_type *, dsl_sequence *);
void dsl_sequence_destruct(dsl_sequence *);

void dsl_sequence_delete(dsl_sequence *, size_t);
void dsl_sequence_fill(dsl_sequence *, DSL_DATA);
void dsl_sequence_alloc(dsl_sequence *, size_t);
dsl_sequence *dsl_sequence_copy(dsl_sequence *, dsl_type *);
void dsl_sequence_append(dsl_sequence *, dsl_sequence *);
void dsl_sequence_map(dsl_sequence *, DSL_DATA (*)(DSL_DATA));
void dsl_sequence_filter(dsl_sequence *, int (*)(DSL_DATA));
void dsl_sequence_sort(dsl_sequence *, int (*)(DSL_DATA, DSL_DATA));
ssize_t dsl_sequence_search(dsl_sequence *, DSL_DATA);

typedef struct dsl_tag_sequence_iter {
    dsl_iter _base;
    //Pure Virtual
    size_t (*index)(dsl_sequence_iter *);
    int (*at_end)(dsl_iter *);
    int (*at_begin)(dsl_iter *);
    DSL_DATA (*at)(dsl_iter *);
    void (*set_at)(dsl_iter *, DSL_DATA);
    void (*insert_at)(dsl_iter *, DSL_DATA);
    void (*insert_after)(dsl_iter *, DSL_DATA);
    DSL_DATA (*remove_at)(dsl_iter *);
    //Virtual
    void (*delete_at)(dsl_iter *);
    ssize_t (*set_index)(dsl_sequence_iter *, size_t);
} dsl_sequence_iter;

void dsl_sequence_iter_construct(dsl_type *, dsl_sequence_iter *);
void dsl_sequence_iter_destruct(dsl_sequence_iter *);

ssize_t dsl_sequence_iter_set_index(dsl_iter *, size_t);
void dsl_sequence_iter_delete_at(dsl_iter *);

typedef struct {
    size_t con;
    double prop;
} dsl_overalloc;

void dsl_default_overalloc(dsl_overalloc *);

typedef struct {
    dsl_sequence _base;
    dsl_overalloc oalloc;
    size_t len;
    size_t cap;
    void *data;
} dsl_array;

typedef struct {
    dsl_type _base;
    dsl_overalloc oalloc;
} dsl_array_type;

void dsl_array_construct(dsl_array_type *, dsl_array *);
void dsl_array_destruct(dsl_array *);

void dsl_default_array(dsl_array_type *);

typedef struct dsl_tag_array_iter dsl_array_iter;

size_t dsl_array_len(dsl_array *);
DSL_DATA dsl_array_get(dsl_array *, size_t);
void dsl_array_set(dsl_array *, size_t, DSL_DATA);
void dsl_array_insert(dsl_array *, size_t, DSL_DATA);
DSL_DATA dsl_array_remove(dsl_array *, size_t);
void dsl_array_iter_start(dsl_array *, dsl_array_iter *);
void dsl_array_iter_end(dsl_array *, dsl_array_iter *);
void dsl_array_alloc(dsl_array *, size_t);

typedef struct dsl_tag_array_iter {
    dsl_sequence_iter _base;
    dsl_array *array;
    ssize_t index;
} dsl_array_iter;

void dsl_array_iter_construct(dsl_type *, dsl_array_iter *);
void dsl_array_iter_destruct(dsl_array_iter *);

dsl_array *dsl_array_iter_owner(dsl_array_iter *);
int dsl_array_iter_next(dsl_array_iter *);
int dsl_array_iter_prev(dsl_array_iter *);
int dsl_array_iter_at_end(dsl_array_iter *);
int dsl_array_iter_at_begin(dsl_array_iter *);
int dsl_array_iter_valid(dsl_array_iter *);
size_t dsl_array_iter_index(dsl_array_iter *);
ssize_t dsl_array_iter_set_index(dsl_array_iter *, size_t);
DSL_DATA dsl_array_iter_at(dsl_array_iter *);
void dsl_array_iter_set_at(dsl_array_iter *, DSL_DATA);
void dsl_array_iter_insert_at(dsl_array_iter *, DSL_DATA);
void dsl_array_iter_insert_after(dsl_array_iter *, DSL_DATA);
DSL_DATA dsl_array_iter_remove_at(dsl_array_iter *);

typedef struct dsl_tag_string dsl_string;

typedef struct dsl_tag_string {
    dsl_array _base;
} dsl_string;

typedef struct dsl_tag_string_iter dsl_string_iter;

typedef struct dsl_tag_string_iter {
    dsl_array_iter _base;
} dsl_string_iter;

typedef struct dsl_tag_bytes dsl_bytes;

typedef struct dsl_tag_bytes {
    dsl_string _base;
} dsl_bytes;

char dsl_bytes_get(dsl_bytes *, size_t);
void dsl_bytes_set(dsl_bytes *, size_t, char);
void dsl_bytes_insert(dsl_bytes *, size_t, char);
char dsl_bytes_remove(dsl_bytes *, size_t);

typedef struct dsl_tag_bytes_iter dsl_bytes_iter;

typedef struct dsl_tag_bytes_iter {
    dsl_string_iter _base;
} dsl_bytes_iter;

char dsl_bytes_iter_at(dsl_bytes_iter *);
void dsl_bytes_iter_set_at(dsl_bytes_iter *, char);
void dsl_bytes_iter_insert_at(dsl_bytes_iter *, char);
void dsl_bytes_iter_insert_after(dsl_bytes_iter *, char);
char dsl_bytes_iter_remove_at(dsl_bytes_iter *);

typedef struct dsl_tag_text dsl_text;

typedef struct dsl_tag_text {
    dsl_string _base;
} dsl_text;

long dsl_text_get(dsl_bytes *, size_t);
void dsl_text_set(dsl_bytes *, size_t, long);
void dsl_text_insert(dsl_bytes *, size_t, long);
long dsl_text_remove(dsl_bytes *, size_t);

typedef struct dsl_tag_text_iter dsl_text_iter;

typedef struct dsl_tag_text_iter {
    dsl_string_iter _base;
} dsl_text_iter;

long dsl_text_iter_at(dsl_bytes_iter *);
void dsl_text_iter_set_at(dsl_bytes_iter *, long);
void dsl_text_iter_insert_at(dsl_bytes_iter *, long);
void dsl_text_iter_insert_after(dsl_bytes_iter *, long);
long dsl_text_iter_remove_at(dsl_bytes_iter *);long

typedef struct dsl_tag_list_node dsl_list_node;

typedef struct dsl_tag_list_node {
    DSL_DATA data;
    dsl_list_node *next;
    dsl_list_node *prev;
    int (*is_first)(dsl_list_node *);
    int (*is_last)(dsl_list_node *);
    //Virtual
    void (*insert_at)(dsl_list_node *, DSL_DATA);
    void (*insert_after)(dsl_list_node *, DSL_DATA);
    void (*delete)(dsl_list_node *);
    void (*delete_after)(dsl_list_node *);
} dsl_list_node;

typedef struct dsl_tag_list dsl_list;

typedef struct dsl_tag_list {
    dsl_sequence _base;
    dsl_list_node *first;
} dsl_list;

void dsl_list_construct(dsl_type *, dsl_list *);
void dsl_list_destruct(dsl_list *);

void dsl_default_list(dsl_type *);

typedef struct dsl_tag_list_iter dsl_list_iter;

size_t dsl_list_len(dsl_list *);
DSL_DATA dsl_list_get(dsl_list *, size_t);
void dsl_list_set(dsl_list *, size_t, DSL_DATA);
void dsl_list_insert(dsl_list *, size_t, DSL_DATA);
DSL_DATA dsl_list_remove(dsl_list *, size_t);
void dsl_list_iter_start(dsl_list *, dsl_list_iter *);
void dsl_list_iter_end(dsl_list *, dsl_list_iter *);

typedef struct dsl_tag_list_iter {
    dsl_iter _base;
    dsl_list *list;
    dsl_list_node *cur;
} dsl_list_iter;

dsl_list *dsl_list_iter_sequence(dsl_list_iter *);
int dsl_list_iter_next(dsl_list_iter *);
int dsl_list_iter_prev(dsl_list_iter *);
int dsl_list_iter_at_end(dsl_list_iter *);
int dsl_list_iter_at_begin(dsl_list_iter *);
int dsl_list_iter_valid(dsl_list_iter *);
size_t dsl_list_iter_index(dsl_list_iter *);
DSL_DATA dsl_list_iter_at(dsl_list_iter *);
void dsl_list_iter_set_at(dsl_list_iter *, DSL_DATA);
void dsl_list_iter_insert_at(dsl_list_iter *, DSL_DATA);
void dsl_list_iter_insert_after(dsl_list_iter *, DSL_DATA);
DSL_DATA dsl_list_iter_remove_at(dsl_list_iter *);

typedef struct {
    DSL_DATA key;
    DSL_DATA val;
} dsl_pair;

typedef struct dsl_tag_map dsl_map;

typedef struct dsl_tag_map {
    dsl_object _base;
    //Pure Virtual
    void (*iter)(dsl_map *, dsl_map_iter *);
    void (*iter_at)(dsl_map *, dsl_map_iter *, DSL_DATA);
    void (*set_pair)(dsl_map *, dsl_pair *);
    //Virtual
    void (*get_pair)(dsl_map *, dsl_pair *);
    void (*remove_pair)(dsl_map *, dsl_pair *);
    size_t (*len)(dsl_map *);
    DSL_DATA (*get)(dsl_map *, DSL_DATA);
    void (*set)(dsl_map *, DSL_DATA, DSL_DATA);
    void (*delete)(dsl_map *, DSL_DATA);
    int (*has)(dsl_map *, DSL_DATA);
    dsl_map *(*copy)(dsl_map *, dsl_type *);
} dsl_map;

void dsl_map_get_pair(dsl_map *, dsl_pair *);
void dsl_map_remove_pair(dsl_map *, dsl_pair *);
size_t dsl_map_len(dsl_map *);
DSL_DATA dsl_map_get(dsl_map *, DSL_DATA);
void dsl_map_set(dsl_map *, DSL_DATA, DSL_DATA);
void dsl_map_delete(dsl_map *, DSL_DATA);
int dsl_map_has(dsl_map *, DSL_DATA);
dsl_map *dsl_map_copy(dsl_map *, dsl_type *);

typedef struct dsl_tag_map_iter dsl_map_iter;

typedef struct dsl_tag_map_iter {
    dsl_iter _base;
    //Pure Virtual
    void (*at)(dsl_map_iter *, dsl_pair *);
    void (*set_at)(dsl_map_iter *, dsl_pair *);
    void (*remove_at)(dsl_map_iter *, dsl_pair *);
    //Virtual
    DSL_DATA (*key_at)(dsl_map_iter *);
    DSL_DATA (*val_at)(dsl_map_iter *);
    void (*set_key)(dsl_map_iter *, DSL_DATA);
    void (*set_val)(dsl_map_iter *, DSL_DATA);
} dsl_map_iter;

DSL_DATA dsl_map_iter_key_at(dsl_map_iter *);
DSL_DATA dsl_map_iter_val_at(dsl_map_iter *);
void dsl_map_iter_set_key(dsl_map_iter *, DSL_DATA);
void dsl_map_iter_set_val(dsl_map_iter *, DSL_DATA);

//For convenience--not all maps *don't* support reverse seek.
int dsl_map_iter_prev_invalid(dsl_map_iter *);

typedef struct {
    dsl_map _base;
    dsl_sequence seq;
} dsl_asseq;

typedef struct {
    dsl_type _base;
    dsl_type seqtype;
} dsl_asseq_type;

void dsl_asseq_construct(dsl_asseq_type *, dsl_asseq *);
void dsl_asseq_destruct(dsl_asseq *);

void dsl_default_asseq(dsl_asseq_type *);

typedef struct {
    dsl_map_iter _base;
    dsl_asseq *asseq;
    dsl_sequence_iter seqit;
} dsl_asseq_iter;

dsl_asseq *dsl_asseq_iter_owner(dsl_asseq_iter *);
int dsl_asseq_iter_next(dsl_asseq_iter *);
int dsl_asseq_iter_prev(dsl_asseq_iter *);
int dsl_asseq_valid(dsl_asseq_iter *);
void dsl_asseq_iter_at(dsl_asseq_iter *, dsl_pair *);
void dsl_asseq_iter_set_at(dsl_asseq_iter *, dsl_pair *);
void dsl_asseq_iter_remove_at(dsl_asseq_iter *, dsl_pair *);

size_t dsl_asseq_len(dsl_asseq *);
void dsl_asseq_iter(dsl_asseq *, dsl_asseq_iter *);
void dsl_asseq_iter_at(dsl_asseq *, dsl_asseq_iter *, DSL_DATA);
void dsl_asseq_set_pair(dsl_asseq *, dsl_pair *);

typedef struct {
    dsl_map _base;
    dsl_array buckets;
    //Virtual
    void (*alloc)(dsl_hmap *, size_t);
} dsl_hmap;

typedef struct {
    dsl_type _base;
    dsl_type buckettype;
} dsl_hmap_type;

void dsl_hmap_construct(dsl_hmap_type *, dsl_hmap *);
void dsl_hmap_destruct(dsl_hmap *);

void dsl_default_hmap(dsl_hmap_type *);

typedef struct {
    dsl_map_iter _base;
    dsl_hmap *hmap;
    dsl_sequence *bucket;
} dsl_hmap_iter;

dsl_hmap *dsl_hmap_iter_owner(dsl_hmap_iter *);
int dsl_hmap_iter_next(dsl_hmap_iter *);
int dsl_hmap_iter_prev(dsl_hmap_iter *);
int dsl_hmap_iter_valid(dsl_hmap_iter *);
void dsl_hmap_iter_at(dsl_hmap_iter *, dsl_pair *);
void dsl_hmap_iter_set_at(dsl_hmap_iter *, dsl_pair *);
void dsl_hmap_iter_remove_at(dsl_hmap_iter *, dsl_pair *);

size_t dsl_hmap_len(dsl_hmap *);
void dsl_hmap_iter(dsl_hmap *, dsl_hmap_iter *);
void dsl_hmap_iter_at(dsl_hmap *, dsl_hmap_iter *, DSL_DATA);
void dsl_hmap_set_pair(dsl_hmap *, dsl_pair *);

#endif
