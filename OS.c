#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300

struct Dir;
struct File;

typedef struct Dir {
	char *name; 
	struct Dir* parent; 
	struct File* head_children_files; 
	struct Dir* head_children_dirs; 
	struct Dir* next; 
} Dir;  

typedef struct File {
	char *name;  
	struct Dir* parent;
	struct File* next; 
} File;  

int check_file_existence (Dir* current_folder, char* name) {
	File *temp_file;
	temp_file = current_folder->head_children_files;
	int error_file = 0;

	while (temp_file != NULL) {

		if (strcmp(temp_file->name, name) == 0) {

			error_file = 1;
		}
		temp_file = temp_file->next;
	}
	return error_file;
}

int check_dir_existence (Dir* current_folder, char* name) {
	Dir *temp_dir;
	temp_dir = current_folder->head_children_dirs;
	int error_dir = 0;

	while (temp_dir != NULL) {

		if (strcmp(temp_dir->name, name) == 0) {

			error_dir = 1;
		}
		temp_dir = temp_dir->next;
	}
	return error_dir;
}

void touch (Dir* parent, char* name) {
	int error_file, error_dir;

	error_file = check_file_existence(parent, name);
	error_dir = check_dir_existence(parent, name);

	if (error_file == 1) {

		printf("File already exists\n");
	}
	else if (error_dir == 1) {

		printf("Directory already exists\n");
	}
	else {
		File *new_file;
		new_file = (File *)malloc(sizeof(File));

		new_file->name = strdup(name);
		new_file->parent = parent;
		new_file->next = NULL;

		if (parent->head_children_files == NULL) {

			parent->head_children_files = new_file;
		}
		else {
			File *last_file = parent->head_children_files;

			while (last_file->next != NULL) {

				last_file = last_file->next;
			}
			last_file->next = new_file;
		}
	}
}

void mkdir (Dir* parent, char* name) {
	int error_file, error_dir;

	error_file = check_file_existence(parent, name);
	error_dir = check_dir_existence(parent, name);

	if (error_file == 1) {

		printf("File already exists\n");
	}
	else if (error_dir == 1) {

		printf("Directory already exists\n");
	}
	else {
		Dir *new_dir;
		new_dir = (Dir *)malloc(sizeof(Dir));

		new_dir->name = strdup(name);
		new_dir->parent = parent;
		new_dir->next = NULL;
		new_dir->head_children_files = NULL;
		new_dir->head_children_dirs = NULL;

		if (parent->head_children_dirs == NULL) {

			parent->head_children_dirs = new_dir;
		}
		else {
			Dir *last_dir = parent->head_children_dirs;

			while (last_dir->next != NULL) {

				last_dir = last_dir->next;
			}
			last_dir->next = new_dir;
		}
	}
}

void ls (Dir* parent) {

	if (parent->head_children_dirs != NULL) {
		Dir *temp_dir;
		temp_dir = parent->head_children_dirs;

		while (temp_dir != NULL) {

			printf("%s\n", temp_dir->name);
			temp_dir = temp_dir->next;
		}
	}

	if (parent->head_children_files != NULL) {
		File *temp_file;
		temp_file = parent->head_children_files;

		while (temp_file != NULL) {

			printf("%s\n", temp_file->name);
			temp_file = temp_file->next;
		}
	}
}

void rm (Dir* parent, char* name) {

	if (parent->head_children_files != NULL) {
		File *prev;
		prev = parent->head_children_files;
		File *temp_file;
		temp_file = parent->head_children_files->next;

		while (prev != NULL) {
			if (strcmp(prev->name, name) != 0 && prev->next == NULL) {

				break;
			}
			else if (strcmp(prev->name,name) == 0 && 
				strcmp(prev->name, parent->head_children_files->name) == 0) {

				parent->head_children_files = temp_file;
				free(prev->name);
				free(prev);
				return;
			}
			else if (strcmp(temp_file->name, name) == 0) {

				prev->next = temp_file->next;
				free(temp_file->name);
				free(temp_file);
				return;
			}
			temp_file = temp_file->next;
			prev = prev->next;
		}
		printf("Could not find the file\n");
	}
	else {

		printf("Could not find the file\n");
	}
}

void remove_files (Dir* current_folder) {

	File *file = current_folder->head_children_files;
	int count = 0;
	char **name_files = malloc(MAX_INPUT_LINE_SIZE*sizeof(char*));

	while (file != NULL) {

		name_files[count] = (char*)malloc(MAX_INPUT_LINE_SIZE+1);

		strcpy(name_files[count], file->name);	
		file = file->next;
		count++;
	}

	for (int i = 0; i < count; i++) {

		rm(current_folder, name_files[i]);
	}

	for (int i = 0; i < count; i++) {

		free(name_files[i]);
	}
	free(name_files);
}

void rmdir (Dir* parent, char* name) {
	Dir *temp_dir = parent->head_children_dirs;

	while (temp_dir != NULL) {
		if (strcmp(temp_dir->name,name) == 0) { 
			
		    if (parent->head_children_dirs == temp_dir) {
				parent->head_children_dirs = temp_dir->next;
			}
			else {
				Dir *prev = parent->head_children_dirs;

			    while (prev->next != temp_dir) {

					prev = prev->next;
				}
				prev->next = temp_dir->next;
			}

			Dir *sub_dir = temp_dir->head_children_dirs;

			while (sub_dir != NULL) {

				rmdir(temp_dir, sub_dir->name);
				sub_dir = temp_dir->head_children_dirs;
			}
			remove_files(temp_dir);
			free(temp_dir->name);
			free(temp_dir);
			return;
		}
		temp_dir = temp_dir->next;
	}
	printf("Could not find the dir\n");
}

void cd (Dir** target, char *name) {

	if (strcmp(name,"..") == 0) {

		if ((*target)->parent != NULL) {

			(*target) = (*target)->parent;
		}
	}
	else {
		Dir *dir_search;
		dir_search = (*target)->head_children_dirs;

		while (dir_search != NULL) {

			if(strcmp(dir_search->name, name) == 0) {

				(*target) = dir_search;
				return;
			}
			dir_search = dir_search->next;
		}
		printf("No directories found!\n");
	}
}

void recursive_path (Dir* target, char *path) {

	if (target->parent == NULL) {
		strcat(path, "/");
		strcat(path, target->name);
		return;
	}

	recursive_path(target->parent, path);
	strcat(path, "/");
	strcat(path, target->name);
}

char *pwd (Dir* target) {
	char *path = calloc(MAX_INPUT_LINE_SIZE*5,sizeof(char));

	recursive_path(target, path);

	return path;
}

void remove_dir (Dir* current_folder) {
	Dir *dir = current_folder->head_children_dirs;
	int count_dirs = 0;
	char **name_dirs = malloc(MAX_INPUT_LINE_SIZE*sizeof(char*));

	while (dir != NULL) {

		name_dirs[count_dirs] = (char*)malloc(MAX_INPUT_LINE_SIZE+1);

		strcpy(name_dirs[count_dirs], dir->name);	
		dir = dir->next;
		count_dirs++;
	}

	for (int i = 0; i < count_dirs; i++) {

		rmdir(current_folder, name_dirs[i]);
	}

	for (int i = 0; i < count_dirs; i++) {

		free(name_dirs[i]);
	}
	free(name_dirs);
}

void stop (Dir* target) {

	while (strcmp(target->name,"home") != 0) {

		cd(&target, "..");
	}
	
	remove_dir(target);
	remove_files(target);
	free(target);

	return;
}

void printf_files (File* temp_file, int level) {

	while (temp_file != NULL) {

		for (int i = 0; i < level; i++) {
			
			printf("    ");
		}
		printf("%s\n", temp_file->name);
		temp_file = temp_file->next;
	}
}

void tree (Dir* target, int level) {

	if (target->head_children_dirs == NULL && 
		target->head_children_files == NULL) {

		return;
	}

	if (target->head_children_dirs == NULL && 
		target->head_children_files != NULL) {

		File *temp_file;

		temp_file = target->head_children_files;
		printf_files(temp_file, level);

		return;
	}

	if (target->head_children_dirs != NULL) {
		Dir *temp_dir;
		temp_dir = target->head_children_dirs;

		while (temp_dir != NULL) {
			
			for (int i = 0; i < level; i++) {

				printf("    ");
			}
			printf("%s\n", temp_dir->name);
			tree(temp_dir, level + 1);
			temp_dir = temp_dir->next;
		}
	}

	if (target->head_children_files != NULL) {
		File *temp_file;
		temp_file = target->head_children_files;

		printf_files(temp_file, level);
	}
}

void mv_file (Dir* current_folder, char *oldname, char *newname) {

	File *temp_file;
	temp_file = current_folder->head_children_files->next;
	File *prev;
	prev = current_folder->head_children_files;
	File *end;
	end = current_folder->head_children_files;

	while (end->next != NULL) {

		end = end->next;
	}

	while (prev != NULL) {

		if (strcmp(prev->name, oldname) == 0 && prev->next == NULL) {

			free(prev->name);
			prev->name = strdup(newname);
			prev->next = NULL;

			return;
		}
		else if (strcmp(prev->name, oldname) == 0 && 
			strcmp(prev->name, current_folder->head_children_files->name) == 0) {

			current_folder->head_children_files = temp_file;
			free(prev->name);
			prev->name = strdup(newname);
			prev->next = NULL;
			end->next = prev;

			return;
		}
		else if (strcmp(temp_file->name,oldname) == 0) {
			
			free(temp_file->name);
			prev->next = temp_file->next;
			temp_file->name = strdup(newname);
			temp_file->next = NULL;
			end->next = temp_file;
			
			return;
		}
		temp_file = temp_file->next;
		prev = prev->next;
	}
}

void mv_dir (Dir* current_folder, char *oldname, char *newname) {

	Dir *temp_dir;
	temp_dir = current_folder->head_children_dirs->next;
	Dir *prev;
	prev = current_folder->head_children_dirs;
	Dir *end;
	end = current_folder->head_children_dirs;

	while (end->next != NULL) {

		end = end->next;
	}

	while (prev != NULL) {

		if (strcmp(prev->name, oldname) == 0 && prev->next == NULL) {

			free(prev->name);
			prev->name = strdup(newname);
			prev->next = NULL;

			return;
		}
		else if (strcmp(prev->name, oldname) == 0 && 
			strcmp(prev->name, current_folder->head_children_dirs->name) == 0) {

			current_folder->head_children_dirs = temp_dir;
			free(prev->name);
			prev->name = strdup(newname);
			prev->next = NULL;
			end->next = prev;

			return;
		}
		else if (strcmp(temp_dir->name, oldname) == 0) {
			
			free(temp_dir->name);
			prev->next = temp_dir->next;
			temp_dir->name = strdup(newname);
			temp_dir->next = NULL;
			end->next = temp_dir;
			
			return;
		}
		temp_dir = temp_dir->next;
		prev = prev->next;
	}
}

void mv(Dir* parent, char *oldname, char *newname) {

	int error_file_oldname, error_dir_oldname;
	int error_file_newname, error_dir_newname;

	error_file_oldname = check_file_existence(parent, oldname);
	error_dir_oldname = check_dir_existence(parent, oldname);

	error_file_newname = check_file_existence(parent, newname);
	error_dir_newname = check_dir_existence(parent, newname);

	if (error_file_oldname == 0 && error_dir_oldname == 0) {

		printf("File/Director not found\n");
	}
	else if (error_file_newname == 1 || error_dir_newname == 1) {

		printf("File/Director already exists\n");
	}
	else {
		if (error_file_oldname == 1) {

			mv_file(parent, oldname, newname);
		} 
		else if (error_dir_oldname == 1) {

			mv_dir(parent, oldname, newname);
		}
	}
}

int main () {

	Dir *current_folder = (Dir*)malloc(sizeof(Dir));

	current_folder->name = "home";
	current_folder->parent = NULL;
	current_folder->next = NULL;
	current_folder->head_children_files = NULL;
	current_folder->head_children_dirs = NULL;

	do
	{
		char *command = calloc(MAX_INPUT_LINE_SIZE,sizeof(char));

		scanf("%s", command);

		if (strcmp(command,"touch") == 0) {

			free(command);
			char *file_name = calloc(MAX_INPUT_LINE_SIZE,sizeof(char));

			scanf("%s", file_name);
			touch(current_folder, file_name);
			free(file_name);
		}
		else if (strcmp(command,"mkdir") == 0) {

			free(command);
			char *dir_name = calloc(MAX_INPUT_LINE_SIZE,sizeof(char));

			scanf("%s", dir_name);
			mkdir(current_folder, dir_name);
			free(dir_name);
		}
		else if (strcmp(command,"ls") == 0) {

			free(command);
			ls(current_folder);
		}
		else if (strcmp(command,"rm") == 0) {

			free(command);
			char *file_name_rm = calloc(MAX_INPUT_LINE_SIZE,sizeof(char));

			scanf("%s", file_name_rm);
			rm(current_folder, file_name_rm);
			free(file_name_rm);
		}
		else if (strcmp(command,"rmdir") == 0) {

			free(command);
			char *dir_name_rm = calloc(MAX_INPUT_LINE_SIZE,sizeof(char));

			scanf("%s", dir_name_rm);
			rmdir(current_folder, dir_name_rm);
			free(dir_name_rm);
		}
		else if (strcmp(command,"cd") == 0) {

			free(command);
			char *cd_argument = calloc(MAX_INPUT_LINE_SIZE,sizeof(char));

			scanf("%s", cd_argument);
			cd(&current_folder, cd_argument);
			free(cd_argument);
		}
		else if (strcmp(command,"tree") == 0) {

			free(command);
			tree(current_folder, 0);
		}
		else if (strcmp(command,"pwd") == 0) {

			free(command);
			char *path;
			path = pwd(current_folder);

			printf ("%s\n", path);
			free(path);
		}
		else if (strcmp(command,"mv") == 0) {

			free(command);
			char *old_name = calloc(MAX_INPUT_LINE_SIZE,sizeof(char));
			scanf("%s", old_name);
			char *new_name = calloc(MAX_INPUT_LINE_SIZE,sizeof(char));
			scanf("%s", new_name);

			mv(current_folder, old_name, new_name);
			free(old_name);
			free(new_name);
		}
		else if (strcmp(command,"stop") == 0) {

			free(command);
			stop(current_folder);
			break;
		}
		else {

			free(command);
		}
	} while (1);

	return 0;
}