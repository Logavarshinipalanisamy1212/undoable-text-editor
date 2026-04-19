#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MSG 200
#define MAX_UNDO 5

char message[MAX_MSG] = "";   // current message
int msg_len = 0;

// Stack to store undo states (backup of message)
char undo_stack[MAX_UNDO][MAX_MSG];
int undo_top = -1;

// Helper: push current message state to undo stack
void push_undo() {
    if (undo_top < MAX_UNDO - 1) {
        undo_top++;
        strcpy(undo_stack[undo_top], message);
    } else {
        // Overflow: shift older undo states
        for (int i = 0; i < MAX_UNDO - 1; i++) {
            strcpy(undo_stack[i], undo_stack[i + 1]);
        }
        strcpy(undo_stack[MAX_UNDO - 1], message);
    }
}

// Undo to previous message
void undo() {
    if (undo_top >= 0) {
        strcpy(message, undo_stack[undo_top]);
        undo_top--;
        printf("5‑minute undo applied.\n");
    } else {
        printf("Nothing to undo!\n");
    }
}

// Display current message
void disp_message() {
    printf("Current message: [%s]\n", message);
}

// Initialize: ask user to enter initial message
void init_message() {
    printf("Enter a message: ");
    fgets(message, MAX_MSG, stdin);
    // remove newline if present
    int len = strlen(message);
    if (len > 0 && message[len - 1] == '\n') {
        message[--len] = '\0';
    }
    msg_len = len;
}

int main() {
    int choice;
    char prev[MAX_MSG];

    // 1st time: enter message
    init_message();
    strcpy(prev, message);  // save previous state
    disp_message();
    push_undo();           // save init state

    while (1) {
        printf("\nMenu:\n");
        printf("1. ADD text\n");
        printf("2. UPDATE (replace whole message)\n");
        printf("3. DELETE (clear message)\n");
        printf("4. 5‑minute UNDO\n");
       
        printf("6. EXIT\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        getchar();  // consume newline

        switch (choice) {
            case 1: {
                // SAVE current (previous) text
                strcpy(prev, message);

                // ADD text
                push_undo();
                char add[MAX_MSG - msg_len + 10];
                printf("Enter text to ADD: ");
                fgets(add, sizeof(add), stdin);
                add[strcspn(add, "\n")] = 0;  // strip newline

                // Show: previous + new text
                printf("Before ADD: [%s]\n", prev);
                printf("After ADD:  [%s %s]\n", prev, add);

                strcat(message,  add);
                disp_message();
                break;
            }
            case 2: {
                // UPDATE: replace whole message
                strcpy(prev, message);
                push_undo();
                printf("Enter UPDATED message: ");
                fgets(message, MAX_MSG, stdin);
                message[strcspn(message, "\n")] = 0;

                // Show: previous vs new
                printf("Previous: [%s]\n", prev);
                printf("Updated:  [%s]\n", message);

                disp_message();
                break;
            }
            case 3: {
                // DELETE: clear message
                strcpy(prev, message);
                push_undo();
                message[0] = '\0';
                printf("Deleted full message.\n");
                disp_message();
                break;
            }
            case 4: {
                // 5‑minute UNDO (stack‑based)
                strcpy(prev, message);  // current
                undo();
                printf("Undo from: [%s]\n", prev);
                printf("After undo: [%s]\n", message);
                break;
         
            }
            case 6: {
                // EXIT
                printf("Exiting text editor. Goodbye!\n");
                exit(0);
                break;
            }
            default:
                printf("Invalid choice. Try again.\n");
                break;
        }
    }

    return 0;
}
    