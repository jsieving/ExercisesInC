#include <glib.h>
#include <glib/gstdio.h>
#include <fcntl.h>

typedef struct {
  gint* count;
  gchar* word;
} Pair;

Pair* make_pair(gchar* word, gint* count) {
  Pair* pair = malloc(sizeof(Pair));
  pair->count = count;
  pair->word = word;
  return pair;
}

void free_pair(void* pair) {
  free((Pair*)pair);
}

gint com_pair(const void* a, const void* b, void* data) { // WHY does it require 3 params?
  gint* a_count = ((Pair*)a)->count;
  gint* b_count = ((Pair*)b)->count;
  return GPOINTER_TO_INT(b_count) - GPOINTER_TO_INT(a_count);
}

void print_pair(void* pair, void* data) { // This one too! Why no variadic functions??
  gint* count = ((Pair*)pair)->count;
  gchar* word = ((Pair*)pair)->word;
  printf("%d\t| '%s'\n", GPOINTER_TO_INT(count), word);
}

void enum_kvs(void* key, void* value, void* dest_list) {
  Pair* pair = make_pair((gchar*) key, (gint*) value);
  g_sequence_append((GSequence*) dest_list, pair); //key should be pair
}

int main() {
  long int count;
  GHashTable* histogram = g_hash_table_new(g_str_hash, g_str_equal);
  gchar *filename = "test.txt";
  gchar *text;
  gchar **word_list;
  GError* error;
  GSequence* pairs = g_sequence_new(free_pair);
  gboolean res = g_file_get_contents(filename, &text, NULL, &error);
  if (res == 0) {
    puts("Error in main: g_file_get_contents: failed to read file.");
    return 1;
  }
  word_list = g_strsplit_set(text, " ,.?!-\n", 0);
  int i = 0;
  while (word_list[i] != NULL) {
    if (word_list[i][0] != '\0') {
      count = (long int) g_hash_table_lookup(histogram, word_list[i]);
      count++;
      g_hash_table_insert(histogram, word_list[i], GINT_TO_POINTER(count));
    }
    i++;
  }
  g_hash_table_foreach(histogram, enum_kvs, pairs);
  g_sequence_sort(pairs, com_pair, NULL);
  g_sequence_foreach(pairs, print_pair, NULL);
  g_strfreev (word_list);
}
