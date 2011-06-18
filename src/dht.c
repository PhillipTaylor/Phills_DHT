
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <syslog.h>
#include <assert.h>

#include <stdio.h>

#include "dht.h"

void **hash_table;
unsigned int lower_bound;
unsigned int upper_bound;

/* called on app start up */
void dht_init() {
	hash_table = NULL;
	lower_bound = 0;
	upper_bound = 0;
}

//TODO: This must logically call back to the dispatcher to shift relocated items to other nodes ?
bool dht_init_table(unsigned int new_lower_bound, unsigned int new_upper_bound, bool migrate) {

	unsigned int i;
	unsigned int hash_table_size;
	unsigned int absolute_position;
	void **new_hash_table;
	void *item;

	assert(new_upper_bound > new_lower_bound);

	/* ensure we can get the space for the new table */
	hash_table_size = upper_bound - lower_bound;
	printf("new upper bound: %d, lower_bound: %d, hash_table_size: %d\n", upper_bound, lower_bound, hash_table_size);
	new_hash_table = (void*) malloc(sizeof(hash_table_size));

	if (((size_t)new_hash_table) == -1) {
		/* call to malloc failed. dht_intialise fails */
		return false;
	}

	printf("here: hash table size: %d\n", hash_table_size);
	/* intialise new array */
	for (i = 0; i < hash_table_size; i++) {
		printf("  setting %d to NULL\n", i);
		new_hash_table[i] = NULL;
	}

	/* free the old table. Possible migrate data to new locations */
	if (dht_is_initialised()) {
		for (i = 0; i < dht_get_size(); i++) {

			/* there is overlap. move the item to the new location. */
			item = hash_table[i];
			if (item != NULL) {
				if (migrate &&
					i + lower_bound > new_lower_bound &&
					i + lower_bound < new_upper_bound) {

					absolute_position = (i + lower_bound);
					new_hash_table[(absolute_position - new_lower_bound)] = item;
				} else {
					free(item);
				}
			}

		}

		free(hash_table);
	}

	/* install new table */
	hash_table = new_hash_table;
	lower_bound = new_lower_bound;
	upper_bound = new_upper_bound;

	return true;
}

bool dht_is_initialised() {
	return (hash_table != NULL);
}

unsigned int dht_get_lower_bound() {
	assert(dht_is_initialised());
	return lower_bound;
}

unsigned int dht_get_upper_bound() {
	assert(dht_is_initialised());
	return upper_bound;
}

unsigned int dht_get_size() {
	assert(dht_is_initialised());
	return (upper_bound - lower_bound);
}

void *dht_read(unsigned int location) {
	assert(dht_is_initialised());
	return hash_table[location];
}

void dht_write(unsigned int location, void* data) {
	assert(dht_is_initialised());
	hash_table[location] = data;
}

