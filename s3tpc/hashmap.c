#include "hashmap.h"


struct hash_map_entry {
	unsigned key;
	void *value;
	struct hash_map_entry *next;
};


struct hash_map {
	size_t capacity;
	size_t size;
	struct hash_map_entry **buckets;
};


struct hash_map *hm_create(size_t capacity) {
	struct hash_map *hm = malloc(sizeof(struct hash_map));
	if (hm == NULL) {
		return NULL;
	}
	hm->capacity = capacity;
	hm->size = 0;
	hm->buckets = calloc(hm->capacity, sizeof(struct hash_map_entry*));
	if (hm->buckets == NULL) {
		free(hm);
		return NULL;
	}
	return hm;
}


void hm_destroy(struct hash_map *hm) {
	struct hash_map_entry *current;
	struct hash_map_entry *next;
	size_t i;
	if (hm == NULL) {
		return;
	}
	for (i = 0; i < hm->capacity; i++) {
		current = hm->buckets[i];
		while (current) {
			next = current->next;
			free(current);
			current = next;
		}
	}
	free(hm->buckets);
	free(hm);
}


struct hash_map_entry **get_bucket(struct hash_map *hm, unsigned key) {
	size_t index = key % hm->capacity;
	return hm->buckets + index;
}


struct hash_map_entry *create_entry(unsigned key, void *value) {
	struct hash_map_entry *entry = malloc(sizeof(struct hash_map_entry));
	// TODO assert entry != NULL
	entry->key = key;
	entry->value = value;
	entry->next = NULL;
	return entry;
}


void *hm_insert(struct hash_map *hm, unsigned key, void *value) {
	void *old_value;
	struct hash_map_entry **bucket = get_bucket(hm, key);
	struct hash_map_entry *entry = *bucket;
	if (entry == NULL) {
		*bucket = create_entry(key, value);
		hm->size += 1;
		return NULL;
	}

	while (entry) {
		if (entry->key == key) {
			old_value = entry->value;
			entry->value = value;
			return old_value;
		}

		if (entry->next == NULL) {
			break;
		}

		entry = entry->next;
	}

	entry->next = create_entry(key, value);
	hm->size += 1;
	return NULL;
}


void *hm_remove(struct hash_map *hm, unsigned key) {
	void *old_value;
	struct hash_map_entry **bucket = get_bucket(hm, key);
	struct hash_map_entry *current;
	struct hash_map_entry *entry = *bucket;

	if (entry == NULL) {
		return NULL;
	}

	if (entry->key == key) {
		old_value = entry->value;
		*bucket = entry->next;
		free(entry);
		hm->size -= 1;
		return old_value;
	}

	while (entry->next) {
		current = entry->next;
		if (current->key == key) {
			old_value = current->value;
			entry->next = current->next;
			free(current);
			hm->size -= 1;
			return old_value;
		}
		entry = current;
	}

	return NULL;
}


void *hm_find(struct hash_map *hm, unsigned key) {
	struct hash_map_entry *entry = *get_bucket(hm, key);
	while (entry) {
		if (entry->key == key) {
			return entry->value;
		}
		entry = entry->next;
	}
	return NULL;
}


size_t hm_size(struct hash_map *hm) {
	return hm->size;
}
