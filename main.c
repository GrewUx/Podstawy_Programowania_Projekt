#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE "reporty_test.txt"

enum post_report_reason
{
    SPAM,
    HEJT,
    WULGARYZMY,
    FEJK_NEWS,
    NIEODPOWIEDNIE_TRESCI
};

enum post_mod_status
{
    DO_WERYFIKACJI,
    W_TRAKCIE_ANALIZY,
    ZATWIERDZONE,
    USUNIETE,
};

typedef struct quick_talk_post_reported
{
    int post_id;
    char post_creator_id[101];
    char post_description[281];
    int post_report_counter;
    enum post_report_reason post_report_reason;
    enum post_mod_status post_mod_status;
    struct quick_talk_post_reported *next;

} quick_talk_post_reported;

const char *get_reason_str(enum post_report_reason nr)
{
    switch (nr)
    {
    case SPAM:
        return "Spam";
    case HEJT:
        return "Hejt";
    case WULGARYZMY:
        return "Wulgaryzmy";
    case FEJK_NEWS:
        return "Fejk-news";
    case NIEODPOWIEDNIE_TRESCI:
        return "Nieodpowiednie tresci";
    default:
        return "Nieznany";
    }
}

const char *get_status_str(enum post_mod_status nr)
{
    switch (nr)
    {
    case DO_WERYFIKACJI:
        return "Do weryfikacji";
    case W_TRAKCIE_ANALIZY:
        return "W trakcie analizy";
    case ZATWIERDZONE:
        return "Zatwierdzone";
    case USUNIETE:
        return "Usuniete";
    default:
        return "Nieznany";
    }
}

quick_talk_post_reported *read_text_file(const char *sciezka)
{
    FILE *plik = fopen(sciezka, "r");
    if (!plik)
    {
        perror("Blad otwarcia pliku!");
        return NULL;
    }

    quick_talk_post_reported *header = NULL;
    quick_talk_post_reported *footer = NULL;
    int id, count, reason_val, status_val;
    char autor[101], tresc[281];

    while (fscanf(plik, "%d|%100[^|]|%280[^|]|%d|%d|%d\n", &id, autor, tresc, &count, &reason_val, &status_val) == 6)
    {
        quick_talk_post_reported *nowy = (quick_talk_post_reported *)malloc(sizeof(quick_talk_post_reported));
        if (!nowy)
        {
            break;
        }

        nowy->post_id = id;
        strcpy(nowy->post_creator_id, autor);
        strcpy(nowy->post_description, tresc);
        nowy->post_report_counter = count;
        nowy->post_report_reason = (enum post_report_reason)reason_val;
        nowy->post_mod_status = (enum post_mod_status)status_val;
        nowy->next = NULL;

        if (header == NULL)
        {
            header = nowy;
        }
        else
        {
            footer->next = nowy;
        }
        footer = nowy;
    }
    fclose(plik);
    return header;
}

void show_list(quick_talk_post_reported *header)
{
    quick_talk_post_reported *current = header;
    printf("\n%-5s | %-15s | %-25s | %-20s | %s\n", "ID", "AUTOR", "POWOD", "STATUS", "TRESC");
    printf("----------------------------------------------------------------------------------------------------------------------\n");

    while (current)
    {
        printf("\n%-5d | %-15s | %-25s | %-20s | %.40s...\n", current->post_id, current->post_creator_id, get_reason_str(current->post_report_reason), get_status_str(current->post_mod_status), current->post_description);
        current = current->next;
    }
}

void free_memory_list(quick_talk_post_reported *header)
{
    while (header)
    {
        quick_talk_post_reported *temp = header;
        header = header->next;
        free(temp);
    }
}

int main()
{
    quick_talk_post_reported *list = read_text_file(INPUT_FILE);

    if (list)
    {
        show_list(list);
        free_memory_list(list);
    }
    else
    {
        printf("blad pliku\n");
    }

    return 0;
}