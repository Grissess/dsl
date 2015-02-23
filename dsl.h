#ifndef DSL_H
#define DSL_H

#ifndef DSL_DATA
typedef void *DSL_DATA;
#endif

#ifndef DSL_NULL
#define DSL_NULL 0
#endif

typedef struct {
    DSL_DATA (*link)(DSL_DATA);
    void (*unlink)(DSL_DATA);
} dsl_incorp;

DSL_DATA dsl_link_void(dsl_incorp *, DSL_DATA);
void dsl_unlink_void(dsl_incorp *, DSL_DATA);

extern dsl_incorp DSL_INCORP_DEFAULT;

typedef struct {
    //Pure Virtual
    int (*equal)(DSL_DATA, DSL_DATA);
    int (*less)(DSL_DATA, DSL_DATA);
    //Virtual
    int (*greater)(DSL_DATA, DSL_DATA);
    int (*lesseq)(DSL_DATA, DSL_DATA);
    int (*greatereq)(DSL_DATA, DSL_DATA);
    int (*noteq)(DSL_DATA, DSL_DATA);
} dsl_relational;

int dsl_relational_greater(dsl_relational *, DSL_DATA, DSL_DATA);
int dsl_relational_lesseq(dsl_relational *, DSL_DATA, DSL_DATA);
int dsl_relational_greatereq(dsl_relational *, DSL_DATA, DSL_DATA);
int dsl_relational_noteq(dsl_relational *, DSL_DATA, DSL_DATA);

typedef struct {
    void *(*alloc)(size_t);
    void (*free)(void *);
} dsl_allocator;

extern dsl_allocator DSL_ALLOCATOR_MALLOC;

typedef struct dsl_tag_object dsl_object;

typedef struct {
    dsl_incorp incorp;
    dsl_allocator alloc;
    dsl_relational rel;
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

typedef struct dsl_tag_iter dsl_iter;

typedef struct dsl_tag_iter {
    dsl_object _base;
    //Pure Virtual
    dsl_object *(*owner)(dsl_iter *);
    int (*next)(dsl_iter *);
    int (*prev)(dsl_iter *);
    int (*at_end)(dsl_iter *);
    int (*at_begin)(dsl_iter *);
    int (*valid)(dsl_iter *);
    DSL_DATA (*at)(dsl_iter *);
    void (*set_at)(dsl_iter *, DSL_DATA);
    void (*insert_at)(dsl_iter *, DSL_DATA);
    void (*insert_after)(dsl_iter *, DSL_DATA);
    DSL_DATA (*remove_at)(dsl_iter *);
} dsl_iter;

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
    void (*append)(dsl_sequence *, dsl_sequence *, dsl_type *);
    void (*map)(dsl_sequence *, DSL_DATA (*)(DSL_DATA));
    void (*filter)(dsl_sequence *, int (*)(DSL_DATA));
    void (*sort)(dsl_sequence *, int (*)(DSL_DATA, DSL_DATA));
} dsl_sequence;

void dsl_sequence_delete(dsl_sequence *, size_t);
void dsl_sequence_fill(dsl_sequence *, DSL_DATA);
void dsl_sequence_alloc(dsl_sequence *, size_t);
dsl_sequence *dsl_sequence_copy(dsl_sequence *, dsl_type *);
void dsl_sequence_append(dsl_sequence *, dsl_sequence *, dsl_type *);
void dsl_sequence_map(dsl_sequence *, DSL_DATA (*)(DSL_DATA));
void dsl_sequence_filter(dsl_sequence *, int (*)(DSL_DATA));
void dsl_sequence_sort(dsl_sequence *, int (*)(DSL_DATA, DSL_DATA));

typedef struct dsl_tag_sequence_iter {
    dsl_iter _base;
    //Pure Virtual
    size_t (*index)(dsl_sequence_iter *);
    //Virtual
    int (*set_index)(dsl_sequence_iter *, size_t);
    void (*delete_at)(dsl_sequence_iter *);
} dsl_sequence_iter;

int dsl_sequence_iter_set_index(dsl_iter *, size_t);
void dsl_sequence_iter_delete_at(dsl_iter *);

typedef struct {
    size_t con;
    double prop;
} dsl_overalloc;

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

extern dsl_array_type DSL_ARRAY_TYPE;

typedef struct dsl_tag_array_iter dsl_array_iter;

size_t dsl_array_len(dsl_array *);
DSL_DATA dsl_array_get(dsl_array *, size_t);
void dsl_array_set(dsl_array *, size_t, DSL_DATA);
void dsl_array_insert(dsl_array *, size_t, DSL_DATA);
DSL_DATA dsl_array_remove(dsl_array *, size_t);
void dsl_array_alloc(dsl_array *, size_t);
void dsl_array_iter_start(dsl_array *, dsl_array_iter *);
void dsl_array_iter_end(dsl_array *, dsl_array_iter *);

typedef struct dsl_tag_array_iter {
    dsl_sequence_iter _base;
    dsl_array *array;
    ssize_t index;
} dsl_array_iter;

dsl_array *dsl_array_iter_owner(dsl_array_iter *);
int dsl_array_iter_next(dsl_array_iter *);
int dsl_array_iter_prev(dsl_array_iter *);
int dsl_array_iter_at_end(dsl_array_iter *);
int dsl_array_iter_at_begin(dsl_array_iter *);
int dsl_array_iter_valid(dsl_array_iter *);
size_t dsl_array_iter_index(dsl_array_iter *);
int dsl_array_iter_set_index(dsl_array_iter *, size_t);
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

extern dsl_type DSL_LIST_TYPE;

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
    size_t (*len)(dsl_map *);
    DSL_DATA (*get)(dsl_map *, DSL_DATA);
    void (*set)(dsl_map *, DSL_DATA, DSL_DATA);
    //Virtual
    int (*has)(dsl_map *, DSL_DATA);
    dsl_map *(*copy)(dsl_map *, dsl_type *);
} dsl_map;

typedef struct dsl_tag_map_iter dsl_map_iter;

typedef struct dsl_tag_map_iter {
    dsl_iter _base;
} dsl_map_iter;



#endif
