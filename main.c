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

void add_post(quick_talk_post_reported **header)
{
    quick_talk_post_reported *new = (quick_talk_post_reported *)malloc(sizeof(quick_talk_post_reported));
    if (!new)
    {
        printf("Blad alokacji pamieci");
        return;
    }
    int max_id = 0;
    quick_talk_post_reported *temp = *header;
    while (temp)
    {
        if (temp->post_id > max_id)
            max_id = temp->post_id;
        temp = temp->next;
    }
    new->post_id = max_id + 1;
    printf("Podaj nazwe autora (max 100 znakow): ");
    scanf(" %100[^\n]", new->post_creator_id);
    printf("Podaj tresc posta (max 280 znakow): ");
    scanf(" %280[^\n]", new->post_description);
    printf("Wybierz kategorie (0-Spam, 1-Hejt, 2-Wulgaryzmy, 3-Fejk-news, 4-Nieodpowiednie): ");
    int reason_choice;
    scanf("%d", &reason_choice);
    new->post_report_reason = (enum post_report_reason)reason_choice;

    new->post_report_counter = 1;
    new->post_mod_status = DO_WERYFIKACJI;
    new->next = NULL;

    if (*header == NULL)
    {
        *header = new;
    }
    else
    {
        temp = *header;
        while (temp->next)
            temp = temp->next;
        temp->next = new;
    }
    printf("\nDodano post o ID: %d\n", new->post_id);
}

void delete_post(quick_talk_post_reported **header)
{

    if (*header == NULL)
    {
        printf("Baza jest pusta, nie ma czego usunac.\n");
        return;
    }

    int id_to_delete;
    printf("Podaj ID posta do usuniecia: ");
    if (scanf("%d", &id_to_delete) != 1)
    {
        printf("Bledny format ID!\n");
        while (getchar() != '\n')
            ;
        return;
    }

    quick_talk_post_reported *current = *header;
    quick_talk_post_reported *previous = NULL;

    while (current != NULL && current->post_id != id_to_delete)
    {
        previous = current;
        current = current->next;
    }

    if (current == NULL)
    {
        printf("NIe znaleziono posta o ID %d\n", id_to_delete);
        return;
    }

    if (current->post_mod_status == DO_WERYFIKACJI)
    {
        printf("\n[BLOKADA] Nie mozna usunac posta o ID %d\n", id_to_delete);
        printf("Status to 'Do weryfikacji'. Najpierw zmien status na 'Zatwierdzone' lub 'Usuniete'. \n");
        return;
    }
    if (previous == NULL)
    {
        *header = current->next;
    }
    else
    {
        previous->next = current->next;
    }
    free(current);
    printf("Post o ID %d zostal poprawnie usuniety.\n", id_to_delete);
}

void edit_post_status(quick_talk_post_reported *header)
{
    if (header == NULL)
    {
        printf("Baza jest pusta!\n");
        return;
    }
    int id_to_edit;
    printf("Podaj ID posta do edycji statusu: ");
    if (scanf("%d", &id_to_edit) != 1)
    {
        printf("Bledny format ID!\n");
        while (getchar() != '\n')
            ;
        return;
    }
    quick_talk_post_reported *current = header;
    while (current != NULL && current->post_id != id_to_edit)
    {
        current = current->next;
    }
    if (current == NULL)
    {
        printf("Nie znaleziono posta o ID %d.\n", id_to_edit);
        return;
    }
    printf("\nEdytujesz post o ID: %d (Autor: %s)\n", current->post_id, current->post_creator_id);
    printf("Obecny status: %s\n", get_status_str(current->post_mod_status));

    printf("\nWybierz nowy status: \n");
    printf("0. DO WERYFIKACJI\n");
    printf("1. W TRAKCIE ANALIZY\n");
    printf("2. ZATWIERDZONE\n");
    printf("3. USUNIETE\n");
    printf("Wybor: ");

    int new_status;
    if (scanf("%d", &new_status) == 1 && new_status >= 0 && new_status <= 3)
    {
        current->post_mod_status = (enum post_mod_status)new_status;
        printf("Status zostal pomyslnie zmieniony na: %s\n", get_status_str(current->post_mod_status));
    }
    else
    {
        printf("Nieprawidlowy wybor statusu!\n");
        while (getchar() != '\n')
            ;
    }
}

int main()
{
    quick_talk_post_reported *list = read_text_file(INPUT_FILE);
    int choice;

    do
    {
        printf("\n--- MENU QUICKTALK ---\n");
        printf("1. Wyswietl wszystkie posty\n");
        printf("2. Dodaj nowy post\n");
        printf("3. Usun post\n");
        printf("4. Edytuj status posta\n");
        printf("0. Wyjdz i zapisz\n");
        printf("Wybor: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            show_list(list);
            break;
        case 2:
            add_post(&list);
            break;
        case 3:
            delete_post(&list);
            break;
        case 0:
            printf("Zamykanie programu...\n");
            break;
        case 4:
            edit_post_status(list);
            break;
        default:
            printf("Bledny wybor!\n");
        }
    } while (choice != 0);

    free_memory_list(list);
    return 0;
}