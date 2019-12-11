
/**
 * Loads in a history file for saving character occurences 
 * 
 * @param history_file 	The history file to read from.
 *			Defaults to "capture.history" if NULL
*/
void loadHistory(char* history_file);

/**
 * Writes back to a history file for saving character occurences 
 * 
 * @param history_file 	The history file to write to.
 *			Defaults to "capture.history" if NULL
*/
void saveHistory(char* history_file);
