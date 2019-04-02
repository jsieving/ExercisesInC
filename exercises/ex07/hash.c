/* Example code for Exercises in C.

Copyright 2016 Allen Downey
License: Creative Commons Attribution-ShareAlike 3.0

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const float RESIZE_LOWER_BOUND = 0.25;
const float RESIZE_UPPER_BOUND = 1;

// VALUE: represents a value in a key-value pair

/* Here's one way of making a polymorphic object in C */

typedef struct {
    enum Type {INT, STRING} type;
    union {
        int i;
        char *s;
    };
} Value;


/* Makes a Value object that contains an int.
*
* i: value to store.
*
* returns: pointer to a new Value
*/
Value *make_int_value(int i)
{
    Value *value = (Value *) malloc (sizeof (Value));
    value->type = INT;
    value->i = i;
    return value;
}


/* Makes a Value object that contains a string.
*
* s: value to store.
*
* returns: pointer to a new Value
*/
Value *make_string_value(char *s)
{
    Value *value = (Value *) malloc (sizeof (Value));
    value->type = STRING;
    value->s = s;
    return value;
}


/* Prints a value object.
*
* value: pointer to Value
*
*/
void print_value (Value *value)
{
    if (value == NULL) {
        printf ("%p", value);
        return;
    }
    switch (value->type) {
        case INT:
        printf ("%d", value->i);
        break;
        case STRING:
        printf ("%s", value->s);
        break;
    }
}

// HASHABLE: Represents a key in a key-value pair.

/* Here's another way to make a polymorphic object.

The key can be any pointer type.  It's stored as a (void *), so
when you extract it, you have to cast it back to whatever it is.

`hash` is a pointer to a function that knows how to hash the key.
`equal` is a pointer to a function that knows how to compare keys.

*/

typedef struct {
    void *key;
    int (*hash) (void *);
    int (*equal) (void *, void *);
    void (*free_me) (void*);
} Hashable;


/* Makes a Hashable object.
*
* key: pointer to anything
* hash: function that can hash keys
* equal: function that compares keys
*
* returns: pointer to Hashable
*
*/
Hashable *make_hashable(void *key,
    int (*hash) (void *),
    int (*equal) (void *, void *),
    void (*free_me) (void*))
{
    Hashable *hashable = (Hashable *) malloc (sizeof (Hashable));
    hashable->key = key;
    hashable->hash = hash;
    hashable->equal = equal;
    hashable->free_me = free_me;
    return hashable;
}

/* Frees the key of a Hashable object, depending on
*  whether it is a type that needs to be freed.
*
* key: pointer to the key of the Hashable.
*/
void free_int(void* key) {
    free(key);
}

void free_str(void* key) {
    return;
}

/* Frees a Hashable object.
*
* hashable: Hashable to be freed.
*/
void free_hashable(Hashable* hashable) {
  hashable->free_me(hashable->key);
  free(hashable);
}


/* Prints a Hashable object.
*
* hashable: pointer to hashable
*/
void print_hashable(Hashable *hashable)
{
    printf ("key %p\n", hashable->key);
    printf ("hash %p\n", hashable->hash);
}


/* Hashes an integer.
*
* p: pointer to integer
*
* returns: integer hash value
*/
int hash_int(void *p)
{
    return *(int *)p;
}


/* Hashes a string.
*
* p: pointer to first char of a string
*
* returns: integer hash value
*/
int hash_string(void *p)
{
    char *s = (char *) p;
    int total = 0;
    int i = 0;

    while (s[i] != 0) {
        total += (223 * s[i]) >> 4;
        i++;
    }
    return total;
}


/* Hashes any Hashable.
*
* hashable: Hashable object
*
* returns: int hash value
*/
int hash_hashable(Hashable *hashable)
{
    return hashable->hash (hashable->key);
}


/* Compares integers.
*
* ip: pointer to int
* jp: pointer to int
*
* returns: 1 if equal, 0 otherwise
*/
int equal_int (void *ip, void *jp)
{
    int i = *(int *) ip;
    int j = *(int *) jp;
    return i == j;
}


/* Compares strings.
*
* s1: pointer to first char of a string
* s2: pointer to first char of a string
*
* returns: 1 if equal, 0 otherwise
*/
int equal_string (void *s1, void *s2)
{
    return (strcmp(s1, s2) == 0);
}


/* Compares Hashables.
*
* h1: Hashable
* h2: Hashable of the same type
*
* returns: 1 if equal, 0 otherwise
*
*/
int equal_hashable(Hashable *h1, Hashable *h2)
{
    return h1->equal (h1->key, h2->key);
}


/* Makes a Hashable int.
*
* Allocates space and copies the int.
*
* x: integer to store
*
* returns: Hashable
*/
Hashable *make_hashable_int (int x)
{
    int *p = (int *) malloc (sizeof (int));
    *p = x;
    return make_hashable((void *) p, hash_int, equal_int, free_int);
}


/* Makes a Hashable int.
*
* Stores a reference to the string (not a copy).
*
* s: string to store
*
* returns: Hashable
*/
Hashable *make_hashable_string (char *s)
{
    return make_hashable((void *) s, hash_string, equal_string, free_str);
}


// NODE: a node in a list of key-value pairs

typedef struct node {
    Hashable *key;
    Value *value;
    struct node *next;
} Node;


/* Makes a Node. */
Node *make_node(Hashable *key, Value *value, Node *next)
{
    Node *node = (Node *) malloc (sizeof (Node));
    node->key = key;
    node->value = value;
    node->next = next;
    return node;
}

/* Frees a Node. */
void free_node(Node* node) {
  free_hashable(node->key);
  free(node->value);
  free(node);
}


/* Prints a Node. */
void print_node(Node *node)
{
    print_hashable(node->key);
    printf ("value %p\n", node->value);
    printf ("next %p\n", node->next);
}


/* Prints all the Nodes in a list. */
void print_list(Node *node)
{
    if (node == NULL) {
        return;
    }
    print_hashable(node->key);
    printf ("value %p\n", node->value);
    print_list(node->next);
}


/* Prepends a new key-value pair onto a list.

This is actually a synonym for make_node.
*/
Node *prepend(Hashable *key, Value *value, Node *rest)
{
    return make_node(key, value, rest);
}


/* Looks up a key and returns the corresponding value, or NULL */
Value *list_lookup(Node *list, Hashable *key)
{
    while (list != NULL) {
      if (equal_hashable(list->key, key)) {
        return list->value;
      }
      list = list->next;
    }
    return NULL;
}


// MAP: a map is an array of lists of key-value pairs

typedef struct map {
    int n;
    int size;
    Node **lists;
} Map;


/* Makes a Map with n lists. */
Map *make_map(int n)
{
    int i;

    Map *map = (Map *) malloc (sizeof (Map));
    map->n = n;
    map->size = 0;
    map->lists = (Node **) calloc (n, sizeof (Node *));
    for (i=0; i<n; i++) {
        map->lists[i] = NULL;
    }
    return map;
}

/* Frees a Map. */
void free_map(Map *map) {
  Node* curr;
  Node* next;
  for (int bucket = 0; bucket < map->n; bucket++) {
    curr = map->lists[bucket];
    while (curr != NULL) {
      next = curr->next;
      free_node(curr);
      curr = next;
    }
  }
  free(map->lists);
  free(map);
}


/* Prints a Map. */
void print_map(Map *map)
{
    int i;
    printf ("Map with %d buckets, %d items\n", map->n, map->size);
    for (i=0; i<map->n; i++) {
        if (map->lists[i] != NULL) {
            printf ("%d\n", i);
            print_list (map->lists[i]);
        }
    }
}


/* Looks up a key and returns the corresponding Node, or NULL */
Node *get(Map *map, Hashable *key)
{
    int bucket = hash_hashable(key) % map->n;
    Node* list = map->lists[bucket];
    while (list != NULL) {
      if (equal_hashable(list->key, key)) {
        return list;
      }
      list = list->next;
    }
    return NULL;
}


void map_resize(Map *map, int change) {
  Node** storage = (Node **) malloc (sizeof (Node *) * map->size);
  Node* curr;
  Node* next;
  int i = 0;
  int bucket;
  for (bucket = 0; bucket < map->n; bucket++) {
    curr = map->lists[bucket];
    while (curr != NULL) {
      next = curr->next;
      curr->next = NULL;
      storage[i] = curr;
      curr = next;
      i++;
    }
  }
  free(map->lists);
  if (change == 1) {map->n = 2 * map->n;}
  else {map->n = map->n / 2;}
  map->lists = (Node **) calloc (map->n, sizeof (Node *));
  while (i > 0) {
    i--;
    curr = storage[i];
    bucket = hash_hashable(curr->key) % map->n;
    Node* list = map->lists[bucket];
    map->lists[bucket] = prepend(curr->key, curr->value, list);
    free(curr);
  }
  free(storage);
}

/* Adds a key-value pair to a map. */
void map_add(Map *map, Hashable *key, Value *value)
{
    Node* existing = get(map, key);
    if (existing) {
      existing->value = value;
      return;
    }
    if ((float) (map->size + 1)/(float) map->n > RESIZE_UPPER_BOUND) {
      map_resize(map, 1);
    }
    int bucket = hash_hashable(key) % map->n;
    Node* list = map->lists[bucket];
    map->lists[bucket] = prepend(key, value, list);
    map->size++;
}

/* Removes a key-value pair from a map. */
void map_remove(Map *map, Hashable *key)
{
    if (get(map, key) == NULL) return;

    int bucket = hash_hashable(key) % map->n;
    Node* prev = NULL;
    Node* curr = map->lists[bucket];
    while (curr != NULL) {
      if (equal_hashable(curr->key, key)) {
        if (prev != NULL) {
          prev->next = curr->next;
        } else {
          map->lists[bucket] = curr->next;
        }
        free_node(curr);
      }
      prev = curr;
      curr = curr->next;
    }
    map->size--;
    if ((float) (map->size)/(float) map->n < RESIZE_LOWER_BOUND) {
      map_resize(map, -1);
    }
}

/* Looks up a key and returns the corresponding value, or NULL. */
Value *map_lookup(Map *map, Hashable *key)
{
    int bucket = hash_hashable(key) % map->n;
    Node* list = map->lists[bucket];
    return list_lookup(list, key);
}


/* Prints the results of a test lookup. */
void print_lookup(Value *value)
{
    printf ("Lookup returned ");
    print_value (value);
    printf ("\n");
}


int main ()
{
    Hashable *hashable1 = make_hashable_int (1);
    Hashable *hashable2 = make_hashable_string ("Apple");
    Hashable *hashable3 = make_hashable_int (2);

    // make a list by hand
    Value *value1 = make_int_value (17);
    Node *node1 = make_node(hashable1, value1, NULL);
    print_node (node1);
    puts("________\n");

    Value *value2 = make_string_value ("Orange");
    Node *list = prepend(hashable2, value2, node1);
    print_list (list);
    puts("________\n");

    // run some test lookups
    Value *value = list_lookup (list, hashable1);
    print_lookup(value);

    value = list_lookup (list, hashable2);
    print_lookup(value);

    value = list_lookup (list, hashable3);
    print_lookup(value);
    puts("________\n");

    free_node(list);
    free_node(node1);
    free_hashable(hashable3);

    // make a map
    Map *map = make_map(8);

    // add some ints to it
    for (int i = 0; i < 10; i++) {
      map_add(map, make_hashable_int(i), make_int_value(i));
    }

    print_map(map);

    // remove most of them
    Hashable *h;
    for (int i = 2; i < 12; i++) {
      h = make_hashable_int(i);
      map_remove(map, h);
      free_hashable(h);
    }

    puts("________\n");
    print_map(map);

    // add some strs to it
    char* strs[] = {"banana", "ananab", "aaabnn", "nanaba", "aaaann", "aaaanm"};
    for (int i = 0; i < 6; i++) {
      map_add(map, make_hashable_string(strs[i]), make_string_value(strs[i]));
    }

    puts("________\n");
    print_map(map);

    // remove "banana"
    h = make_hashable_string(strs[0]);
    map_remove(map, h);
    free_hashable(h);

    puts("________\n");
    print_map(map);

    free_map(map);

    return 0;
}
