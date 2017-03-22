/*
DSL -- Data Structure Library
list -- Lists
*/

#include <stdlib.h>

#include "dsl.h"

dsl_list *dsl_new_list(dsl_object_funcs *of) {
	dsl_list *ret = malloc(sizeof(dsl_list));
	if(!of) of = &DSL_DEFAULT_OF;
	ret->first = NULL;
	ret->of = *of;
	return ret;
}

dsl_list_iter *dsl_new_list_iter(dsl_list *list) {
	dsl_list_iter *ret;
	if(!list) return NULL;
	ret = malloc(sizeof(dsl_list_iter));
	ret->list = list;
	ret->cur = list->first;
	return ret;
}

dsl_list *dsl_list_iter_list(dsl_list_iter *it) {
	if(!it) return NULL;
	return it->list;
}

void dsl_free_list_iter(dsl_list_iter *it) {
	free(it);
}

ssize_t dsl_list_iter_seek(dsl_list_iter *it, size_t idx) {
	size_t i=0;
	if(!it) return -1;
	it->cur = it->list->first;
	if(!it->cur) return -1;
	while(it->cur->next && i<idx) {
		i++;
		it->cur = it->cur->next;
	}
	return i;
}

ssize_t dsl_list_iter_index(dsl_list_iter *it) {
	size_t i=0;
	dsl_listnode *node;
	if(!it) return -1;
	node = it->list->first;
	if(!it->cur || !node) return -1;
	while(node) {
		if(node == it->cur) return i;
		node = node->next;
		i++;
	}
	return -1;
}

DSL_DATATYPE dsl_list_iter_at(dsl_list_iter *it) {
	if(!it || !it->cur) return DSL_NULL;
	return it->cur->data;
}

void dsl_list_iter_set_at(dsl_list_iter *it, DSL_DATATYPE value) {
	DSL_DATATYPE temp;
	if(!it || !it->cur) return;
	temp = it->cur->data;
	it->cur->data = it->list->of.copy(value);
	it->list->of.destr(temp);
	
	/*if(dsl_list_iter_at_end(it)) {
		dsl_list_iter_delete_at(it);
		dsl_list_iter_insert_after(it, value);
		dsl_list_iter_next(it);
	} else {
		dsl_list_iter_delete_at(it);
		dsl_list_iter_insert_at(it, value);
	}*/
}

void dsl_list_iter_insert_at(dsl_list_iter *it, DSL_DATATYPE value) {
	dsl_listnode *node;
	if(!it) return;
	node = malloc(sizeof(dsl_listnode));
	node->data = it->list->of.copy(value);
	if(!it->list->first) {
		node->prev = NULL;
		node->next = NULL;
		it->list->first = node;
		it->cur = node;
	} else {
		if(!it->cur) {
			free(node);
			return;
		}
		node->next = it->cur;
		node->prev = it->cur->prev;
		if(it->cur->prev) it->cur->prev->next = node;
		it->cur->prev = node;
		if(it->cur == it->list->first) it->list->first = node;
		it->cur = node;
	}
}

void dsl_list_iter_insert_after(dsl_list_iter *it, DSL_DATATYPE value) {
	dsl_listnode *node;
	if(!it) return;
	node = malloc(sizeof(dsl_listnode));
	node->data = it->list->of.copy(value);
	if(!it->list->first) {
		node->prev = NULL;
		node->next = NULL;
		it->list->first = node;
		it->cur = node;
	} else {
		if(!it->cur) {
			free(node);
			return;
		}
		if(it->cur->next) {
			node->next = it->cur->next->next;
			if(it->cur->next->next) it->cur->next->next->prev = node;
		} else {
			node->next = NULL;
		}
		node->prev = it->cur;
		it->cur->next = node;
	}
}

DSL_DATATYPE dsl_list_iter_remove_at(dsl_list_iter *it) {
	dsl_listnode *node;
	DSL_DATATYPE *temp;
	if(!it) return DSL_NULL;
	node = it->cur;
	if(node->next) {
		it->cur = node->next;
	} else {
		it->cur = node->prev;
	}
	if(!node) return DSL_NULL;
	if(node->next) node->next->prev = node->prev;
	if(node->prev) node->prev->next = node->next;
	if(node == it->list->first) it->list->first = node->next;
	node->next = NULL;
	node->prev = NULL;
	temp = node->data;
	it->list->of.destr(temp);
	free(node);
	return temp;
}

void dsl_list_iter_delete_at(dsl_list_iter *it) {
	if(!it) return;
	dsl_list_iter_remove_at(it);
}

int dsl_list_iter_next(dsl_list_iter *it) {
	if(!it) return 0;
	if(it->cur) {
		it->cur = it->cur->next;
		return it->cur?1:0;
	} else return 0;
}

int dsl_list_iter_prev(dsl_list_iter *it) {
	if(!it) return 0;
	if(it->cur) {
		it->cur = it->cur->prev;
		return it->cur?1:0;
	} else return 0;
}

int dsl_list_iter_at_end(dsl_list_iter *it) {
	return !it || (it->cur && !it->cur->next);
}

int dsl_list_iter_at_begin(dsl_list_iter *it) {
	return !it || (it->cur && !it->cur->prev);
}

int dsl_list_iter_is_invalid(dsl_list_iter *it) {
	return !it || !it->cur;
}

void dsl_free_list(dsl_list *list) {
	dsl_list_iter *it = dsl_new_list_iter(list);
	dsl_listnode *cur, *prev;
	if(!list) return; //NB: it will be NULL too
	if(!dsl_list_iter_is_invalid(it)) do {
		list->of.destr(dsl_list_iter_at(it));
	} while(dsl_list_iter_next(it));
	dsl_free_list_iter(it);
	cur = list->first;
	while(cur) {
		prev = cur;
		cur = cur->next;
		free(prev);
	}
	free(list);
}

dsl_listnode *dsl_list_node(dsl_list *list, size_t idx) {
	dsl_listnode *ret;
	dsl_list_iter *it = dsl_new_list_iter(list);
	if(!list) return NULL;
	dsl_list_iter_seek(it, idx);
	ret = it->cur;
	dsl_free_list_iter(it);
	return ret;
}

size_t dsl_list_len(dsl_list *list) {
	size_t ret=1;
	dsl_list_iter *it;
	if(!list || !list->first) return 0;
	it = dsl_new_list_iter(list);
	while(dsl_list_iter_next(it)) ret++;
	dsl_free_list_iter(it);
	return ret;
}

DSL_DATATYPE dsl_list_get(dsl_list *list, size_t idx) {
	dsl_listnode *node = dsl_list_node(list, idx);
	if(!node) return DSL_NULL;
	return node->data;
}

void dsl_list_set(dsl_list *list, size_t idx, DSL_DATATYPE value) {
	dsl_listnode *node = dsl_list_node(list, idx);
	DSL_DATATYPE temp;
	if(!node) return;
	temp = node->data;
	node->data = list->of.copy(value);
	list->of.destr(temp);
}

void dsl_list_insert(dsl_list *list, size_t idx, DSL_DATATYPE value) {
	dsl_list_iter *it = dsl_new_list_iter(list);
	ssize_t ridx = dsl_list_iter_seek(it, idx);
	if(!list) return;
	if(idx == ridx+1) {
		dsl_list_iter_insert_after(it, value);
	} else if(idx == ridx) {
		dsl_list_iter_insert_at(it, value);
	}
	dsl_free_list_iter(it);
}

DSL_DATATYPE dsl_list_remove(dsl_list *list, size_t idx) {
	dsl_list_iter *it = dsl_new_list_iter(list);
	ssize_t ridx = dsl_list_iter_seek(it, idx);
	DSL_DATATYPE ret;
	if(!list) return DSL_NULL;
	if(idx == ridx) {
		ret = dsl_list_iter_remove_at(it);
	} else {
		ret = DSL_NULL;
	}
	dsl_free_list_iter(it);
	return ret;
}

void dsl_list_delete(dsl_list *list, size_t idx) {
	if(!list) return;
	list->of.destr(dsl_list_remove(list, idx));
}

dsl_list *dsl_list_copy(dsl_list *src) {
	dsl_list *dst;
	dsl_list_iter *is, *id;
	if(!src) return dsl_new_list(NULL);
	dst = dsl_new_list(&(src->of));
	is = dsl_new_list_iter(src);
	id = dsl_new_list_iter(dst);
	if(!dsl_list_iter_is_invalid(is)) do {
		dsl_list_iter_insert_after(id, dsl_list_iter_at(is));
		if(!dsl_list_iter_at_end(id)) dsl_list_iter_next(id);
	} while(dsl_list_iter_next(is));
	dsl_free_list_iter(is);
	dsl_free_list_iter(id);
	return dst;
}

dsl_list *dsl_list_append(dsl_list *left, dsl_list *right) {
	dsl_list *dst;
	dsl_list_iter *il = dsl_new_list_iter(left), *ir = dsl_new_list_iter(right), *id;
	dsl_object_funcs* of;
	if(left) {
		of = &(left->of);
	} else if(right) {
		of = &(right->of);
	} else {
		of = NULL;
	}
	dst = dsl_new_list(of);
	id = dsl_new_list_iter(dst);
	if(!dsl_list_iter_is_invalid(il)) do {
		dsl_list_iter_insert_after(id, of->copy(dsl_list_iter_at(il)));
		if(!dsl_list_iter_at_end(id)) dsl_list_iter_next(id);
	} while(dsl_list_iter_next(il));
	if(!dsl_list_iter_is_invalid(ir)) do {
		dsl_list_iter_insert_after(id, of->copy(dsl_list_iter_at(ir)));
		if(!dsl_list_iter_at_end(id)) dsl_list_iter_next(id);
	} while(dsl_list_iter_next(ir));
	dsl_free_list_iter(il);
	dsl_free_list_iter(ir);
	dsl_free_list_iter(id);
	return dst;
}