#include <glib.h>
#include <glib/gstdio.h>
#include <fcntl.h>

// a pair of a word and its count.
typedef struct {
  gint* count;
  gchar* word;
} Pair;

// creates a Pair from a word and its count.
Pair* make_pair(gchar* word, gint* count) {
  Pair* pair = malloc(sizeof(Pair));
  pair->count = count;
  pair->word = word;
  return pair;
}

// frees a Pair.
void free_pair(void* pair) {
  free((Pair*)pair);
}

// com-pairs two pairs by their counts to sort in descending order.
gint com_pair(const void* a, const void* b, void* data) { // WHY does it require 3 params?
  gint* a_count = ((Pair*)a)->count;
  gint* b_count = ((Pair*)b)->count;
  return GPOINTER_TO_INT(b_count) - GPOINTER_TO_INT(a_count);
}

// prints a word and its count.
void print_pair(void* pair, void* data) { // This one too! Why no variadic functions??
  int count = GPOINTER_TO_INT(((Pair*)pair)->count);
  gchar* word = ((Pair*)pair)->word;
  printf("%d\t| '%s'\n", count, word);
}

// "enumerate key-values": for each key & value in a hashtable,
// create a pair from it and add it to a sequence
void enum_kvs(void* key, void* value, void* dest_list) {
  Pair* pair = make_pair((gchar*) key, (gint*) value);
  g_sequence_append((GSequence*) dest_list, pair);
}

int main(int argc, char* argv[]) {
  long int count; // count of the last word. long int because of a type issue.
  GHashTable* histogram = g_hash_table_new(g_str_hash, g_str_equal);
  gchar *filename;
  gchar *text; // text from file
  gchar *punctuation = "`~!#$^&*()_-+={}[]|\\/?:;'\"<>,.\n ";
  gchar **word_list; // list of words
  GError* error; // error buffer
  GSequence* pairs = g_sequence_new(free_pair); // sequence for sorted histogram

  // sets the source file, if one was provided.
  if (argc > 1) {
    filename = argv[1];
  } else {
    filename = "test.txt";
  }

  // read the file contents and check that this was successful
  gboolean res = g_file_get_contents(filename, &text, NULL, &error);
  if (res == 0) {
    puts("Error in main: g_file_get_contents: failed to read file.");
    return 1;
  }

  // split the text into words, then count them to build a histogram
  word_list = g_strsplit_set(text, punctuation, 0);
  int i = 0;
  while (word_list[i] != NULL) {
    if (word_list[i][0] != '\0') {
      count = (long int) g_hash_table_lookup(histogram, word_list[i]);
      count++;
      g_hash_table_insert(histogram, word_list[i], GINT_TO_POINTER(count));
    }
    i++;
  }
  // turn the key-value pairs in histogram into a sequence of Pairs in pairs
  g_hash_table_foreach(histogram, enum_kvs, pairs);
  // sort and print the pairs
  g_sequence_sort(pairs, com_pair, NULL);
  g_sequence_foreach(pairs, print_pair, NULL);
  g_strfreev (word_list);
}
