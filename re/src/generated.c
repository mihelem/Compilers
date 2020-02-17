
/* Generated GOTO code for DFA */
/* Michele Miccinesi 2020      */

#include "vector_macros.h"

vector_def(size_t);

typedef struct matches_t {
	vector_type(size_t) begins,
						lasts;
} matches_t;

matches_t *destroy_matches_t (matches_t matches[static 1]) {
	destroy_vector(size_t, &matches->begins);
	destroy_vector(size_t, &matches->lasts);

	return matches;
}

matches_t *place_matches_t (matches_t matches[static 1]) {
	matches->begins = vector(size_t, 0);
	matches->lasts = vector(size_t, 0);

	return matches;
}

vector_def(matches_t);
vector_type(size_t) search_automata (const uint8_t *buffer, size_t end){
	uint8_t c;
	size_t counter = 0;
	vector_type(size_t) lasts = vector(size_t, 0);
node0:
	if (counter == end) { return lasts; }
	c = buffer[counter];
	++counter;
	switch(c) {
	case 110:
		goto node1;
	default:
		return lasts;
	}
node1:
	if (counter == end) { return lasts; }
	c = buffer[counter];
	++counter;
	switch(c) {
	case 97:
		goto node2;
	case 110:
		goto node1;
	case 111:
		goto node3;
	default:
		return lasts;
	}
node2:
	if (counter == end) { return lasts; }
	c = buffer[counter];
	++counter;
	switch(c) {
	case 110:
		goto node1;
	default:
		return lasts;
	}
node3:
	if (counter == end) { return lasts; }
	c = buffer[counter];
	++counter;
	switch(c) {
	case 33:
		goto node2;
	case 110:
		goto node1;
	default:
		return lasts;
	}
}
matches_t reverse_automata (
	uint8_t const *buffer,
	vector_type(size_t) lasts[static 1],
	size_t const last_i){
	uint8_t c;
	size_t 	counter = 0;
	size_t const last = lasts->data[last_i];
	size_t lasts_counter = 0;

	matches_t matches;
	place_matches_t(&matches);
node0:
	while (lasts_counter <= last_i && lasts->data[last_i-lasts_counter] >= last-counter) {
		if (lasts->data[last_i-lasts_counter] == last-counter) {
			push_back_vector(size_t, &matches.lasts, last-counter);
			lasts->data[last_i-lasts_counter] = ~0;
		}
		++lasts_counter;
	}
	if (counter > last) {
		return matches;
	}

	c = buffer[last-counter]
	++counter;
	switch(c) {
	case 33:
		goto node1;
	case 97:
		goto node2;
	case 110:
		goto node3;
	default:
		return matches;
	}
node1:
	if (counter > last) {
		return matches;
	}

	c = buffer[last-counter]
	++counter;
	switch(c) {
	case 111:
		goto node2;
	default:
		return matches;
	}
node2:
	if (counter > last) {
		return matches;
	}

	c = buffer[last-counter]
	++counter;
	switch(c) {
	case 110:
		goto node3;
	default:
		return matches;
	}
node3:
	push_back_vector(size_t, &matches.begins, last-counter);
	if (counter > last) {
		return matches;
	}

	c = buffer[last-counter]
	++counter;
	switch(c) {
	default:
		return matches;
	}
}
string_t read_file (const char *filename) {
	string_t content;
	place_string_t(&text);

	FILE *file	= fopen(filename, "rb");
	if (!file) {
		perror("No file\n");
		exit(EXIT_FAILURE);
	}

	fseek(file, 0, SEEK_END);
	size_t length = ftell(file);
	fseek(file, 0, SEEK_SET);

	content.data = malloc(length+1);
	if (!text.data)
	{
		perror("Memory shortage\n");
		fclose(file);
		exit(EXIT_FAILURE);
	}
	content.end = length;

	fread(content.data, length, 1, file);
	fclose(file);

	return content;
}

void print_matches (matches_t *matches) {
	printf("
begins: [");
	forrall (&matches->begins, i) {
		printf("%lu, ", matches->begins[i]);
	}
	printf("]\nend: [");
	forrall (&matches->lasts, i) {
		printf("%lu, ", matches->lasts[i]);
	}
	printf("]\n");
}


int main(int argc, char **argv) {
	if (argc<2) {
		perror("No filename\n");
		exit(EXIT_FAILURE);
	}

	string_t content = read_file(argv[1]);

	vector_type(size_t) lasts = 
		 search_automata(content.data, content.end);

	vector_type(matches_t) all_matches = vector(matches_t, 0);

	forrall (lasts, i) {
		if (~lasts.data[i]) {
			push_back_vector(
				matches_t,
				&all_matches,
				reverse_automata(
					content.data,
					&lasts,
					i));
		}
	}

	forrall (&all_matches, i) {
		print_matches(&all_matches.data[i]);
		destroy_matches_t(&all_matches.data[i]);
	}
	destroy_vector(matches_t, &all_matches);
	destroy_vector(size_t, &lasts);
	destroy_string_t(&content);
}