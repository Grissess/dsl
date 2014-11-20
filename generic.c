/*
DSL -- Data Structure Library
generic -- Generic Functions
*/

#include "dsl.h"

void dsl_null_destruct(DSL_DATATYPE i) {}

DSL_DATATYPE dsl_null_copy(DSL_DATATYPE i) {
	return i;
}

dsl_object_funcs DSL_DEFAULT_OF = {dsl_null_destruct, dsl_null_copy};