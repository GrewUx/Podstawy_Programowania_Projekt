#include "posts.h"

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