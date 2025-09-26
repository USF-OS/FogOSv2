#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

#define FLAG_BUF_LEN 12
#define BUF_LEN 128

struct man_flags {
        int h_flag; // display help page flag
        int p_flag; // print in chunks flag
        int d_flag; // delete after printing flag
        int p_flag_buflen;
        int clr_flag;
        int s_flag;
};


// Preprocesses the path, operates on the ptrs themselves.
void preprocess(char* file_name, char* lib_flag){
    // Converts all uppercase chars to lowercase.
    for(char* path = file_name; *path!= '\0'; path++){ // loop until null term
        if(*path >= 65 && *path <= 90){ // if capital char, convert.
            *path += 32;
        }
    }
    char final_path[128] = "man_docs/";
    char* arr_ptr;
    char* txt_ext = ".txt";

    for(arr_ptr = final_path; *arr_ptr != '\0'; arr_ptr++);
    
    memmove(arr_ptr, lib_flag, 3); // copy the flag into the path

    for(arr_ptr = final_path; *arr_ptr != '\0'; arr_ptr++); // move ptr back to end


    // we can use strlen for sizing as these are char (1b) len strings
    memmove(arr_ptr, file_name, strlen(file_name)); // concat the filename to the arr_ptr


    for(arr_ptr = final_path; *arr_ptr != '\0'; arr_ptr++); // move ptr back to end
    memmove(arr_ptr, txt_ext, sizeof(txt_ext)); // append .txt to path

    memmove(file_name, final_path, strlen(final_path)); // overwrite the original path
}

// Checks if man directory exists already in system. If not, will create an
// empty working directory for the man util.
void archive_check(){
    char* options[] = {"man_docs", "man_docs/clu", "man_docs/clib"};
    int check_dir = exec("mkdir", &options[0]);
    if (check_dir < 0){ // if successful, dir did not exist before.
        // create directories.
        printf("No man directory detected... creating directory...\n");
        exec("mkdir", &options[1]); // command-line-utils
        exec("mkdir", &options[2]); // c-library docs
        printf("Done!\n");
    }
}


// returns the power of a given exponent int.
// for example, input 3, 10^3, would be 10*10*10 or 1000.
int pwr_of(int pwr){
    int ret_val = 1;
    for(int i = 0; i < pwr; i++){
        ret_val *= 10;
    }
    return ret_val;
}

// converts a standard c string to an int by reading it char by char.
// does NOT support negatives (who would want to read negative lines of a man
// page???)
int poor_mans_atoi(char* input_str){
    int ret_int = 0;
    if(*input_str < 48 || *input_str > 57){ // if the first char isn't a num, return fail.
        return -1;
    }

    // get length of the str
    int pwr_len = strlen(input_str);
    pwr_len--; // subtract by 1 to compensate for pwr_of

    // convert the str
    for (;*input_str != '\0'; input_str++){
        if (*input_str < 48 || *input_str > 57){ // if out of range of ascii num...
            return -1; // not a valid ascii number (char), fail.
        }
        ret_int += ((int)(*input_str) - 48) * pwr_of(pwr_len);
    }
    return ret_int;
}

// prints out the man doc page with the specified lines per page.
void print_by_chunks(int fd, int lines_per_page, struct man_flags flags) {
    char buf[512]; // read a certain number of characters` at a time
    int n;
    int line_count = 0;
    char c;

    if (flags.d_flag != 1){ // if not debug mode, do normal clearing
        printf("\033c"); // CLEAR THE TERMINAL for printing.
    }


    while ((n = read(fd,buf,512)) > 0){
        for (int i = 0; i < n; i++){
            write(1, &buf[i], 1); // print char in stdout
            if (buf[i] == '\n'){
                line_count++;
                if (line_count >= lines_per_page){ // pause for user
                    printf("Displayed %d lines, press Enter to continue or 'q' to quit: ", lines_per_page);
                    while(read(0, &c, 1) == 1){
                        if (c == '\n'){
                            printf("\033[1A\033[2K\r"); //move back one line and delete, then return
                            break;
                        }
                        if (c == 'q'){
                            printf("\n\033c"); // CLEAR THE TERMINAL
                            return;
                        }
                    } // wait for enter key press
                    
                    if (flags.clr_flag == 1){
                        printf("\n\033c"); // CLEAR THE TERMINAL
                    }
                    line_count = 0; // reset after waiting.
                } 
            }
        }
        if (n < 0){
            printf("encountered error while reading man file.\n");
            exit(1);
        }
    }
    printf("Reached End of Page. Press Enter or 'q' to quit: ");
    while(read(0, &c, 1) == 1){
        if (c == '\n' || c == 'q'){
            printf("\n\033c"); // CLEAR THE TERMINAL
            return;
        }
    } // wait for enter key press
}




int main(int argc, char *argv[]){
    char lib_flag[3] = "0/"; // 0 to indicate empty.

    char* lib_index[9] = {"1/","2/","3/","4/","5/","6/","7/","8/","9/"}; // for reference when -rs flag.

    struct man_flags flags = {0,0,0,0,0,0}; // init all flags to false.
    
    int tmp_filename = 1; // 1 by default

    char* curr_flag;

    
    if(argc < 2){ // Check initial conditions. Send to 'man' page.
        char* help_doc_path = "man_docs/help_man.txt";
        int fd_help = open(help_doc_path, O_RDONLY);
        if (fd_help < 0){ // file not found or error with reading.
            printf("help man file not found or issue with opening.\n");
            exit(1);
        }
        flags.clr_flag = 1;
        print_by_chunks(fd_help, 200, flags); // print out the help doc
        close(fd_help);
        return 1;

    }

    for(int i = 1; i < argc; i++){ // loop to check flags or lib or filename.
        curr_flag = argv[i];
        if(curr_flag[0] == '-'){ // flag
            switch(curr_flag[1]){
                case 'c': // clear after print flag
                    flags.clr_flag = 1;
                    break;

                case 'h': // help
                    flags.h_flag = 1;
                    break;

                case 'p': // char print limit
                    flags.p_flag = 1;
                    char* p_flag_val = argv[i+1];
                    // check next flag val, expect an int (str vers)
                    flags.p_flag_buflen = poor_mans_atoi(p_flag_val); // convert str to int
                    i+=1;
                    break;
                    
                    // .. add more flag conditions here as needed
                    
                case 'd': // debug flag
                    flags.d_flag = 1;
                    printf("DEBUGGING Mode Enabled.\n");
                    break;

                case 's': // for -r, recursive flag cases.
                    flags.s_flag = 1;
                    break;

                default:
                    printf("Unrecognized flag.\n");
                    exit(1);
            }
        }
        else if(curr_flag[0] >= 48 && curr_flag[0] <= 57 
                && *lib_flag == '0' && curr_flag[1] == '\0'){
            // if current char is a 'single' ascii num and no lib_flag has been
            // assigned yet other than 1, assign to curr_flag. Otherwise do nothing.
            lib_flag[0] = curr_flag[0]; // assign current lib num
        }
        else if(curr_flag[0] < 48 || curr_flag[0] >57){
            // if not an int (ascii form) (no file names with numbers please god)
            // mark as the path and and continue.
            tmp_filename = i; // get arg number
            continue;
        }
        else{ // invalid flag, bad.
            // not a valid flag with '-' or a library.. or a path, throw error.
            printf("curr_flag: %s\n", curr_flag);
            printf("Unrecognized or invalid flag or libary.\n");
            exit(1);
        }
    }
 

    char doc_path[BUF_LEN]; 
    // This can be a bit performance intensive, so it's only enabled by flag.
    if (flags.s_flag == 1){ // if recursive search is on, we check ALL directories.
        
        if (flags.d_flag == 1){
            printf("Searching in all directories...\n");
        }
        for (int i = 0; i < 9; i++){ // loop through dirs 1-9
            char tmp_doc_path[BUF_LEN];
            strcpy(tmp_doc_path, argv[tmp_filename]); 
            preprocess(tmp_doc_path, lib_index[i]); // proprocess the path

            int t_fd = open(tmp_doc_path, O_RDONLY);

            if (!(t_fd < 0)){ // if opened successfully... use that as the path.
                strcpy(doc_path, tmp_doc_path); // copy into the actual path.
                break;
            }
            close(t_fd);
        }
    }
    else{ // process path as normally
        strcpy(doc_path, argv[tmp_filename]); // get the doc name from the identified filename
        preprocess(doc_path, lib_flag); // proprocess the path
    }


        
    if (flags.p_flag == 1){ // display docs in chunks
        if (flags.p_flag_buflen == 0 || flags.p_flag_buflen == -1){ // no line size selected, default to 10
            flags.p_flag_buflen = 10;
        }
    }
    else if (flags.h_flag == 1){ // display the actual help page.
        strcpy(doc_path, "man_docs/help.txt"); 
        flags.p_flag_buflen = 200;
    }
    else{ // standard full doc display
        flags.p_flag_buflen = 200; // set to default 200 lines
    }
    

    if (flags.d_flag == 1){ // DEBUG FLAG PRINTOUT
        printf("doc_path: %s\n", doc_path);
    }
    

    int fd = open(doc_path, O_RDONLY); // open and check the file.
    if (fd < 0){ // file not found or error with reading.
        printf("File not found or issue with opening.\n");
        exit(1);
    }
    print_by_chunks(fd, flags.p_flag_buflen, flags);
        
    close(fd); // close the file after reading.
    return 0;
}
