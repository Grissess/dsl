/*
DSL -- Data Structure Library
array -- Arrays
*/

#include <stdlib.h>
#include <string.h>

#include "dsl.h"

dsl_overallocation *dsl_new_overalloc(void) {
	dsl_overallocation *ret = malloc(sizeof(dsl_overallocation));
	dsl_init_overalloc(ret);
	return ret;
}

void dsl_free_overalloc(dsl_overallocation *oa) {
	free(oa);
}

void dsl_init_overalloc(dsl_overallocation *oa) {
	oa->con = 0;
	oa->prop = 0;
}

void dsl_overalloc_set_const(dsl_overallocation *oa, size_t con) {
	oa->con = con;
}

void dsl_overalloc_set_prop(dsl_overallocation *oa, double prop) {
	oa->prop = prop;
}

dsl_overallocation DSL_DEFAULT_OA = {0, 0};

dsl_array *dsl_new_array(dsl_overallocation *oa, dsl_object_funcs *of) {
	dsl_array *ret = malloc(sizeof(dsl_array));
	if(!oa) oa = &DSL_DEFAULT_OA;
	if(!of) of = &DSL_DEFAULT_OF;
	ret->data = NULL;
	ret->len = 0;
	ret->cap = 0;
	ret->oa = *oa;
	ret->of = *of;
	return ret;
}

void dsl_free_array(dsl_array *array) {
	size_t i;
	if(!array) return;
	for(i=0; i<array->len; i++) {
		array->of.destr(array->data[i]);
	}
	free(array->data);
	free(array);
}

void dsl_array_alloc(dsl_array *array, size_t sz) {
	size_t i;
	if(!array) return;
	if(array->len > sz) {
		for(i=sz; i<array->len; i++) {
			array->of.destr(array->data[i]);
		}
		array->len = sz;
	}
	array->data = realloc(array->data, sz * sizeof(DSL_DATATYPE));
	array->cap = sz;
}

void dsl_array_fill(dsl_array *array, size_t sz, DSL_DATATYPE value) {
	size_t i;
	if(!array) return;
	dsl_array_alloc(array, sz);
	for(i=0; i<sz; i++) {
		array->data[i] = array->of.copy(value);
	}
	array->len = sz;
}

size_t dsl_array_len(dsl_array *array) {
	if(!array) return 0;
	return array->len;
}

DSL_DATATYPE dsl_array_get(dsl_array *array, size_t idx) {
	if(!array) return DSL_NULL;
	if(idx >= array->len) {
		return DSL_NULL;
	}
	return array->data[idx];
}

void dsl_array_set(dsl_array *array, size_t idx, DSL_DATATYPE value) {
	DSL_DATATYPE temp;
	if(!array) return;
	if(idx >= array->len) return;
	temp = array->data[idx];
	array->data[idx] = array->of.copy(value);
	array->of.destr(temp);
	
	//dsl_array_delete(array, idx);
	//dsl_array_insert(array, idx, value);
}

void dsl_array_insert(dsl_array *array, size_t idx, DSL_DATATYPE value) {
	if(!array) return;
	if(idx > array->len) return;
	if(array->len >= array->cap) {
		array->cap += 1 + array->oa.con + (array->cap * array->oa.prop);
		array->data = realloc(array->data, array->cap * sizeof(DSL_DATATYPE));
	}
	memmove(array->data + (idx+1), array->data + idx, sizeof(DSL_DATATYPE) * (array->len - idx));
	array->data[idx] = array->of.copy(value);
	array->len++;
}

DSL_DATATYPE dsl_array_remove(dsl_array *array, size_t idx) {
	DSL_DATATYPE ret;
	if(!array) return DSL_NULL;
	if(idx >= array->len) return DSL_NULL;
	ret = array->data[idx];
	memmove(array->data + idx, array->data + (idx+1), sizeof(DSL_DATATYPE) * (array->len - idx - 1));
	array->len--;
	array->of.destr(ret);
	return ret;
}

void dsl_array_delete(dsl_array *array, size_t idx) {
	if(!array) return;
	dsl_array_remove(array, idx);
}

dsl_array *dsl_array_copy(dsl_array *src) {
	size_t i;
	dsl_array *dst;
	if(!src) return dsl_new_array(NULL, NULL);
	dst = dsl_new_array(&(src->oa), &(src->of));
	if(src->len <= 0) return dst;
	dst->data = realloc(dst->data, sizeof(DSL_DATATYPE) * src->len);
	dst->cap = src->len;
	dst->len = src->len;
	/* This would work if invoking copyfunc weren't a must */
	/*memcpy(dst->data, src->data, sizeof(DSL_DATATYPE) * src->len);*/
	for(i=0; i<src->len; i++) {
		dst->data[i] = src->of.copy(src->data[i]);
	}
	return dst;
}

dsl_array *dsl_array_append(dsl_array *left, dsl_array *right) {
	dsl_array *ret;
	size_t sz, i;
	if(left) {
		if(right) {
			ret = dsl_new_array(&(left->oa), &(left->of));
			sz = left->len + right->len;
		} else {
			ret = dsl_new_array(&(left->oa), &(left->of));
			sz = left->len;
		}
	} else {
		if(right) {
			ret = dsl_new_array(&(right->oa), &(right->of));
			sz = right->len;
		} else {
			return dsl_new_array(NULL, NULL);
		}
	}
	if(sz <= 0) return ret;
	ret->data = realloc(ret->data, sizeof(DSL_DATATYPE) * sz);
	ret->cap = sz;
	ret->len = sz;
	for(i=0; i<(left?left->len:0); i++) {
		ret->data[i] = left->of.copy(left->data[i]);
	}
	for(i=0; i<(right?right->len:0); i++) {
		ret->data[i + (left?left->len:0)] = right->of.copy(right->data[i]);
	}
	return ret;
}

dsl_array_iter *dsl_new_array_iter(dsl_array *array) {
	dsl_array_iter *ret;
	if(!array) return NULL;
	ret = malloc(sizeof(dsl_array_iter));
	ret->array = array;
	ret->idx = (dsl_array_len(array)?0:-1);
	return ret;
}

dsl_array *dsl_array_iter_array(dsl_array_iter *it) {
	if(!it) return NULL;
	return it->array;
}

void dsl_free_array_iter(dsl_array_iter *it) {
	free(it);
}

ssize_t dsl_array_iter_seek(dsl_array_iter *it, size_t idx) {
	if(!it) return -1;
	if(idx >= dsl_array_len(it->array)) idx = dsl_array_len(it->array) - 1;
	it->idx = idx;
	return idx;
}

ssize_t dsl_array_iter_index(dsl_array_iter *it) {
	if(!it) return -1;
	return it->idx;
}

DSL_DATATYPE dsl_array_iter_at(dsl_array_iter *it) {
	if(!it) return DSL_NULL;
	return dsl_array_get(it->array, it->idx);
}

void dsl_array_iter_set_at(dsl_array_iter *it, DSL_DATATYPE value) {
	if(!it) return;
	dsl_array_set(it->array, it->idx, value);
}

void dsl_array_iter_insert_at(dsl_array_iter *it, DSL_DATATYPE value) {
	if(!it) return;
	dsl_array_insert(it->array, it->idx, value);
}

void dsl_array_iter_insert_after(dsl_array_iter *it, DSL_DATATYPE value) {
	if(!it) return;
	dsl_array_insert(it->array, it->idx+1, value);
}

DSL_DATATYPE dsl_array_iter_remove_at(dsl_array_iter *it) {
	if(!it) return DSL_NULL;
	return dsl_array_remove(it->array, it->idx);
}

void dsl_array_iter_delete_at(dsl_array_iter *it) {
	if(!it) return;
	dsl_array_delete(it->array, it->idx);
}

int dsl_array_iter_next(dsl_array_iter *it) {
	if(!it) return 0;
	if(it->idx + 1 >= dsl_array_len(it->array)) {
		it->idx = -1;
		return 0;
	}
	it->idx++;
	return 1;
}

int dsl_array_iter_prev(dsl_array_iter *it) {
	if(!it) return 0;
	if(it->idx - 1 < 0) return 0;
	it->idx--;
	return 1;
}

int dsl_array_iter_at_end(dsl_array_iter *it) {
	if(!it) return 1;
	return it->idx == dsl_array_len(it->array) - 1;
}

int dsl_array_iter_at_begin(dsl_array_iter *it) {
	if(!it) return 1;
	return it->idx == 0;
}

int dsl_array_iter_is_invalid(dsl_array_iter *it) {
	if(!it) return 1; //Emphatically
	return it->idx == -1 || it->idx >= dsl_array_len(it->array);
}