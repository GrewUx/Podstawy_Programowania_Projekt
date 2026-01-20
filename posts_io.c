#include "posts.h"

quick_talk_post_reported *read_text_file(const char *sciezka)
{
    FILE *plik = fopen(sciezka, "r");
    if (!plik)
    {
        perror("Blad otwarcia pliku!");
        return NULL;
    }

    fseek(plik, 0, SEEK_END);
    if (ftell(plik) == 0)
    {
        printf("Plik '%s' jest pusty.\n", sciezka);
        fclose(plik);
        return NULL;
    }

    rewind(plik);

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

void save_to_text_file(quick_talk_post_reported *header, const char *sciezka)
{
    FILE *plik = fopen(sciezka, "w");
    if (!plik)
    {
        perror("Blad zapisu do pliku!");
        return;
    }
    quick_talk_post_reported *current = header;
    int count = 0;

    while (current != NULL)
    {
        fprintf(plik, "%d|%s|%s|%d|%d|%d\n",
                current->post_id, current->post_creator_id, current->post_description, current->post_report_counter, (int)current->post_report_reason, (int)current->post_mod_status

        );

        current = current->next;
        count++;
    }

    fclose(plik);
    printf("Pomyslnie zapisano %d obiektow do pliku %s\n", count, sciezka);
}
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

void show_list(quick_talk_post_reported *header)
{
    quick_talk_post_reported *current = header;
    printf("\n%-5s | %-15s | %-25s | %-15s | %-20s | %s\n", "ID", "AUTOR", "POWOD", "ILOSC ZGLOSZEN", "STATUS", "TRESC");
    printf("----------------------------------------------------------------------------------------------------------------------");

    while (current)
    {
        printf("\n%-5d | %-15s | %-25s | %-11d zgl.| %-20s | %.40s...\n", current->post_id, current->post_creator_id, get_reason_str(current->post_report_reason), current->post_report_counter, get_status_str(current->post_mod_status), current->post_description);
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
