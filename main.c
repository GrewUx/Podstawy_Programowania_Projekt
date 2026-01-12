#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
    printf("Podaj tresc posta (max 280 znakow): ");
    scanf(" %280[^\n]", new->post_description);
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
    printf("Wybierz kategorie (0-Spam, 1-Hejt, 2-Wulgaryzmy, 3-Fejk-news, 4-Nieodpowiednie): ");
    int reason_choice;
    if (scanf("%d", &reason_choice) != 1)
    {
        printf("Kategoria spoza zakresu!\n");
        while (getchar() != '\n')
            ;
        return;
    }
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

void delete_multiple_by_category(quick_talk_post_reported **header)
{
    if (*header == NULL)
    {
        printf("Baza jest pusta!");
        return;
    }
    printf("\nWybierz kategorie do masowego usuniecia: \n");
    printf("0-Spam, 1-Hejt, 2-Wulgaryzmy, 3-Fejk-news, 4-Nieodpowiednie\nWybor: ");

    int cat_choice;
    if (scanf("%d", &cat_choice) != 1 || cat_choice < 0 || cat_choice > 4)
    {
        printf("Nieprawidlowy wybor!");
        while (getchar() != '\n')
            ;
        return;
    }

    quick_talk_post_reported *current = *header;
    quick_talk_post_reported *previous = NULL;
    int deleted_count = 0;
    int blocked_count = 0;

    while (current != NULL)
    {
        if ((int)current->post_report_reason == cat_choice)
        {
            if (current->post_mod_status == DO_WERYFIKACJI)
            {
                blocked_count++;
                previous = current;
                current = current->next;
            }
            else
            {
                quick_talk_post_reported *temp = current;
                if (previous == NULL)
                {
                    *header = current->next;
                    current = *header;
                }
                else
                {
                    previous->next = current->next;
                    current = previous->next;
                }

                free(temp);
                deleted_count++;
            }
        }
        else
        {
            previous = current;
            current = current->next;
        }
    }
    printf("\n--- RAPORT USUNIECIA---\n");
    printf("Usunieto poprawnie %d postow.\n", deleted_count);
    if (blocked_count > 0)
    {
        printf("Zablokowano usuniecie %d postow (status 'Do weryfikacji').\n", blocked_count);
    }
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

int main(int argc, char *argv[])
{
    if (argc < 2)
    {

        printf("Blad: Nie podano nazwy pliku wejsciowego!\n");
        printf("Uzycie: %s <nazwa_pliku.txt>\n", argv[0]);
        return 1;
    }
    char *name_of_file = argv[1];
    quick_talk_post_reported *list = read_text_file(name_of_file);

    if (!list)
    {
        printf("Nie udalo sie wczytac danych z pliku: %s\n", name_of_file);
        return 1;
    }

    int choice;

    do
    {
        printf("\n--- MENU QUICKTALK ---\n");
        printf("1. Wyswietl wszystkie posty\n");
        printf("2. Dodaj nowy post\n");
        printf("3. Usun wybrany post\n");
        printf("4. Edytuj status posta\n");
        printf("5. Wyszukaj posty\n");
        printf("6. Sortowanie\n");
        printf("7. Usun posty masowo\n");
        printf("0. Zapisz i wyjdz z programu\n");
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
            printf("Zapisywanie danych...\n");
            save_to_text_file(list, name_of_file);
            printf("Zamykanie programu...\n");
            break;
        case 4:
            edit_post_status(list);
            break;
        case 5:
            printf("\nWybierz kryterium:\n1. Szukaj autora (tekstowe)\n2. Szukaj po liczbie zgloszen (liczbowe)\nWybor: ");
            int search_choice;
            if (scanf("%d", &search_choice) != 1)
            {
                printf("Bledna wartosc!\n");
                while (getchar() != '\n')
                    ;
            }
            if (search_choice == 1)
            {
                search_by_author(list);
            }
            else if (search_choice == 2)
            {
                search_by_reports(list);
            }
            break;
        case 6:
            printf("\nPo jakiej kolumnie sortowac?\n");
            printf("1. Autor\n");
            printf("2. ID\n");
            printf("3. Liczba zgloszen\n");
            printf("Wybor: ");
            int s_choice;
            if (scanf("%d", &s_choice) != 1)
            {
                printf("Bledna wartosc!\n");
                while (getchar() != '\n')
                    ;
            }
            if (s_choice == 1)
            {
                sort_by_author(list);
            }
            else if (s_choice == 2)
            {
                sort_by_numeric(list, 1);
            }
            else if (s_choice == 3)
            {
                sort_by_numeric(list, 2);
            }
            else
            {
                printf("Niepoprawny wybor.\n");
            }
            break;
        case 7:
            delete_multiple_by_category(&list);
            break;
        default:
            printf("Bledny wybor!\n");
        }
    } while (choice != 0);

    free_memory_list(list);
    return 0;
}