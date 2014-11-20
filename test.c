/*
DSL -- Data Sequence Library
test -- Testing
*/

#include <stdio.h>
#include <stdlib.h>

#include "dsl.h"

#define AS_INT(x) (*((int *) (x)))

typedef struct {
	int value;
	int refs;
} dcell;

void destructor(dcell *a) {
	if(!a) {
		printf("<Called destructor for NULL>\n");
	} else {
		a->refs--;
		printf("<Called destructor for %d (refs now %d)>\n", a->value, a->refs);
		if(a->refs<=0) free(a);
	}
}

dcell *copier(dcell *a) {
	if(!a) {
		printf("<Called copier for NULL>\n");
	} else {
		a->refs++;
		printf("<Called copier for %d (refs now %d)>\n", a->value, a->refs);
	}
	return a;
}

dsl_object_funcs test_funcs = {(dsl_destructor) destructor, (dsl_copier) copier};

void pr_seq(dsl_seq *seq) {
	dsl_seq_iter *iter;
	dcell *v;
	size_t i;
	printf("Iterating using direct access:\n");
	for(i=0; i<dsl_seq_len(seq); i++) {
		v = dsl_seq_get(seq, i);
		if(!v) {
			printf("NULL\n");
		} else {
			printf("%d <%d>\n", v->value, v->refs);
		}
	}
	printf("Iterating using iterator:\n");
	iter = dsl_new_seq_iter(seq);
	if(!dsl_seq_iter_is_invalid(iter)) do {
		v = dsl_seq_iter_at(iter);
		if(!v) {
			printf("NULL\n");
		} else {
			printf("%d <%d>\n", v->value, v->refs);
		}
	} while(dsl_seq_iter_next(iter));
	dsl_free_seq_iter(iter);
}

dcell *map_3x(dcell *old) {
	dcell *ret;
	if(!old) return NULL;
	ret = malloc(sizeof(dcell));
	ret->value = old->value * 3;
	ret->refs = 0;
	return ret;
}

int filter_odd(dcell *a) {
	if(!a) return 0;
	return a->value % 2 == 1;
}

int sort_less(dcell *a, dcell *b) {
	int ret;
	printf("<Comparing ");
	if(a) {
		printf("%d", a->value);
	} else {
		printf("NULL");
	}
	printf(" and ");
	if(b) {
		printf("%d", b->value);
	} else {
		printf("NULL");
	}
	printf("> = ");
	if(!a || !b) ret = 0;
	else ret = (a->value < b->value)?-1:1;
	printf("%d\n", ret);
	return ret;
}

int test_array() {
	dsl_seq *seq;
	int res;
	printf("---Testing array as sequence---\n");
	seq = dsl_seq_new_array(&DSL_DEFAULT_OA, &test_funcs);
	res = test_seq(seq);
	//dsl_free_seq(seq);
	return res;
}

int test_list() {
	dsl_seq *seq;
	int res;
	printf("---Testing list as sequence---\n");
	seq = dsl_seq_new_list(&test_funcs);
	res = test_seq(seq);
	//dsl_free_seq(seq);
	return res;
}

int test_seq(dsl_seq *seq) {
	dsl_seq_iter *iter;
	DSL_DATATYPE temp;
	dsl_seq *seq2, *seq3;
	dcell *ONE, *TWO, *THREE, *FOUR, *FIVE;
	ONE = malloc(sizeof(dcell));
	TWO = malloc(sizeof(dcell));
	THREE = malloc(sizeof(dcell));
	FOUR = malloc(sizeof(dcell));
	FIVE = malloc(sizeof(dcell));
	ONE->value = 1;
	TWO->value = 2;
	THREE->value = 3;
	FOUR->value = 4;
	FIVE->value = 5;
	ONE->refs = 1;
	TWO->refs = 1;
	THREE->refs = 1;
	FOUR->refs = 1;
	FIVE->refs = 1;
	printf("Testing empty seq properties...\n");
	if(dsl_seq_len(seq)!=0) {
		printf("Array length not 0!\n");
		return 1;
	}
	iter = dsl_new_seq_iter(seq);
	if(!dsl_seq_iter_is_invalid(iter)) {
		printf("Iterator of empty seq valid!");
		return 1;
	}
	dsl_free_seq_iter(iter);
	printf("Inserting 1, 2, 3...\n");
	dsl_seq_insert(seq, 0, ONE);
	dsl_seq_insert(seq, 1, TWO);
	dsl_seq_insert(seq, 2, THREE);
	if(dsl_seq_len(seq)!=3) {
		printf("Seq not at three length after three inserts!\n");
		return 1;
	}
	pr_seq(seq);
	printf("Changing some entries...\n");
	dsl_seq_set(seq, 1, FIVE);
	dsl_seq_set(seq, 2, FOUR);
	pr_seq(seq);
	printf("Copying seqence...\n");
	seq2 = dsl_seq_copy(seq);
	printf("-Original-\n");
	pr_seq(seq);
	printf("-Copy-\n");
	pr_seq(seq2);
	printf("Deleting an entry...\n");
	temp = dsl_seq_remove(seq, 1);
	printf("Removed: %d\n", AS_INT(temp));
	pr_seq(seq);
	printf("Appending sequences...\n");
	seq3 = dsl_seq_append(seq, seq2);
	pr_seq(seq3);
	printf("Sorting...\n");
	dsl_seq_sort(seq3, (dsl_cmp_func) sort_less);
	pr_seq(seq3);
	printf("Using map (map_3x)...\n");
	dsl_seq_map(seq3, (dsl_map_func) map_3x);
	pr_seq(seq3);
	printf("Using filter (filter_odd)...\n");
	dsl_seq_filter(seq3, (dsl_filter_func) filter_odd);
	pr_seq(seq3);
	printf("Freeing sequences...\n");
	dsl_free_seq(seq2);
	dsl_free_seq(seq3);
	dsl_free_seq(seq);
	printf("Testing refcounts:\n1=%d, 2=%d, 3=%d, 4=%d, 5=%d\n", ONE->refs, TWO->refs, THREE->refs, FOUR->refs, FIVE->refs);
	if(ONE->refs!=1 || TWO->refs!=1 || THREE->refs!=1 || FOUR->refs!=1 || FIVE->refs!=1) {
		printf("ERROR: Refcount not one on release!\n");
		return 1;
	}
	free(ONE);
	free(TWO);
	free(THREE);
	free(FOUR);
	free(FIVE);
	printf("---Basic sequence tests passed!---\n");
	return 0;
}

int test_basic_list() {
	// TODO
	printf("<No basic list test>\n");
	return 0;
}

int main() {
	if(test_array() || test_list()) {
		printf("Test failed!\n");
		return 1;
	}
	printf("All tests passed!\n");
	return 0;
}