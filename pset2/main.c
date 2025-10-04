#include "simplefind.h"

// prints all flags, and arguments. Intended for debugging getopt.
void print_options(bool ls_flag, bool xdev_flag, bool name_flag, char *name_pattern, char *starting_path) {
	printf("ls_flag: %d\n", ls_flag);
	printf("xdev_flag: %d\n", xdev_flag);
	printf("name_flag: %d\n", name_flag);
	printf("name pattern: %s\n", name_pattern);
	printf("starting path: %s\n", starting_path);
}

int main(int argc, char *argv[]) {
	bool ls_flag = false;
	bool xdev_flag = false;
	bool name_flag = false;
	char *name_pattern = NULL;
	char *starting_path = NULL;

	int opt;

	while((opt = getopt(argc, argv, "lxn:")) != -1){
		switch (opt) {
			case 'l':
				ls_flag = true;
				break;
			case 'x':
				xdev_flag = true;
				break;
			case 'n':
                                /*
				 * if the pattern contains a '/', and the pattern is not referring to
				 * the root directory '/', return an error.
				 */
				if  ((strchr(optarg + 1, '/') != NULL)  || (optarg[0] == '/' && strlen(optarg) != 1)) {
					fprintf(stderr, "Error: invalid syntax, name pattern should not include a '/'.\n"); 
					return 255;
				}

				name_flag = true;
				name_pattern = optarg;
				break;
			case '?':
				// if missing option or argument, return an error
				return 255;
		}
	}

	// if there is more than 1 non-option argument, return an error	
	if ((argc - optind)> 1) {
		fprintf(stderr, "Error: Too many arguments specified\n");
		return 255;
	} 

	// if no starting path specified, set the name pattern to just .
	if ((argc - optind) == 0) {
		starting_path = ".";
	} else if ((argc - optind) == 1) {
		starting_path = argv[optind];
	}
	
	// print the first directory entry. Only don't print it if there is the -n flag and the starting_path doesn't match the name_pattern
	if (name_flag) {
		if (fnmatch(name_pattern, starting_path, 0) == 0) {
			print_info(ls_flag, xdev_flag, 0, name_pattern, starting_path, starting_path, NULL);
		}
	} else {
		print_info(ls_flag, xdev_flag, 0, name_pattern, starting_path, starting_path, NULL);
	}

	recursive_dfs_search(ls_flag, xdev_flag, name_flag, name_pattern, starting_path);

	return 0;
}       

