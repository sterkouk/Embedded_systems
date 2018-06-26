#ifndef LIST   
#define LIST

#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct listnode listnode;

struct listnode{
    char* SourceIP;
    char* DestIP;
    char* SMS;
    struct listnode * next;
};

void push(listnode *head,  char* SourceIP, char* DestIP, char* SMS);
char* remove_by_index(listnode ** head, int n);
char* pop(listnode ** head);
char* concat(const char* s1, const char* s2);


void push(listnode * head, char* SourceIP, char* DestIP, char* SMS) {
    listnode * current = head;
    while (current->next != NULL) 
    {
        current = current->next;
    }


    current->next = malloc(sizeof(listnode));
    current->next->SourceIP = strdup(SourceIP);
    current->next->DestIP = strdup(DestIP);
    current->next->SMS = strdup(SMS);
    current->next->next = NULL;
}

char* remove_by_index(listnode ** head, int n) {
    int i = 0;
    char* retval = NULL;
    listnode * current = *head;
    listnode * temp_node = NULL;

    if (n == 0) {
        return pop(head);
    }

    for (i = 0; i < n-1; i++) {
        if (current->next == NULL) {
            return "-1";
        }
        current = current->next;
    }

    temp_node = current->next;
    retval = temp_node->SMS;
    current->next = temp_node->next;
    free(temp_node);

    return retval;

}

char* pop(listnode ** head) {
    char* retval = NULL;
    listnode * next_node = NULL;

    if (*head == NULL) {
        return "-1";
    }

    next_node = (*head)->next;
    retval = (*head)->SMS;
    free(*head);
    *head = next_node;

    return retval;
}

char* concat(const char* s1, const char* s2)
{
	char* result = malloc(strlen(s1) + strlen(s2) + 1);
	strcpy(result, s1);
	strcat(result, s2);

	return result;
}
#endif
