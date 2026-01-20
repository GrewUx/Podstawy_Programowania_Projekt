#ifndef POSTS_H
#define POSTS_H

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
// posts_io.c
quick_talk_post_reported *read_text_file(const char *sciezka);
void save_to_text_file(quick_talk_post_reported *header, const char *sciezka);
void show_list(quick_talk_post_reported *header);
void free_memory_list(quick_talk_post_reported *header);
const char *get_reason_str(enum post_report_reason nr);
const char *get_status_str(enum post_mod_status nr);

// posts_logic.c
void add_post(quick_talk_post_reported **header);
void delete_post(quick_talk_post_reported **header);
void edit_post_status(quick_talk_post_reported *header);
void delete_multiple_by_category(quick_talk_post_reported **header);

// posts_tools.c
void search_by_author(quick_talk_post_reported *header);
void search_by_reports(quick_talk_post_reported *header);
void swap_data(quick_talk_post_reported *a, quick_talk_post_reported *b);
void sort_by_author(quick_talk_post_reported *header);
void sort_by_numeric(quick_talk_post_reported *header, int mode);

#endif