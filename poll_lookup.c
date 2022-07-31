/*
 * CSE30 Summer Session 1 '22 HW3
 * CSE30 username: cs30s122dv (TODO: Fill in)
 */

#include "poll_lookup.h"

/*
 * !!! DO NOT EDIT THIS FUNCTION !!!
 * main
 *
 * Arguments: argc, argv
 *
 * Operation: Main driver for the program, calls other funttions to:
 *            parse the options, allocate the hash table, load the table, print
 *out the table stats
 *            and make print population stats of the desired city/state
 * Returns:   EXIT_SUCCESS if all ok, EXIT_FAILURE otherwise
 * !!! DO NOT EDIT THIS FUNCTION !!!
 */
int main(int argc, char *argv[]) {
  node **table;
  unsigned long size = TABLE_SIZE;
  // name of csv file
  char *filename;
  int info = 0;

  // Indicates days we want stats for/to remove
  char *date = NULL;
  char *del_date = NULL;

  // Parse options
  if (!parse_opts(argc, argv, &filename, &size, &info, &date, &del_date)) {
    return EXIT_FAILURE;
  }

  // Allocate space for table
  if ((table = calloc(size, sizeof(node *))) == NULL) {
    fprintf(stderr, "%s: Unable to allocate space for hash table\n", argv[0]);
    return EXIT_FAILURE;
  }

  // Load records from file
  if (load_table(table, size, filename)) {
    return EXIT_FAILURE;
  }

  // Delete data first
  if (del_date) {
    char *stripped_date = strip_date(del_date);
    if (stripped_date) { // no malloc fail
      delete_date(table, size, stripped_date);
      free(stripped_date);
    } else {
      return EXIT_FAILURE;
    }
  }

  // Produce data for a single date
  if (date) {
    char *stripped_date = strip_date(date);
    if (stripped_date) { // no malloc fail
      print_date_stats(table, size, stripped_date);
      free(stripped_date);
    } else {
      return EXIT_FAILURE;
    }
  }

  // Print metadata
  if (info)
    print_info(table, size);

  // Clean up
  delete_table(table, size);

  return EXIT_SUCCESS;
}

/*
 * !!! DO NOT EDIT THIS FUNCTION !!!
 * hash
 *
 * Arguments: a null terminated string
 *
 * Operation: calculates a hash value for the string
 *
 * returns:   the hash value
 * !!! DO NOT EDIT THIS FUNCTION !!!
 */
unsigned long hash(char *str) {
  unsigned long hash = 0;
  unsigned int c;
#ifdef C_HASH
  while ((c = (unsigned char)*str++) != '\0') {
    hash = c + (hash << 6) + (hash << 16) - hash;
  }
#else
  while ((c = (unsigned char)*str++) != '\0') {
    hash = hashFun((unsigned long)c, hash);
  }
#endif
  return hash;
}

/*
 * add_node
 *
 * Arguments: linked list pointer head, year, month, day, hour, pm25, temp
 */
node *add_node(node *front, int year, int month, int day, int hour, int pm25,
    int temp) {
  // TODO: Implement add_node
  node *newNode = malloc(sizeof(node));
  if(newNode == NULL) {
    return NULL;
  }
  newNode -> year = year;
  newNode -> month = month;
  newNode -> day = day;
  newNode -> hour = hour;
  newNode -> pm25 = pm25;
  newNode -> temp = temp;
  
  if(front -> next != NULL) {
    newNode -> next = front -> next;
  }
  front -> next = newNode -> next;
  return newNode;
}

/*
 * print_date_stats
 * Print the average stats for this date
 *
 * Arguments: pointer to hash table, hash table size, date as a string in the
 *form YYYY-MM-DD
 */
void print_date_stats(node **table, unsigned long size, char *datestr) {
  // TODO: Implement print_data_stats

  unsigned long hashNum = hash(datestr);

  unsigned long num = hashFun(size, hashNum);

  node *row = table[num];

  int c = 0;
  int line[4];

  char *time = strtok(datestr, "-");
  while(time != NULL) {
    line[c] = atoi(time);
    time = strtok(NULL, "-");
    c++;
  }
  c = 0;

  int sumTemp = 0, minTemp = 0, maxTemp = 0, minpm25 = 0, maxpm25 = 0, sumpm25 = 0, count = 0;

  node *tempNode = row -> next;

  do {
    if(tempNode -> year == line[0]) {
      if(tempNode -> month == line[1]) {
        if(tempNode -> day == line[2]) {
          count++;
          sumpm25 += tempNode -> pm25;
          sumTemp += tempNode -> temp;
          if(tempNode -> pm25 > maxpm25) {
            maxpm25 = tempNode -> pm25;
          }
          if(tempNode -> pm25 < minpm25) {
            minpm25 = tempNode -> pm25;
          }

          if(tempNode -> temp > maxTemp) {
            maxTemp = tempNode -> temp;
          }
          if(tempNode -> temp < minTemp) {
            minTemp = tempNode -> temp;
          }
        }
      }
    } 
    tempNode = tempNode -> next;
  } while(tempNode -> next != NULL);

  int avgpm25 = sumpm25/count;
  int avgTemp = sumTemp/count;
  // Use the following formatting strings to print messages.
  if(count == 0) {
    fprintf(stdout, "Unable to find any data for the date %s.\n", datestr);
  }
  else {
    printf("Minimum pm2.5: %d\tMaximum pm2.5: %d\tAverage pm2.5: %d\n",
          minpm25, maxpm25, avgpm25);
    printf("Minimum temp: %d\tMaximum temp: %d\tAverage temp: %d\n",
          minTemp, maxTemp, avgTemp);
  }
}

/*
 * load_table
 * Allocate memory for the hash table of node*s
 *
 * Arguments: pointer to hash table, hash table size, file name
 */
int load_table(node **table, unsigned long size, char *filename) {
  // TODO: Implement load_table
  // read from txt
  char *buffer = NULL;		// pointer to stirng
	//char* table[TABLE_SIZE];
  size_t bufsize = 99;
  int line[6];
  FILE *fp = fopen(filename, "r");
  if(fp == NULL) {
    perror("load_table filename open");
    exit(1);
  }

  char timeStr[MAX_SIZE_DATESTR];
  char time[MAX_SIZE_DATESTR];

  unsigned long hashNum = 0;
  unsigned long num = 0;

	// read input 1st line
  buffer = (char *)malloc(bufsize * sizeof(char));
  if(buffer == NULL)
  {
    perror("load_table malloc");
    exit(1);
  }
  
  if(fgets(buffer, LINE_SIZE, fp) == NULL) {
    perror("load_table filename open");
    free(buffer);
    exit(1);
  }

  char *token = strtok(buffer, ",");
  int c = 0;


  while(token != NULL) {
    if(strcmp(token, "NA") == 0)
      token = "0";
    line[c] = atoi(token);
    token = strtok(NULL, ",");
    c++;
  }
  c = 0;

  while(fgets(buffer, 100, fp) != NULL) {
    token = strtok(buffer, ",");
      while(token != NULL) {
        if(strcmp(token, "NA") == 0) {
          token = "0";
        }

      line[c] = atoi(token);
      token = strtok(NULL, ",");
      c++;
      } 

      sprintf(time, "%d", line[0]);
      strcat(timeStr, time);
      strcat(timeStr, "-");
      sprintf(time, "%d", line[1]);
      strcat(timeStr, time);
      strcat(timeStr, "-");
      sprintf(time, "%d", line[2]);
      strcat(timeStr, time);
          // printf("timeStr = %s\n", timeStr);

      hashNum = hash(timeStr);
      num = hashFun(size, hashNum);
      

      if(node_lookup(table[num], sprintf(time, "%d", line[0]), sprintf(time, "%d", line[1]), sprintf(time, "%d", line[2]), sprintf(time, "%d", line[3])) != NULL) {
        fprintf(stderr, "load_table duplicate entry: %d-%d-%d %d\n", line[0], line[1], line[2], line[3]);
        continue;
      }

      if(add_node(table[num], sprintf(time, "%d", line[0]), sprintf(time, "%d", line[1]), sprintf(time, "%d", line[2]), sprintf(time, "%d", line[3]), sprintf(time, "%d", line[4]), sprintf(time, "%d", line[5])) == NULL) {
        fprintf(stderr, "load_table could not add %s\n", timeStr);
        continue;
      }



    printf("load_table duplicate entry: %d-%d-%d %d\n", line[0], line[1], line[2], line[3]);
    c = 0;
    timeStr[0] = '\0';
  }

  fclose(fp);
  free(buffer);
  return 0;
}

/*
 * print_info
 *
 * Arguments: pointer to a hash table, number of elements
 */
void print_info(node **table, unsigned long size) {
  // TODO: Implement print_info
  unsigned long numNode = 0, maxNode = 0, minNode = TABLE_SIZE, count = 0, empty = 0, tableS = TABLE_SIZE;
  node *tempNode;

  for(unsigned long i = 0; i < size; i++) {
    tempNode = table[i];
    if(tempNode -> next == NULL) {
      empty++;
    }
    while(tempNode -> next != NULL) {
      count++;
      tempNode = tempNode ->next;
    }
    numNode += count;
    if(count > maxNode) {
      maxNode = count;
    }
    if(count < minNode) {
      minNode = count;
    }

  }

  printf("Table size: %lu\n",tableS);
  printf("Total entries: %lu\n", numNode);
  printf("Longest chain: %lu\n", maxNode);
  printf("Shortest chain: %lu\n", minNode);
  printf("Empty buckets: %lu\n", empty);
}

/*
 * delete_date
 * Delete all nodes associated with a given date of form YYYY-MM-DD
 * All leading zeros have been removed in the date string
 */
void delete_date(node **table, unsigned long size, char *datestr) {
  unsigned long hashval = hash(datestr) % size;
  node *chain = table[hashval];
  node *tmp, *prev = NULL;
  node* new_head = chain;

  const char split[] = "-";
  char *token = strtok(datestr, split);
  int cols[COL_DAY+1];
  int c = 0;
  while (token != NULL) {
    cols[c] = atoi(token);
    token = strtok(NULL, split);
    c++;
  }

  while (chain != NULL) {
    tmp = chain->next;
    // Delete if matching
    if (chain->year == cols[COL_YEAR] && chain->month == cols[COL_MONTH]
        && chain->day == cols[COL_DAY]) {
      // Only link previous if there was a previous
      if (prev) {
        prev->next = tmp;
      // No previous: this was the head, now new head is the one in front of us
      } else {
        new_head = tmp;
      }
      free(chain);
    // If not matching, don't delete and set prev as usual
    } else {
      prev = chain;
    }
    chain = tmp;
  }

  table[hashval] = new_head;
}


/*
 * delete_table
 *
 * Arguments: pointer to hash table, hash table array size
 */
void delete_table(node **table, unsigned long size) {
  unsigned int i;
  node *tmp;
  node *curr_tmp;

  for (i = 0; i < size; i++) {
    curr_tmp = table[i];
    while (curr_tmp != NULL) {
      tmp = curr_tmp->next;
      free(curr_tmp);
      curr_tmp = tmp;
    }
  }

  free(table);
}