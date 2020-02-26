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
		goto node7;
	case 55:
		goto node1;
	case 56:
		goto node2;
	case 57:
		goto node3;
	default:
		
		return accepted_states;
	}
node1:
	accept(counter, 1, &accepted_states);
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node1;
	case 49:
		goto node9;
	case 50:
		goto node10;
	case 51:
		goto node11;
	case 52:
		goto node8;
	case 53:
		goto node6;
	case 54:
		goto node7;
	case 55:
		goto node1;
	case 56:
		goto node9;
	case 57:
		goto node10;
	default:
		
		return accepted_states;
	}
node2:
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
		goto node7;
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
node3:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node7;
	case 49:
		goto node1;
	case 50:
		goto node63;
	case 51:
		goto node3;
	case 52:
		goto node4;
	case 53:
		goto node5;
	case 54:
		goto node6;
	case 55:
		goto node7;
	case 56:
		goto node1;
	case 57:
		goto node63;
	default:
		
		return accepted_states;
	}
node4:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node59;
	case 49:
		goto node4;
	case 50:
		goto node5;
	case 51:
		goto node6;
	case 52:
		goto node7;
	case 53:
		goto node1;
	case 54:
		goto node60;
	case 55:
		goto node59;
	case 56:
		goto node4;
	case 57:
		goto node5;
	default:
		
		return accepted_states;
	}
node5:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node6;
	case 49:
		goto node7;
	case 50:
		goto node1;
	case 51:
		goto node54;
	case 52:
		goto node55;
	case 53:
		goto node56;
	case 54:
		goto node5;
	case 55:
		goto node6;
	case 56:
		goto node7;
	case 57:
		goto node1;
	default:
		
		return accepted_states;
	}
node6:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node37;
	case 49:
		goto node38;
	case 50:
		goto node39;
	case 51:
		goto node40;
	case 52:
		goto node6;
	case 53:
		goto node7;
	case 54:
		goto node1;
	case 55:
		goto node37;
	case 56:
		goto node38;
	case 57:
		goto node39;
	default:
		
		return accepted_states;
	}
node7:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node8;
	case 49:
		goto node6;
	case 50:
		goto node7;
	case 51:
		goto node1;
	case 52:
		goto node9;
	case 53:
		goto node10;
	case 54:
		goto node11;
	case 55:
		goto node8;
	case 56:
		goto node6;
	case 57:
		goto node7;
	default:
		
		return accepted_states;
	}
node8:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node13;
	case 49:
		goto node7;
	case 50:
		goto node1;
	case 51:
		goto node32;
	case 52:
		goto node33;
	case 53:
		goto node34;
	case 54:
		goto node8;
	case 55:
		goto node13;
	case 56:
		goto node7;
	case 57:
		goto node1;
	default:
		
		return accepted_states;
	}
node9:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node29;
	case 49:
		goto node30;
	case 50:
		goto node13;
	case 51:
		goto node7;
	case 52:
		goto node1;
	case 53:
		goto node9;
	case 54:
		goto node31;
	case 55:
		goto node29;
	case 56:
		goto node30;
	case 57:
		goto node13;
	default:
		
		return accepted_states;
	}
node10:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node7;
	case 49:
		goto node1;
	case 50:
		goto node28;
	case 51:
		goto node10;
	case 52:
		goto node29;
	case 53:
		goto node30;
	case 54:
		goto node13;
	case 55:
		goto node7;
	case 56:
		goto node1;
	case 57:
		goto node28;
	default:
		
		return accepted_states;
	}
node11:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node12;
	case 49:
		goto node11;
	case 50:
		goto node8;
	case 51:
		goto node13;
	case 52:
		goto node7;
	case 53:
		goto node1;
	case 54:
		goto node14;
	case 55:
		goto node12;
	case 56:
		goto node11;
	case 57:
		goto node8;
	default:
		
		return accepted_states;
	}
node12:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node7;
	case 49:
		goto node1;
	case 50:
		goto node27;
	case 51:
		goto node12;
	case 52:
		goto node11;
	case 53:
		goto node8;
	case 54:
		goto node13;
	case 55:
		goto node7;
	case 56:
		goto node1;
	case 57:
		goto node27;
	default:
		
		return accepted_states;
	}
node13:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node9;
	case 49:
		goto node10;
	case 50:
		goto node16;
	case 51:
		goto node17;
	case 52:
		goto node13;
	case 53:
		goto node7;
	case 54:
		goto node1;
	case 55:
		goto node9;
	case 56:
		goto node10;
	case 57:
		goto node16;
	default:
		
		return accepted_states;
	}
node14:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node11;
	case 49:
		goto node8;
	case 50:
		goto node13;
	case 51:
		goto node7;
	case 52:
		goto node1;
	case 53:
		goto node14;
	case 54:
		goto node15;
	case 55:
		goto node11;
	case 56:
		goto node8;
	case 57:
		goto node13;
	default:
		
		return accepted_states;
	}
node15:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node7;
	case 49:
		goto node1;
	case 50:
		goto node14;
	case 51:
		goto node15;
	case 52:
		goto node11;
	case 53:
		goto node8;
	case 54:
		goto node13;
	case 55:
		goto node7;
	case 56:
		goto node1;
	case 57:
		goto node14;
	default:
		
		return accepted_states;
	}
node16:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node23;
	case 49:
		goto node16;
	case 50:
		goto node17;
	case 51:
		goto node13;
	case 52:
		goto node7;
	case 53:
		goto node1;
	case 54:
		goto node24;
	case 55:
		goto node23;
	case 56:
		goto node16;
	case 57:
		goto node17;
	default:
		
		return accepted_states;
	}
node17:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node13;
	case 49:
		goto node7;
	case 50:
		goto node1;
	case 51:
		goto node18;
	case 52:
		goto node19;
	case 53:
		goto node20;
	case 54:
		goto node17;
	case 55:
		goto node13;
	case 56:
		goto node7;
	case 57:
		goto node1;
	default:
		
		return accepted_states;
	}
node18:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node20;
	case 49:
		goto node17;
	case 50:
		goto node13;
	case 51:
		goto node7;
	case 52:
		goto node1;
	case 53:
		goto node18;
	case 54:
		goto node22;
	case 55:
		goto node20;
	case 56:
		goto node17;
	case 57:
		goto node13;
	default:
		
		return accepted_states;
	}
node19:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node7;
	case 49:
		goto node1;
	case 50:
		goto node21;
	case 51:
		goto node19;
	case 52:
		goto node20;
	case 53:
		goto node17;
	case 54:
		goto node13;
	case 55:
		goto node7;
	case 56:
		goto node1;
	case 57:
		goto node21;
	default:
		
		return accepted_states;
	}
node20:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node19;
	case 49:
		goto node20;
	case 50:
		goto node17;
	case 51:
		goto node13;
	case 52:
		goto node7;
	case 53:
		goto node1;
	case 54:
		goto node18;
	case 55:
		goto node19;
	case 56:
		goto node20;
	case 57:
		goto node17;
	default:
		
		return accepted_states;
	}
node21:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node20;
	case 49:
		goto node17;
	case 50:
		goto node13;
	case 51:
		goto node7;
	case 52:
		goto node1;
	case 53:
		goto node21;
	case 54:
		goto node19;
	case 55:
		goto node20;
	case 56:
		goto node17;
	case 57:
		goto node13;
	default:
		
		return accepted_states;
	}
node22:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node7;
	case 49:
		goto node1;
	case 50:
		goto node18;
	case 51:
		goto node22;
	case 52:
		goto node20;
	case 53:
		goto node17;
	case 54:
		goto node13;
	case 55:
		goto node7;
	case 56:
		goto node1;
	case 57:
		goto node18;
	default:
		
		return accepted_states;
	}
node23:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node7;
	case 49:
		goto node1;
	case 50:
		goto node26;
	case 51:
		goto node23;
	case 52:
		goto node16;
	case 53:
		goto node17;
	case 54:
		goto node13;
	case 55:
		goto node7;
	case 56:
		goto node1;
	case 57:
		goto node26;
	default:
		
		return accepted_states;
	}
node24:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node16;
	case 49:
		goto node17;
	case 50:
		goto node13;
	case 51:
		goto node7;
	case 52:
		goto node1;
	case 53:
		goto node24;
	case 54:
		goto node25;
	case 55:
		goto node16;
	case 56:
		goto node17;
	case 57:
		goto node13;
	default:
		
		return accepted_states;
	}
node25:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node7;
	case 49:
		goto node1;
	case 50:
		goto node24;
	case 51:
		goto node25;
	case 52:
		goto node16;
	case 53:
		goto node17;
	case 54:
		goto node13;
	case 55:
		goto node7;
	case 56:
		goto node1;
	case 57:
		goto node24;
	default:
		
		return accepted_states;
	}
node26:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node16;
	case 49:
		goto node17;
	case 50:
		goto node13;
	case 51:
		goto node7;
	case 52:
		goto node1;
	case 53:
		goto node26;
	case 54:
		goto node23;
	case 55:
		goto node16;
	case 56:
		goto node17;
	case 57:
		goto node13;
	default:
		
		return accepted_states;
	}
node27:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node11;
	case 49:
		goto node8;
	case 50:
		goto node13;
	case 51:
		goto node7;
	case 52:
		goto node1;
	case 53:
		goto node27;
	case 54:
		goto node12;
	case 55:
		goto node11;
	case 56:
		goto node8;
	case 57:
		goto node13;
	default:
		
		return accepted_states;
	}
node28:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node29;
	case 49:
		goto node30;
	case 50:
		goto node13;
	case 51:
		goto node7;
	case 52:
		goto node1;
	case 53:
		goto node28;
	case 54:
		goto node10;
	case 55:
		goto node29;
	case 56:
		goto node30;
	case 57:
		goto node13;
	default:
		
		return accepted_states;
	}
node29:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node10;
	case 49:
		goto node29;
	case 50:
		goto node30;
	case 51:
		goto node13;
	case 52:
		goto node7;
	case 53:
		goto node1;
	case 54:
		goto node9;
	case 55:
		goto node10;
	case 56:
		goto node29;
	case 57:
		goto node30;
	default:
		
		return accepted_states;
	}
node30:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node13;
	case 49:
		goto node7;
	case 50:
		goto node1;
	case 51:
		goto node9;
	case 52:
		goto node10;
	case 53:
		goto node29;
	case 54:
		goto node30;
	case 55:
		goto node13;
	case 56:
		goto node7;
	case 57:
		goto node1;
	default:
		
		return accepted_states;
	}
node31:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node7;
	case 49:
		goto node1;
	case 50:
		goto node9;
	case 51:
		goto node31;
	case 52:
		goto node29;
	case 53:
		goto node30;
	case 54:
		goto node13;
	case 55:
		goto node7;
	case 56:
		goto node1;
	case 57:
		goto node9;
	default:
		
		return accepted_states;
	}
node32:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node34;
	case 49:
		goto node8;
	case 50:
		goto node13;
	case 51:
		goto node7;
	case 52:
		goto node1;
	case 53:
		goto node32;
	case 54:
		goto node36;
	case 55:
		goto node34;
	case 56:
		goto node8;
	case 57:
		goto node13;
	default:
		
		return accepted_states;
	}
node33:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node7;
	case 49:
		goto node1;
	case 50:
		goto node35;
	case 51:
		goto node33;
	case 52:
		goto node34;
	case 53:
		goto node8;
	case 54:
		goto node13;
	case 55:
		goto node7;
	case 56:
		goto node1;
	case 57:
		goto node35;
	default:
		
		return accepted_states;
	}
node34:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node33;
	case 49:
		goto node34;
	case 50:
		goto node8;
	case 51:
		goto node13;
	case 52:
		goto node7;
	case 53:
		goto node1;
	case 54:
		goto node32;
	case 55:
		goto node33;
	case 56:
		goto node34;
	case 57:
		goto node8;
	default:
		
		return accepted_states;
	}
node35:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node34;
	case 49:
		goto node8;
	case 50:
		goto node13;
	case 51:
		goto node7;
	case 52:
		goto node1;
	case 53:
		goto node35;
	case 54:
		goto node33;
	case 55:
		goto node34;
	case 56:
		goto node8;
	case 57:
		goto node13;
	default:
		
		return accepted_states;
	}
node36:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node7;
	case 49:
		goto node1;
	case 50:
		goto node32;
	case 51:
		goto node36;
	case 52:
		goto node34;
	case 53:
		goto node8;
	case 54:
		goto node13;
	case 55:
		goto node7;
	case 56:
		goto node1;
	case 57:
		goto node32;
	default:
		
		return accepted_states;
	}
node37:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node51;
	case 49:
		goto node52;
	case 50:
		goto node6;
	case 51:
		goto node7;
	case 52:
		goto node1;
	case 53:
		goto node37;
	case 54:
		goto node53;
	case 55:
		goto node51;
	case 56:
		goto node52;
	case 57:
		goto node6;
	default:
		
		return accepted_states;
	}
node38:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node7;
	case 49:
		goto node1;
	case 50:
		goto node50;
	case 51:
		goto node38;
	case 52:
		goto node51;
	case 53:
		goto node52;
	case 54:
		goto node6;
	case 55:
		goto node7;
	case 56:
		goto node1;
	case 57:
		goto node50;
	default:
		
		return accepted_states;
	}
node39:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node46;
	case 49:
		goto node39;
	case 50:
		goto node40;
	case 51:
		goto node6;
	case 52:
		goto node7;
	case 53:
		goto node1;
	case 54:
		goto node47;
	case 55:
		goto node46;
	case 56:
		goto node39;
	case 57:
		goto node40;
	default:
		
		return accepted_states;
	}
node40:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node6;
	case 49:
		goto node7;
	case 50:
		goto node1;
	case 51:
		goto node41;
	case 52:
		goto node42;
	case 53:
		goto node43;
	case 54:
		goto node40;
	case 55:
		goto node6;
	case 56:
		goto node7;
	case 57:
		goto node1;
	default:
		
		return accepted_states;
	}
node41:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node43;
	case 49:
		goto node40;
	case 50:
		goto node6;
	case 51:
		goto node7;
	case 52:
		goto node1;
	case 53:
		goto node41;
	case 54:
		goto node45;
	case 55:
		goto node43;
	case 56:
		goto node40;
	case 57:
		goto node6;
	default:
		
		return accepted_states;
	}
node42:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node7;
	case 49:
		goto node1;
	case 50:
		goto node44;
	case 51:
		goto node42;
	case 52:
		goto node43;
	case 53:
		goto node40;
	case 54:
		goto node6;
	case 55:
		goto node7;
	case 56:
		goto node1;
	case 57:
		goto node44;
	default:
		
		return accepted_states;
	}
node43:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node42;
	case 49:
		goto node43;
	case 50:
		goto node40;
	case 51:
		goto node6;
	case 52:
		goto node7;
	case 53:
		goto node1;
	case 54:
		goto node41;
	case 55:
		goto node42;
	case 56:
		goto node43;
	case 57:
		goto node40;
	default:
		
		return accepted_states;
	}
node44:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node43;
	case 49:
		goto node40;
	case 50:
		goto node6;
	case 51:
		goto node7;
	case 52:
		goto node1;
	case 53:
		goto node44;
	case 54:
		goto node42;
	case 55:
		goto node43;
	case 56:
		goto node40;
	case 57:
		goto node6;
	default:
		
		return accepted_states;
	}
node45:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node7;
	case 49:
		goto node1;
	case 50:
		goto node41;
	case 51:
		goto node45;
	case 52:
		goto node43;
	case 53:
		goto node40;
	case 54:
		goto node6;
	case 55:
		goto node7;
	case 56:
		goto node1;
	case 57:
		goto node41;
	default:
		
		return accepted_states;
	}
node46:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node7;
	case 49:
		goto node1;
	case 50:
		goto node49;
	case 51:
		goto node46;
	case 52:
		goto node39;
	case 53:
		goto node40;
	case 54:
		goto node6;
	case 55:
		goto node7;
	case 56:
		goto node1;
	case 57:
		goto node49;
	default:
		
		return accepted_states;
	}
node47:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node39;
	case 49:
		goto node40;
	case 50:
		goto node6;
	case 51:
		goto node7;
	case 52:
		goto node1;
	case 53:
		goto node47;
	case 54:
		goto node48;
	case 55:
		goto node39;
	case 56:
		goto node40;
	case 57:
		goto node6;
	default:
		
		return accepted_states;
	}
node48:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node7;
	case 49:
		goto node1;
	case 50:
		goto node47;
	case 51:
		goto node48;
	case 52:
		goto node39;
	case 53:
		goto node40;
	case 54:
		goto node6;
	case 55:
		goto node7;
	case 56:
		goto node1;
	case 57:
		goto node47;
	default:
		
		return accepted_states;
	}
node49:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node39;
	case 49:
		goto node40;
	case 50:
		goto node6;
	case 51:
		goto node7;
	case 52:
		goto node1;
	case 53:
		goto node49;
	case 54:
		goto node46;
	case 55:
		goto node39;
	case 56:
		goto node40;
	case 57:
		goto node6;
	default:
		
		return accepted_states;
	}
node50:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node51;
	case 49:
		goto node52;
	case 50:
		goto node6;
	case 51:
		goto node7;
	case 52:
		goto node1;
	case 53:
		goto node50;
	case 54:
		goto node38;
	case 55:
		goto node51;
	case 56:
		goto node52;
	case 57:
		goto node6;
	default:
		
		return accepted_states;
	}
node51:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node38;
	case 49:
		goto node51;
	case 50:
		goto node52;
	case 51:
		goto node6;
	case 52:
		goto node7;
	case 53:
		goto node1;
	case 54:
		goto node37;
	case 55:
		goto node38;
	case 56:
		goto node51;
	case 57:
		goto node52;
	default:
		
		return accepted_states;
	}
node52:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node6;
	case 49:
		goto node7;
	case 50:
		goto node1;
	case 51:
		goto node37;
	case 52:
		goto node38;
	case 53:
		goto node51;
	case 54:
		goto node52;
	case 55:
		goto node6;
	case 56:
		goto node7;
	case 57:
		goto node1;
	default:
		
		return accepted_states;
	}
node53:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node7;
	case 49:
		goto node1;
	case 50:
		goto node37;
	case 51:
		goto node53;
	case 52:
		goto node51;
	case 53:
		goto node52;
	case 54:
		goto node6;
	case 55:
		goto node7;
	case 56:
		goto node1;
	case 57:
		goto node37;
	default:
		
		return accepted_states;
	}
node54:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node56;
	case 49:
		goto node5;
	case 50:
		goto node6;
	case 51:
		goto node7;
	case 52:
		goto node1;
	case 53:
		goto node54;
	case 54:
		goto node58;
	case 55:
		goto node56;
	case 56:
		goto node5;
	case 57:
		goto node6;
	default:
		
		return accepted_states;
	}
node55:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node7;
	case 49:
		goto node1;
	case 50:
		goto node57;
	case 51:
		goto node55;
	case 52:
		goto node56;
	case 53:
		goto node5;
	case 54:
		goto node6;
	case 55:
		goto node7;
	case 56:
		goto node1;
	case 57:
		goto node57;
	default:
		
		return accepted_states;
	}
node56:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node55;
	case 49:
		goto node56;
	case 50:
		goto node5;
	case 51:
		goto node6;
	case 52:
		goto node7;
	case 53:
		goto node1;
	case 54:
		goto node54;
	case 55:
		goto node55;
	case 56:
		goto node56;
	case 57:
		goto node5;
	default:
		
		return accepted_states;
	}
node57:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node56;
	case 49:
		goto node5;
	case 50:
		goto node6;
	case 51:
		goto node7;
	case 52:
		goto node1;
	case 53:
		goto node57;
	case 54:
		goto node55;
	case 55:
		goto node56;
	case 56:
		goto node5;
	case 57:
		goto node6;
	default:
		
		return accepted_states;
	}
node58:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node7;
	case 49:
		goto node1;
	case 50:
		goto node54;
	case 51:
		goto node58;
	case 52:
		goto node56;
	case 53:
		goto node5;
	case 54:
		goto node6;
	case 55:
		goto node7;
	case 56:
		goto node1;
	case 57:
		goto node54;
	default:
		
		return accepted_states;
	}
node59:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node7;
	case 49:
		goto node1;
	case 50:
		goto node62;
	case 51:
		goto node59;
	case 52:
		goto node4;
	case 53:
		goto node5;
	case 54:
		goto node6;
	case 55:
		goto node7;
	case 56:
		goto node1;
	case 57:
		goto node62;
	default:
		
		return accepted_states;
	}
node60:
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
		goto node7;
	case 52:
		goto node1;
	case 53:
		goto node60;
	case 54:
		goto node61;
	case 55:
		goto node4;
	case 56:
		goto node5;
	case 57:
		goto node6;
	default:
		
		return accepted_states;
	}
node61:
	if ((c = getchar())==EOF) { return accepted_states; }
	++counter;
	switch(c) {
	case 48:
		goto node7;
	case 49:
		goto node1;
	case 50:
		goto node60;
	case 51:
		goto node61;
	case 52:
		goto node4;
	case 53:
		goto node5;
	case 54:
		goto node6;
	case 55:
		goto node7;
	case 56:
		goto node1;
	case 57:
		goto node60;
	default:
		
		return accepted_states;
	}
node62:
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
		goto node7;
	case 52:
		goto node1;
	case 53:
		goto node62;
	case 54:
		goto node59;
	case 55:
		goto node4;
	case 56:
		goto node5;
	case 57:
		goto node6;
	default:
		
		return accepted_states;
	}
node63:
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
		goto node7;
	case 52:
		goto node1;
	case 53:
		goto node63;
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