/* Generated GOTO code for DFA   */
/* Michele Miccinesi 2020        */
/* ----------------------------- */
/* Recognise multiples of 7 in   */
/* base 10 with a lr scan        */
/* ----------------------------- */
/* Substitute getchar() with a   */
/* random generator in the range */
/* [0-9] to obtain a random gene-*/
/* rator of numbers divisibles   */
/* 7 in base 10                  */


#include "vector_macros.h"

typedef struct accepting_state_t {
	uint64_t counter, node_id;
} accepting_state_t;

#define destroy_accepting_state_t(state) 
vector_def_header(accepting_state_t);
vector_def(accepting_state_t);


vector_type(accepting_state_t) *accept(uint64_t counter, uint64_t node_id, vector_type(accepting_state_t) *accepted_states) {
	accepting_state_t state = {.counter=counter, .node_id=node_id};
	push_back_vector(accepting_state_t, accepted_states, state);
	return accepted_states;
}

vector_type(accepting_state_t) printer() {
	char c;
	uint64_t counter = 0;
	vector_type(accepting_state_t) accepted_states = vector(accepting_state_t, 0);

node0:
	accept(counter, 0, &accepted_states);
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node0;
	case 49:
		goto node1;
	case 50:
		goto node2;
	case 51:
		goto node3;
	case 52:
		goto node4;
	case 53:
		goto node5;
	case 54:
		goto node6;
	case 55:
		goto node0;
	case 56:
		goto node1;
	case 57:
		goto node2;
	default:
		
		return accepted_states;
	}
node1:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node3;
	case 49:
		goto node4;
	case 50:
		goto node5;
	case 51:
		goto node6;
	case 52:
		goto node0;
	case 53:
		goto node1;
	case 54:
		goto node2;
	case 55:
		goto node3;
	case 56:
		goto node4;
	case 57:
		goto node5;
	default:
		
		return accepted_states;
	}
node2:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node6;
	case 49:
		goto node0;
	case 50:
		goto node1;
	case 51:
		goto node2;
	case 52:
		goto node3;
	case 53:
		goto node4;
	case 54:
		goto node5;
	case 55:
		goto node6;
	case 56:
		goto node0;
	case 57:
		goto node1;
	default:
		
		return accepted_states;
	}
node3:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node2;
	case 49:
		goto node3;
	case 50:
		goto node4;
	case 51:
		goto node5;
	case 52:
		goto node6;
	case 53:
		goto node0;
	case 54:
		goto node1;
	case 55:
		goto node2;
	case 56:
		goto node3;
	case 57:
		goto node4;
	default:
		
		return accepted_states;
	}
node4:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node5;
	case 49:
		goto node6;
	case 50:
		goto node0;
	case 51:
		goto node1;
	case 52:
		goto node2;
	case 53:
		goto node3;
	case 54:
		goto node4;
	case 55:
		goto node5;
	case 56:
		goto node6;
	case 57:
		goto node0;
	default:
		
		return accepted_states;
	}
node5:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node1;
	case 49:
		goto node2;
	case 50:
		goto node3;
	case 51:
		goto node4;
	case 52:
		goto node5;
	case 53:
		goto node6;
	case 54:
		goto node0;
	case 55:
		goto node1;
	case 56:
		goto node2;
	case 57:
		goto node3;
	default:
		
		return accepted_states;
	}
node6:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node4;
	case 49:
		goto node5;
	case 50:
		goto node6;
	case 51:
		goto node0;
	case 52:
		goto node1;
	case 53:
		goto node2;
	case 54:
		goto node3;
	case 55:
		goto node4;
	case 56:
		goto node5;
	case 57:
		goto node6;
	default:
		
		return accepted_states;
	}
}

void print_acks (vector_type(accepting_state_t) *acks) {
	printf("\n---------------\n");
	forall (acks, i) {
		printf("%ld, ", acks->data[i].counter);
	}
	printf("\n");
}

int main() {
	vector_type(accepting_state_t) acks;
	while (true) {
		acks = printer();
		print_acks(&acks);
		clear_vector(accepting_state_t, &acks);
	}
	destroy_vector(accepting_state_t, &acks);
}