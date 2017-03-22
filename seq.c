/*
DSL -- Data Structure Library
seq -- Sequences
*/

#include <stdlib.h>
#include <assert.h>

#include "dsl.h"

dsl_seq *dsl_seq_from_array(dsl_array *array) {
	dsl_seq *ret;
	if(!array) return NULL;
	ret = malloc(sizeof(dsl_seq));
	ret->type = DSL_SEQ_ARRAY;
	ret->array = array;
	return ret;
}

dsl_seq *dsl_seq_from_list(dsl_list *list) {
	dsl_seq *ret;
	if(!list) return NULL;
	ret = malloc(sizeof(dsl_seq));
	ret->type = DSL_SEQ_LIST;
	ret->list = list;
	return ret;
}

dsl_seq * dsl_seq_new_array(dsl_overallocation *oa, dsl_object_funcs *of) {
	return dsl_seq_from_array(dsl_new_array(oa, of));
}

dsl_seq *dsl_seq_new_list(dsl_object_funcs *of) {
	return dsl_seq_from_list(dsl_new_list(of));
}

size_t dsl_seq_len(dsl_seq *seq) {
	if(!seq) return 0;
	switch(seq->type) {
		case DSL_SEQ_ARRAY:
			return dsl_array_len(seq->array);
			break;
			
		case DSL_SEQ_LIST:
			return dsl_list_len(seq->list);
			break;
			
		default:
			assert(0);
	}
}

DSL_DATATYPE dsl_seq_get(dsl_seq *seq, size_t idx) {
	if(!seq) return DSL_NULL;
	switch(seq->type) {
		case DSL_SEQ_ARRAY:
			return dsl_array_get(seq->array, idx);
			break;
			
		case DSL_SEQ_LIST:
			return dsl_list_get(seq->list, idx);
			break;
			
		default:
			assert(0);
	}
}

void dsl_seq_set(dsl_seq *seq, size_t idx, DSL_DATATYPE value) {
	if(!seq) return;
	switch(seq->type) {
		case DSL_SEQ_ARRAY:
			dsl_array_set(seq->array, idx, value);
			break;
			
		case DSL_SEQ_LIST:
			dsl_list_set(seq->list, idx, value);
			break;
			
		default:
			assert(0);
	}
}

void dsl_seq_insert(dsl_seq *seq, size_t idx, DSL_DATATYPE value) {
	if(!seq) return;
	switch(seq->type) {
		case DSL_SEQ_ARRAY:
			dsl_array_insert(seq->array, idx, value);
			break;
			
		case DSL_SEQ_LIST:
			dsl_list_insert(seq->list, idx, value);
			break;
			
		default:
			assert(0);
	}
}

DSL_DATATYPE dsl_seq_remove(dsl_seq *seq, size_t idx) {
	if(!seq) return DSL_NULL;
	switch(seq->type) {
		case DSL_SEQ_ARRAY:
			return dsl_array_remove(seq->array, idx);
			break;
			
		case DSL_SEQ_LIST:
			return dsl_list_remove(seq->list, idx);
			break;
			
		default:
			assert(0);
	}
}

void dsl_seq_delete(dsl_seq *seq, size_t idx) {
	if(!seq) return;
	switch(seq->type) {
		case DSL_SEQ_ARRAY:
			dsl_array_delete(seq->array, idx);
			break;
			
		case DSL_SEQ_LIST:
			dsl_list_delete(seq->list, idx);
			break;
			
		default:
			assert(0);
	}
}

dsl_seq *dsl_seq_copy(dsl_seq *seq) {
	if(!seq) return NULL;
	switch(seq->type) {
		case DSL_SEQ_ARRAY:
			return dsl_seq_from_array(dsl_array_copy(seq->array));
			break;
			
		case DSL_SEQ_LIST:
			return dsl_seq_from_list(dsl_list_copy(seq->list));
			break;
			
		default:
			assert(0);
	}
}

dsl_seq_iter *dsl_new_seq_iter(dsl_seq *seq) {
	dsl_seq_iter *ret;
	if(!seq) return NULL;
	ret = malloc(sizeof(dsl_seq_iter));
	ret->type = seq->type;
	ret->seq = seq;
	switch(seq->type) {
		case DSL_SEQ_ARRAY:
			ret->array = dsl_new_array_iter(seq->array);
			break;
			
		case DSL_SEQ_LIST:
			ret->list = dsl_new_list_iter(seq->list);
			break;
			
		default:
			assert(0);
	}
	return ret;
}

dsl_seq *dsl_seq_iter_seq(dsl_seq_iter *it) {
	if(!it) return NULL;
	return it->seq;
}

void dsl_free_seq_iter(dsl_seq_iter *it) {
	switch(it->type) {
		case DSL_SEQ_ARRAY:
			dsl_free_array_iter(it->array);
			break;
			
		case DSL_SEQ_LIST:
			dsl_free_list_iter(it->list);
			break;
			
		default:
			assert(0);
			break;
	}
	free(it);
}

ssize_t dsl_seq_iter_seek(dsl_seq_iter *it, size_t idx) {
	if(!it) return -1;
	switch(it->type) {
		case DSL_SEQ_ARRAY:
			return dsl_array_iter_seek(it->array, idx);
			break;
			
		case DSL_SEQ_LIST:
			return dsl_list_iter_seek(it->list, idx);
			break;
			
		default:
			assert(0);
	}
}

ssize_t dsl_seq_iter_index(dsl_seq_iter *it) {
	if(!it) return -1;
	switch(it->type) {
		case DSL_SEQ_ARRAY:
			return dsl_array_iter_index(it->array);
			break;
			
		case DSL_SEQ_LIST:
			return dsl_list_iter_index(it->list);
			break;
			
		default:
			assert(0);
	}
}

DSL_DATATYPE dsl_seq_iter_at(dsl_seq_iter *it) {
	if(!it) return DSL_NULL;
	switch(it->type) {
		case DSL_SEQ_ARRAY:
			return dsl_array_iter_at(it->array);
			break;
			
		case DSL_SEQ_LIST:
			return dsl_list_iter_at(it->list);
			break;
			
		default:
			assert(0);
	}
}

void dsl_seq_iter_set_at(dsl_seq_iter *it, DSL_DATATYPE value) {
	if(!it) return;
	switch(it->type) {
		case DSL_SEQ_ARRAY:
			dsl_array_iter_set_at(it->array, value);
			break;
			
		case DSL_SEQ_LIST:
			dsl_list_iter_set_at(it->list, value);
			break;
			
		default:
			assert(0);
	}
}

void dsl_seq_iter_insert_at(dsl_seq_iter *it, DSL_DATATYPE value) {
	if(!it) return;
	switch(it->type) {
		case DSL_SEQ_ARRAY:
			dsl_array_iter_insert_at(it->array, value);
			break;
			
		case DSL_SEQ_LIST:
			dsl_list_iter_insert_at(it->list, value);
			break;
			
		default:
			assert(0);
	}
}

void dsl_seq_iter_insert_after(dsl_seq_iter *it, DSL_DATATYPE value) {
	if(!it) return;
	switch(it->type) {
		case DSL_SEQ_ARRAY:
			dsl_array_iter_insert_after(it->array, value);
			break;
			
		case DSL_SEQ_LIST:
			dsl_list_iter_insert_after(it->list, value);
			break;
			
		default:
			assert(0);
	}
}

DSL_DATATYPE dsl_seq_iter_remove_at(dsl_seq_iter *it) {
	if(!it) return DSL_NULL;
	switch(it->type) {
		case DSL_SEQ_ARRAY:
			return dsl_array_iter_remove_at(it->array);
			break;
			
		case DSL_SEQ_LIST:
			return dsl_list_iter_remove_at(it->list);
			break;
			
		default:
			assert(0);
	}
}

void dsl_seq_iter_delete_at(dsl_seq_iter *it) {
	if(!it) return;
	switch(it->type) {
		case DSL_SEQ_ARRAY:
			dsl_array_iter_delete_at(it->array);
			break;
			
		case DSL_SEQ_LIST:
			dsl_list_iter_delete_at(it->list);
			break;
			
		default:
			assert(0);
	}
}

int dsl_seq_iter_next(dsl_seq_iter *it) {
	if(!it) return 0;
	switch(it->type) {
		case DSL_SEQ_ARRAY:
			return dsl_array_iter_next(it->array);
			break;
			
		case DSL_SEQ_LIST:
			return dsl_list_iter_next(it->list);
			break;
			
		default:
			assert(0);
	}
}

int dsl_seq_iter_prev(dsl_seq_iter *it) {
	if(!it) return 0;
	switch(it->type) {
		case DSL_SEQ_ARRAY:
			return dsl_array_iter_prev(it->array);
			break;
			
		case DSL_SEQ_LIST:
			return dsl_list_iter_prev(it->list);
			break;
			
		default:
			assert(0);
	}
}

int dsl_seq_iter_at_end(dsl_seq_iter *it) {
	if(!it) return 1;
	switch(it->type) {
		case DSL_SEQ_ARRAY:
			return dsl_array_iter_at_end(it->array);
			break;
			
		case DSL_SEQ_LIST:
			return dsl_list_iter_at_end(it->list);
			break;
			
		default:
			assert(0);
	}
}

int dsl_seq_iter_at_begin(dsl_seq_iter *it) {
	if(!it) return 1;
	switch(it->type) {
		case DSL_SEQ_ARRAY:
			return dsl_array_iter_at_begin(it->array);
			break;
			
		case DSL_SEQ_LIST:
			return dsl_list_iter_at_begin(it->list);
			break;
			
		default:
			assert(0);
	}
}

int dsl_seq_iter_is_invalid(dsl_seq_iter *it) {
	if(!it) return 1; //Emphatically
	switch(it->type) {
		case DSL_SEQ_ARRAY:
			return dsl_array_iter_is_invalid(it->array);
			break;
			
		case DSL_SEQ_LIST:
			return dsl_list_iter_is_invalid(it->list);
			break;
			
		default:
			assert(0);
	}
}

dsl_array *dsl_seq_to_array(dsl_seq *seq) {
	dsl_array *array;
	dsl_array_iter *ia;
	dsl_object_funcs *of;
	dsl_seq_iter *is = dsl_new_seq_iter(seq);
	if(!seq) return dsl_new_array(NULL, NULL);
	switch(seq->type) {
		case DSL_SEQ_ARRAY:
			of = &(seq->array->of);
			array = dsl_new_array(&(seq->array->oa), of);
			break;
			
		case DSL_SEQ_LIST:
			of = &(seq->list->of);
			array = dsl_new_array(&DSL_DEFAULT_OA, of);
			break;
			
		default:
			assert(0);
	}
	ia = dsl_new_array_iter(array);
	if(!dsl_seq_iter_is_invalid(is)) do {
		dsl_array_iter_insert_after(ia, of->copy(dsl_seq_iter_at(is)));
		if(!dsl_array_iter_at_end(ia)) dsl_array_iter_next(ia);
	} while(dsl_seq_iter_next(is));
	dsl_free_array_iter(ia);
	dsl_free_seq_iter(is);
	return array;
}

dsl_list *dsl_seq_to_list(dsl_seq *seq) {
	dsl_list *list;
	dsl_list_iter *il;
	dsl_object_funcs *of;
	dsl_seq_iter *is = dsl_new_seq_iter(seq);
	if(!seq) return dsl_new_list(NULL);
	switch(seq->type) {
		case DSL_SEQ_ARRAY:
			of = &(seq->array->of);
			list = dsl_new_list(of);
			break;
			
		case DSL_SEQ_LIST:
			of = &(seq->list->of);
			list = dsl_new_list(of);
			break;
			
		default:
			assert(0);
	}
	il = dsl_new_list_iter(list);
	if(!dsl_seq_iter_is_invalid(is)) do {
		dsl_list_iter_insert_after(il, of->copy(dsl_seq_iter_at(is)));
		if(!dsl_list_iter_at_end(il)) dsl_list_iter_next(il);
	} while(dsl_seq_iter_next(is));
	dsl_free_list_iter(il);
	dsl_free_seq_iter(is);
	return list;
}

void dsl_free_seq(dsl_seq *seq) {
	if(!seq) return;
	switch(seq->type) {
		case DSL_SEQ_ARRAY:
			dsl_free_array(seq->array);
			break;
			
		case DSL_SEQ_LIST:
			dsl_free_list(seq->list);
			break;
			
		default:
			assert(0);
	}
	free(seq);
}

dsl_seq *dsl_seq_append(dsl_seq *left, dsl_seq *right) {
	dsl_seq *dst;
	dsl_seq_iter *il = dsl_new_seq_iter(left), *ir = dsl_new_seq_iter(right), *id;
	dsl_object_funcs *of;
	dsl_seqtype tp = left?left->type:(right?right->type:DSL_SEQ_ARRAY);
	dsl_overallocation *oa;
	switch(tp) {
		case DSL_SEQ_ARRAY:
			of = left?&(left->array->of):(right?&(right->array->of):NULL);
			oa = left?&(left->array->oa):(right?&(right->array->oa):NULL);
			dst = dsl_seq_new_array(oa, of);
			break;
			
		case DSL_SEQ_LIST:
			of = left?&(left->list->of):(right?&(right->list->of):NULL);
			dst = dsl_seq_new_list(of);
			break;
			
		default:
			assert(0);
	}
	id = dsl_new_seq_iter(dst);
	if(!dsl_seq_iter_is_invalid(il)) do {
		dsl_seq_iter_insert_after(id, dsl_seq_iter_at(il));
		if(!dsl_seq_iter_at_end(id)) dsl_seq_iter_next(id);
	} while(dsl_seq_iter_next(il));
	if(!dsl_seq_iter_is_invalid(ir)) do {
		dsl_seq_iter_insert_after(id, dsl_seq_iter_at(ir));
		if(!dsl_seq_iter_at_end(id)) dsl_seq_iter_next(id);
	} while(dsl_seq_iter_next(ir));
	dsl_free_seq_iter(il);
	dsl_free_seq_iter(ir);
	dsl_free_seq_iter(id);
	return dst;
}

void dsl_seq_map(dsl_seq *seq, dsl_map_func map) {
	dsl_seq_iter *it = dsl_new_seq_iter(seq);
	if(!it) return;
	if(!dsl_seq_iter_is_invalid(it)) do {
		dsl_seq_iter_set_at(it, map(dsl_seq_iter_at(it)));
	} while(dsl_seq_iter_next(it));
	dsl_free_seq_iter(it);
}

void dsl_seq_filter(dsl_seq *seq, dsl_filter_func filter) {
	dsl_seq_iter *it = dsl_new_seq_iter(seq);
	if(!it) return;
	if(!dsl_seq_iter_is_invalid(it)) do {
		if(filter(dsl_seq_iter_at(it))) {
			dsl_seq_iter_next(it);
		} else {
			dsl_seq_iter_delete_at(it);
		}
	} while(!dsl_seq_iter_is_invalid(it));
	dsl_free_seq_iter(it);
}

dsl_array *merge_sort(dsl_array *arr, size_t len, dsl_cmp_func cmp) {
	dsl_array *left, *right, *ret, *sleft, *sright;
	dsl_array_iter *il, *ir, *ia;
	DSL_DATATYPE l, r;
	size_t i, half=len/2;
	
	if(!arr || len<=0) {
		return NULL;
	}
	if(len==1) {
		return dsl_array_copy(arr);
	}
	
	left = dsl_new_array(&DSL_DEFAULT_OA, &(arr->of));
	right = dsl_new_array(&DSL_DEFAULT_OA, &(arr->of));
	dsl_array_fill(left, half, DSL_NULL);
	dsl_array_fill(right, len-half, DSL_NULL);
	for(i=0; i<half; i++) {
		dsl_array_set(left, i, dsl_array_get(arr, i));
	}
	for(i=half; i<len; i++) {
		dsl_array_set(right, i-half, dsl_array_get(arr, i));
	}
	
	sleft = merge_sort(left, half, cmp);
	sright = merge_sort(right, len-half, cmp);
	dsl_free_array(left);
	dsl_free_array(right);
	il = dsl_new_array_iter(sleft);
	ir = dsl_new_array_iter(sright);
	ret = dsl_new_array(&(arr->oa), &(arr->of));
	dsl_array_fill(ret, len, DSL_NULL);
	ia = dsl_new_array_iter(ret);
	
	while(!dsl_array_iter_is_invalid(il) && !dsl_array_iter_is_invalid(ir)) {
		l = dsl_array_iter_at(il);
		r = dsl_array_iter_at(ir);
		if(cmp(l, r)<0) {
			dsl_array_iter_set_at(ia, l);
			dsl_array_iter_next(il);
		} else {
			dsl_array_iter_set_at(ia, r);
			dsl_array_iter_next(ir);
		}
		if(!dsl_array_iter_at_end(ia)) dsl_array_iter_next(ia);
	}
	while(!dsl_array_iter_is_invalid(il)) {
		dsl_array_iter_set_at(ia, dsl_array_iter_at(il));
		if(!dsl_array_iter_at_end(ia)) dsl_array_iter_next(ia);
		dsl_array_iter_next(il);
	}
	while(!dsl_array_iter_is_invalid(ir)) {
		dsl_array_iter_set_at(ia, dsl_array_iter_at(ir));
		if(!dsl_array_iter_at_end(ia)) dsl_array_iter_next(ia);
		dsl_array_iter_next(ir);
	}
	
	dsl_free_array_iter(il);
	dsl_free_array_iter(ir);
	dsl_free_array_iter(ia);
	dsl_free_array(sleft);
	dsl_free_array(sright);
	
	return ret;
}

void dsl_seq_sort(dsl_seq *seq, dsl_cmp_func cmp) {
	dsl_array *arr, *sarr;
	dsl_array_iter *ia;
	dsl_seq_iter *is;
	size_t len = dsl_seq_len(seq);
	
	if(!seq || len<=1) {
		return;
	}
	
	arr = dsl_seq_to_array(seq);
	sarr = merge_sort(arr, len, cmp);
	dsl_free_array(arr);
	
	ia = dsl_new_array_iter(sarr);
	is = dsl_new_seq_iter(seq);
	do {
		dsl_seq_iter_set_at(is, dsl_array_iter_at(ia));
		sarr->of.destr(dsl_array_iter_at(ia));
		dsl_seq_iter_next(is);
	} while(dsl_array_iter_next(ia));
	dsl_free_array_iter(ia);
	dsl_free_seq_iter(is);
	dsl_free_array(sarr);
}
