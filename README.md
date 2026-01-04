# PP_projekt
10. Moderacyjna Baza Postów Social-Media

Platforma „QuickTalk” to jedno z najpopularniejszych mediów społecznościowych na świecie. Każdego dnia użytkownicy publikują setki tysięcy krótkich wpisów komentujących rzeczywistość, wydarzenia, trendy i memy. Aby utrzymać platformę w bezpiecznym i przyjaznym stanie, działa szybki zespół moderatorów. Do ich pracy potrzebny jest sprawny system katalogowania i oceny postów, które zostały zgłoszone przez innych użytkowników.

Twoim zadaniem jest stworzenie programu umożliwiającego zarządzanie danymi o zgłoszonych postach: ich rejestracją, wyszukiwaniem, modyfikacją, sortowaniem, usuwaniem oraz zapisem/odczytem z pliku. System ułatwia szybkie sortowanie i ocenę treści tak, aby mem o kotach nie trafił na listę „poważne zagrożenie dla demokracji”.

Każdy post posiada minimalny (może być rozszerzony) zestaw cech, które muszą zostać zapisane w systemie:

-numeru ID wpisu - wewnętrzny unikalny identyfikator wszystkich wpisów w systemie (nadawany automatycznie w momencie dodania wpisu);

-nazwa autora - nazwa użytkownika publikującego wpis, o maksymalnej długości 100 znaków. Pole może zawierać pseudonim, nazwę konta lub inny identyfikator;

-treść posta - tekst wpisu, o maksymalnej długości zależnej od projektu (np. do 280 znaków). Treść powinna być przechowywana jako tekst i nie powinna być traktowana jako pole kluczowe;

-kategoria zgłoszenia - powód, dla którego post został zgłoszony: spam, hejt, wulgaryzmy, fejk-news, nieodpowiednie treści. 

-liczba zgłoszeń - liczba określająca, ile razy użytkownicy zgłosili dany post do moderacji;

-status moderacji - aktualny stan wpisu, np.: do weryfikacji, w trakcie analizy, zatwierdzone, usunięte. Posty, które nie zostały jeszcze przeanalizowane przez moderatora, nie mogą zostać usunięte. Najpierw należy zmienić ich status na zatwierdzone lub usunięte w wyniku moderacji. Program powinien zablokować próbę usunięcia i wyświetlić stosowny komunikat.
