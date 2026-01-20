#include "posts.h"

void swap_data(quick_talk_post_reported *a, quick_talk_post_reported *b)
{
    int temp_id = a->post_id;
    a->post_id = b->post_id;
    b->post_id = temp_id;

    int temp_count = a->post_report_counter;
    a->post_report_counter = b->post_report_counter;
    b->post_report_counter = temp_count;

    enum post_report_reason temp_reason = a->post_report_reason;
    a->post_report_reason = b->post_report_reason;
    b->post_report_reason = temp_reason;

    enum post_mod_status temp_status = a->post_mod_status;
    a->post_mod_status = b->post_mod_status;
    b->post_mod_status = temp_status;

    char temp_text[281];
    strcpy(temp_text, a->post_creator_id);
    strcpy(a->post_creator_id, b->post_creator_id);
    strcpy(b->post_creator_id, temp_text);

    strcpy(temp_text, a->post_description);
    strcpy(a->post_description, b->post_description);
    strcpy(b->post_description, temp_text);
}

void sort_by_author(quick_talk_post_reported *header)
{
    if (header == NULL)
    {
        return;
    }
    int swapped;
    quick_talk_post_reported *ptr1;
    quick_talk_post_reported *lptr = NULL;

    do
    {
        swapped = 0;
        ptr1 = header;

        while (ptr1->next != lptr)
        {
            if (strcmp(ptr1->post_creator_id, ptr1->next->post_creator_id) > 0)
            {
                swap_data(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
    printf("Lista zostala posortowana alfabetycznie po autorach.\n");
}

void sort_by_numeric(quick_talk_post_reported *header, int mode)
{
    if (header == NULL)
    {
        return;
    }
    int swapped;
    quick_talk_post_reported *ptr1;
    quick_talk_post_reported *lptr = NULL;

    do
    {
        swapped = 0;
        ptr1 = header;

        while (ptr1->next != lptr)
        {
            int condition = 0;
            if (mode == 1)
            {
                condition = (ptr1->post_id > ptr1->next->post_id);
            }
            else if (mode == 2)
            {
                condition = (ptr1->post_report_counter > ptr1->next->post_report_counter);
            }

            if (condition)
            {
                swap_data(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
    printf("Lista zostala posortowana liczbowo.\n");
}

void search_by_author(quick_talk_post_reported *header)
{
    if (header == NULL)
    {
        printf("Baza jest pusta!\n");
        return;
    }

    char search_term[101];
    printf("Podaj nazwe autora lub jej poczatek: ");
    scanf(" %100[^\n]", search_term);
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
    int len = strlen(search_term);
    int found_count = 0;

    printf("\nWyniki wyszukiwania dla: \"%s\"\n", search_term);
    printf("\n%-5s | %-15s | %-25s | %-15s | %-20s | %s\n", "ID", "AUTOR", "POWOD", "ILOSC ZGLOSZEN", "STATUS", "TRESC");
    printf("----------------------------------------------------------------------------------------------------------------------");

    quick_talk_post_reported *current = header;
    while (current != NULL)
    {
        if (strncmp(current->post_creator_id, search_term, len) == 0)
        {
            printf("\n%-5d | %-15s | %-25s | %-11d zgl.| %-20s | %.40s...\n",
                   current->post_id, current->post_creator_id, get_reason_str(current->post_report_reason), current->post_report_counter, get_status_str(current->post_mod_status), current->post_description);

            found_count++;
        }
        current = current->next;
    }
    if (found_count == 0)
    {
        printf("Nie znaleziono zadnych postow pasujacych do kryteriow. \n");
    }
    else
    {
        printf("\nZnaleziono %d post(ow).\n", found_count);
    }
}

void search_by_reports(quick_talk_post_reported *header)
{
    if (header == NULL)
    {
        printf("Baza jest pusta!\n");
        return;
    }

    int min_reports;
    printf("Wyswietl posty, ktorych ilosc zgloszen wynosi przynajmniej: ");
    if (scanf("%d", &min_reports) != 1)
    {
        printf("Bledna wartosc!\n");
        while (getchar() != '\n')
            ;
        return;
    }

    int found_count = 0;
    printf("\nPosty z liczba zgloszen wieksza lub rowna %d:\n", min_reports);
    printf("\n%-5s | %-15s | %-25s | %-15s | %-20s | %s\n", "ID", "AUTOR", "POWOD", "ILOSC ZGLOSZEN", "STATUS", "TRESC");
    printf("----------------------------------------------------------------------------------------------------------------------");

    quick_talk_post_reported *current = header;
    while (current != NULL)
    {
        if (current->post_report_counter >= min_reports)
        {
            printf("\n%-5d | %-15s | %-25s | %-11d zgl.| %-20s | %.40s...\n",
                   current->post_id, current->post_creator_id, get_reason_str(current->post_report_reason), current->post_report_counter, get_status_str(current->post_mod_status), current->post_description);

            found_count++;
        }
        current = current->next;
    }
    if (found_count == 0)
    {
        printf("Brak postow spelniajacych to kryterium.\n");
    }
}
