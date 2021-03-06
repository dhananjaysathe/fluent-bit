/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*  Fluent Bit
 *  ==========
 *  Copyright (C) 2015-2017 Treasure Data Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <fluent-bit/flb_info.h>

#if !defined(FLB_PARSER_H) && defined(FLB_HAVE_REGEX)
#define FLB_PARSER_H

#include <fluent-bit/flb_regex.h>
#include <fluent-bit/flb_config.h>
#include <fluent-bit/flb_time.h>

#define FLB_PARSER_REGEX 1
#define FLB_PARSER_JSON  2

struct flb_parser {
    /* configuration */
    int type;             /* parser type */
    char *name;           /* format name */
    char *p_regex;        /* pattern for main regular expression */
    char *time_fmt;       /* time format */
    char *time_key;       /* field name that contains the time */
    int time_keep;        /* keep time field */
    char *time_frac_secs; /* time format have fractional seconds ? */

    /* internal */
    int time_with_year; /* do time_fmt consider a year (%Y) ? */
    char *time_fmt_year;

    struct flb_regex *regex;
    struct mk_list _head;
};

static inline time_t flb_parser_tm2time(const struct tm *src)
{
    struct tm tmp;

    tmp = *src;
    return timegm(&tmp) - src->tm_gmtoff;
}


struct flb_parser *flb_parser_create(char *name, char *format,
                                     char *p_regex,
                                     char *time_fmt, char *time_key,
                                     int time_keep, struct flb_config *config);
int flb_parser_conf_file(char *file, struct flb_config *config);
void flb_parser_destroy(struct flb_parser *parser);
struct flb_parser *flb_parser_get(char *name, struct flb_config *config);
int flb_parser_do(struct flb_parser *parser, char *buf, size_t length,
                  void **out_buf, size_t *out_size, struct flb_time *out_time);

void flb_parser_exit(struct flb_config *config);
int flb_parser_frac_tzone(char *str, int len, double *frac, int *tmdiff);

#endif
