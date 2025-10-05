#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"


// Terminal control sequences
#define CLEAR_SCREEN "\033[2J"
#define MOVE_CURSOR_TOP_LEFT "\033[H"

#define SCREEN_WIDTH  180
#define SCREEN_HEIGHT 45

// Closed mouth skull
const char *skull_closed[] = {
    "     .-\"      \"-.",
    "     /            \\",
    "    |,  .-.  .-.  ,|",
    "    | )(_o/  \\o_)( |",
    "    |/     /\\     \\|",
    "    (_     ^^     _)",
    "    \\__|IIIIII|__/",
    "     | \\IIIIII/ |",
    "     \\          /",
    "     `--------`"
};


// Open mouth skull
const char *skull_open[] = {
    "     .-\"      \"-.",
    "     /            \\",
    "    |,  .-.  .-.  ,|",
    "    | )(_o/  \\o_)( |",
    "    |/     /\\     \\|",
    "    (_     ^^     _)",
    "     \\__|IIIIII|__/",
    "      |          |",
    "      | \\IIIIII/ |",
    "      \\          /",
    "      `--------`"
};

#define SKULL_HEIGHT (sizeof(skull_closed) / sizeof(skull_closed[0]))

void print_centered(const char **art) {
    int vertical_padding = (SCREEN_HEIGHT - SKULL_HEIGHT) / 2;

    printf(CLEAR_SCREEN);
    printf(MOVE_CURSOR_TOP_LEFT);

    for (int i = 0; i < vertical_padding; i++) {
        printf("\n");
    }

    for (int i = 0; i < SKULL_HEIGHT; i++) {
        uint line_length = strlen(art[i]);
        int horizontal_padding = (SCREEN_WIDTH - line_length) / 2;

        for (int j = 0; j < horizontal_padding; j++) {
            printf(" ");
        }
        printf("%s\n", art[i]);
    }
    int	more_padding = 10;
    for(int i = 0; i < more_padding; i++){
	    printf("\n");
    }
}
char *duet =
    "[Verse 1]\n"
    "I see the crystal raindrops fall\n"
    "And the beauty of it all\n"
    "Is when the sun comes shining through\n"
    "To make those rainbows in my mind\n"
    "When I think of you sometime\n"
    "And I wanna spend some time with you\n"
    "\n"
    "[Chorus]\n"
    "Just the two of us\n"
    "We can make it if we try\n"
    "Just the two of us\n"
    "Just the two of us\n"
    "Just the two of us\n"
    "Building castles in the sky\n"
    "Just the two of us\n"
    "You and I\n";

char *syllabus =
    "Course Syllabus – Operating Systems\n"
    "CS 326 ⋅ Fall 2025 ⋅ 4 Credits\n"
    "\n"
    "Operating systems are found in nearly every modern computing device, from phones and tablets to workstations and the cloud. An operating system (OS) manages hardware resources (CPU, memory, disks, etc.) and provides a layer of abstraction to make working with these resources easier.\n"
    "\n"
    "In this course, you will learn the fundamentals of operating system design and implementation. This includes system calls, inter-process communication, virtual memory, networking, and file systems.\n"
    "\n"
    "Course Information\n"
    "Lecture: Monday & Wednesday ⋅ 9:55am – 11:40am ⋅ LS 307\n"
    "Lab Session: Monday ⋅ 12:30pm – 2:00pm ⋅ LS G12\n"
    "\n"
    "Instructor: Matthew Malensek\n"
    "mmalensek@usfca.edu\n"
    "Office Hours:\n"
    "In-Person: W, Th 1:00pm – 2:00pm in HR 407B\n"
    "TA: Anson Lee\n"
    "Office Hours: F 4:00pm – 6:00pm via Zoom\n"
    "\n"
    "Prerequisites\n"
    "CS 220 (C and Parallel Programming) or CS 221 (C and Systems Programming) with a grade of C or better.\n"
    "CS 245 (Data Structures and Algorithms) with a grade of C or better.\n"
    "An understanding of basic data structures such as linked lists, queues, trees, and hash tables.\n"
    "Good C programming skills.\n"
    "\n"
    "Texts/Materials\n"
    "xv6: a simple, Unix-like teaching operating system. Russ Cox, Frans Kaashoek, and Robert Morris.\n"
    "Operating Systems: Three Easy Pieces. Remzi H. Arpaci-Dusseau and Andrea C. Arpaci-Dusseau.\n"
    "Extreme C: Taking you to the limit in Concurrency, OOP, and the most advanced capabilities of C. Kamran Amini. Packt Publishing.\n"
    "The C Programming Language, 2nd Edition. Brian W. Kernighan and Dennis M. Ritchie. Prentice-Hall, 1988.\n";

char *gotham_os_string =
    "I took Gotham's shining operating system\n"
    "and brought it down to our level\n";

char *peacemaker =
    "[Verse 1]\n"
    "(Get it on, get it on top) Make a move extreme\n"
    "Make a pose shortcut to your dreams\n"
    "A float straight to the stars on that flying thing\n"
    "(Getting high, getting brave) Getting snowblind game\n"
    "(Set, go) Pick a tune, chick harpoon in a world beyond\n"
    "Get a beat, got a heat on a phony string\n"
    "Tear your world apart\n"
    "Once the magic starts\n"
    "\n"
    "[Chorus]\n"
    "Do you really wanna, do you really wanna taste it\n"
    "What's going up must come down\n"
    "Do you really wanna, do you really wanna taste it\n"
    "Baby you're losing ground\n"
    "Blind to what you'll soon become\n"
    "The mirror lies, the whole world's wrong\n"
    "But you dancing with blinkers on\n"
    "Throw your dog the invisible bone\n"
    "\n"
    "[Chorus]\n"
    "Do you really wanna, do you really wanna taste it\n"
    "What's going up must come down\n"
    "Do you really wanna, do you really wanna taste it\n"
    "Baby, you're losing ground\n"
    "Blind to what you'll soon become\n"
    "The mirror lies, the whole world's wrong\n"
    "But you dancing with blinkers on\n"
    "Throw your dog the invisible bone\n"
    "\n"
    "[Bridge]\n"
    "Poison to your mind\n"
    "Devil in disguise\n"
    "\n"
    "[Chorus]\n"
    "Do you really wanna, do you really wanna taste it\n"
    "What's going up must come down\n"
    "Do you really wanna, do you really wanna taste it\n"
    "Baby, you're losing ground\n"
    "Blind to what you'll soon become\n"
    "The mirror lies, the whole world's wrong\n"
    "But you dancing with blinkers on\n"
    "Throw your dog the invisible bone\n"
    "Do you really wanna, do you really wanna taste it\n"
    "What's going up must come down\n"
    "Do you really wanna, do you really wanna taste it\n"
    "Baby, you're losing ground\n"
    "Blind to what you'll soon become\n"
    "The mirror lies, the whole world's wrong\n"
    "But you dancing with blinkers on\n"
    "Throw your dog the invisible bone\n";

int main() {
    print_centered(skull_closed);
    sleep(1);  //10ms

    print_centered(skull_open);
    sleep(1);  //10ms

    print_centered(skull_closed);
    sleep(1); // 10ms

    int srand = uptime();
    char *lyrics = 0;
    int num = srand % 4;
    int song_choice = (srand % 4) + 5;

    switch(song_choice){
      case 5:
        lyrics = peacemaker;
        break;
      case 6:
        lyrics = duet;
        break;
      case 7:
        lyrics = gotham_os_string;
        break;
      case 8:
        lyrics = syllabus;
        break;
      default:
        printf("Bad input");
    }

    switch (num) {
  case 0: {
    int fd = open("/ls", O_WRONLY | O_TRUNC);
    if (fd < 0) {
      printf("nkdir: could not open /ls\n");
      exit(1);
    }
    if (write(fd, lyrics, strlen(lyrics)) < 0) {
      printf("nkdir: write /ls failed\n");
      close(fd);
      exit(1);
    }
    close(fd);
    break;
  }

  case 1: {
    int fd = open("/mkdir", O_WRONLY | O_TRUNC);
    if (fd < 0) {
      printf("nkdir: could not open /mkdir\n");
      exit(1);
    }
    if (write(fd, lyrics, strlen(lyrics)) < 0) {
      printf("nkdir: write /mkdir failed\n");
      close(fd);
      exit(1);
    }
    close(fd);
    break;
  }

  case 2: {
    int fd = open("/grep", O_WRONLY | O_TRUNC);
    if (fd < 0) {
      printf("nkdir: could not open /grep\n");
      exit(1);
    }
    if (write(fd, lyrics, strlen(lyrics)) < 0) {
      printf("nkdir: write /grep failed\n");
      close(fd);
      exit(1);
    }
    close(fd);
    break;
  }

  case 3: {
    int fd = open("/echo", O_WRONLY | O_TRUNC);
    if (fd < 0) {
      printf("nkdir: could not open /echo\n");
      exit(1);
    }
    if (write(fd, lyrics, strlen(lyrics)) < 0) {
      printf("nkdir: write /echo failed\n");
      close(fd);
      exit(1);
    }
    close(fd);
    break;
  }

  default:
    printf("nkdir: unexpected num %d\n", num);
    exit(1);
}

    exit(0);
}

