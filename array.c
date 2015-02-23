/*
DSL -- Data Structure Library
array -- Arrays
*/

#include <stdlib.h>
#include <string.h>

#include "dsl.h"

void dsl_default_overalloc(dsl_overalloc *oalloc) {
    oalloc->con = 0;
    oalloc->prop = 1;
}

void dsl_default_array(dsl_array_type *arrtp) {
    dsl_default_type(dsl_as(dsl_type, arrtp));
    dsl_vt(dsl_type, arrtp, szobj) = sizeof(dsl_array);
    dsl_vt(dsl_type, arrtp, sztp) = sizeof(dsl_array_type);
    dsl_vt(dsl_type, arrtp, construct) = dsl_array_construct;
    dsl_vt(dsl_type, arrtp, destruct) = dsl_array_destruct;
    dsl_default_overalloc(&(arrtp->oalloc));
}

void dsl_array_construct(dsl_array_type *arrtp, dsl_array *arr) {
    dsl_sequence *seq = dsl_as(dsl_sequence, arr);
    dsl_sequence_construct(dsl_as(dsl_type, arrtp), seq);
    seq->len = dsl_array_len;
    seq->get = dsl_array_get;
    seq->set = dsl_array_set;
    seq->insert = dsl_array_insert;
    seq->remove = dsl_array_remove;
    seq->iter_start = dsl_array_iter_start;
    seq->iter_end = dsl_array_iter_end;
    seq->alloc = dsl_array_alloc;
    arr->data = NULL;
    arr->len = 0;
    arr->cap = 0;
    arr->oalloc = arrtp->oalloc;
}

void dsl_array_destruct(dsl_array *arr) {
	size_t i;
	dsl_retnull(arr,);
	for(i=0; i<arr->len; i++) {
		dsl_dvt(dsl_object, arr, incorp, unlink, arr->data[i]);
	}
	dsl_dvt(dsl_object, arr, alloc, free, array->data);
	dsl_sequence_destruct(dsl_as(dsl_sequence, arr));
}

void dsl_array_alloc(dsl_array *arr, size_t sz) {
	size_t i;
	dsl_retnull(arr,);
	if(arr->len > sz) {
		for(i=sz; i<arr->len; i++) {
		    dsl_dvt(dsl_object, arr, incorp, unlink, arr->data[i]);
		}
		arr->len = sz;
	}
	arr->data = dsl_dvt(dsl_object, arr, alloc, realloc, arr->data, sz * sizeof(DSL_DATA));
	arr->cap = sz;
}

size_t dsl_array_len(dsl_array *arr) {
    dsl_retnull(arr,)
	return array->len;
}

DSL_DATA dsl_array_get(dsl_array *arr, size_t idx) {
    dsl_retnull(arr, DSL_NULL);
	if(idx < 0 || idx >= array->len) {
		return DSL_NULL;
	}
	return arr->data[idx];
}

void dsl_array_set(dsl_array *arr, size_t idx, DSL_DATA value) {
	DSL_DATA temp;
	dsl_retnull(arr,);
	if(idx < 0 || idx >= arr->len) return;
	temp = arr->data[idx];
	arr->data[idx] = dsl_dvt(dsl_object, arr, incorp, link, value);
	dsl_dvt(dsl_object, arr, incorp, unlink, temp);
}

void dsl_array_insert(dsl_array *arr, size_t idx, DSL_DATA value) {
    dsl_retnull(arr,);
	if(idx < 0 || idx > arr->len) return;
	if(arr->len >= arr->cap) {
		array->cap += 1 + arr->oalloc.con + (arr->cap * arr->oalloc.prop);
		array->data = dsl_dvt(dsl_object, arr, alloc, realloc, array->data, array->cap * sizeof(DSL_DATATYPE));
	}
	memmove(arr->data + (idx+1), arr->data + idx, sizeof(DSL_DATA) * (arr->len - idx));
	arr->data[idx] = dsl_dvt(dsl_object, arr, incorp, link, value);
	arr->len++;
}

DSL_DATA dsl_array_remove(dsl_array *arr, size_t idx) {
	DSL_DATA ret;
	dsl_retnull(arr, DSL_NULL);
	if(idx < 0 || idx >= arr->len) return DSL_NULL;
	ret = arr->data[idx];
	memmove(arr->data + idx, arr->data + (idx+1), sizeof(DSL_DATA) * (arr->len - idx - 1));
	arr->len--;
	dsl_dvt(dsl_object, arr, incorp, unlink, ret);
	return ret;
}

void dsl_array_delete(dsl_array *arr, size_t idx) {
    dsl_retnull(arr,);
	dsl_array_remove(arr, idx);
}

void dsl_array_iter_start(dsl_array *arr, dsl_array_iter *it) {
    dsl_retnull(arr,);
    it->array = arr;
    it->index = 0;
}

void dsl_array_iter_end(dsl_array *arr, dsl_array_iter *it) {
    dsl_retnull(arr,);
    it->array = arr;
    it->index = arr->len - 1;
}

void dsl_array_iter_construct(dsl_type *tp, dsl_array_iter *it) {
    dsl_sequence_iter *seqit = dsl_as(dsl_sequence_iter, it);
    dsl_sequence_iter_construct(tp, seqit);
    seqit->

ssize_t dsl_array_iter_set_index(dsl_array_iter *it, size_t idx) {
    dsl_retnull(it, -1);
	if(idx < 0) idx = -1;
	if(idx >= dsl_dast(dsl_sequence, it->array, len)) idx = -1;
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
