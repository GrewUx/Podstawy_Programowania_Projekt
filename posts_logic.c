#include "posts.h"

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
