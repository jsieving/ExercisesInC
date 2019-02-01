/*
 * Program to evaluate card values from user input.
 * Taken from Head First C and modified by Jane Sieving.
 */

#include <stdio.h>
#include <stdlib.h>

/* Prompts the user for input and puts the reply in the given buffer.
   User input is truncated to the first two characters.

   card_name: buffer where result is stored
*/
void get_card_name(char * card_name) {
  puts("Enter a card name: ");
  scanf("%2s", card_name);
}

/* Translates the card name to a numeric value using the first character
   of the name for a face card/ace or the number of a number card.
   Face cards => 10
   Ace => 11
   1-10 => 1-10

   val: int where the card value is stored
*/
int get_face_value(char * card_name) {
  int val;
  if (card_name[0] == 'K' || card_name[0] == 'Q' || card_name[0] == 'J' ||
      card_name[0] == 'k' || card_name[0] == 'q' || card_name[0] == 'j') {
    val = 10;
  } else if (card_name[0] == 'A' || card_name[0] == 'a') {
    val = 11;
  } else {
    val = atoi(card_name);
  }
  printf("The card value is: %i\n", val);
  return val;
}

/* Adds the value of the most recent card to a running total.

   val: int containing the most recent card value
   total: int contating the running total
*/
int update_score(int val, int total) {
  total += val;
  printf("The total value is: %i\n\n", total);
  return total;
}

/* Repeatedly asks the user to name a card, determines their value,
   and keeps a running total of the values.

   total: running total of card values
   val: most recent card value
   card_name: most recent card name, shortened to 2 characters
*/
int main() {
  int total = 0;
  int val;
  char card_name[3];
  puts("Welcome to the pointless card game, \nwhere there is no winning and you always win! \nType Ctrl+C any time to quit. \n");
  while (1) {
    get_card_name(card_name);
    val = get_face_value(card_name);
    total = update_score(val, total);
    /* clears the rest of the card name from input */
    while((getchar())!='\n');
  }
  return 0;
}
