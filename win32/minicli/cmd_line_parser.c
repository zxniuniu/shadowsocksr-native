//
// Created by ssrlive on 18-4-22.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "cmd_line_parser.h"

const char *server_port = "443";
const char *DFL_REQUEST_PAGE = "/";

static void object_safe_free(void **obj) {
    if (obj && *obj) {
        free(*obj);
        *obj = NULL;
    }
}

static void string_safe_assign(char **target, const char *value) {
    object_safe_free((void **)target);
    if (target && value) {
        *target = strdup(value);
    }
}

struct cmd_line_info * cmd_line_info_create(int argc, char * const argv[]) {
    int opt;

    struct cmd_line_info *info = (struct cmd_line_info *)calloc(1, sizeof(*info));

    while (-1 != (opt = getopt(argc, argv, "s:p:r:c:d:h"))) {
        switch (opt) {
        case 's':
            string_safe_assign(&info->server_addr, optarg);
            break;
        case 'p':
            string_safe_assign(&info->server_port, optarg);
            break;
        case 'r':
            string_safe_assign(&info->request_page, optarg);
            break;
        case 'c':
            string_safe_assign(&info->root_cert_file, optarg);
            break;
        case 'd':
            info->dump_level = atoi(optarg);
            break;
        case 'h':
        default:
            info->help_flag = true;
            break;
        }
    }

    if (info->server_port == 0) {
        string_safe_assign(&info->server_port, server_port);
    }
    if (info->request_page == NULL) {
        string_safe_assign(&info->request_page, DFL_REQUEST_PAGE);
    }

    return info;
}

void cmd_line_info_destroy(struct cmd_line_info *info) {
    if (info) {
        object_safe_free((void **)&info->root_cert_file);
        object_safe_free((void **)&info->server_addr);
        object_safe_free((void **)&info->server_port);
        object_safe_free((void **)&info->request_page);
        free(info);
    }
}

const char * app_name(const char *app_path) {
    const char *p = strrchr(app_path, '/');
    if (p == NULL) {
        p = strrchr(app_path, '\\');
    }
    if (p) {
        ++p;
    } else {
        p = app_path;
    }
    return p;
}

int usage(int argc, char * const argv[]) {
    printf("tls mini client\n"
        "\n"
        "Usage:\n"
        "\n"
        "  %s [-s <server address>] [-p <server port>] [-r request_page] [-c <certificate file>] [-h]\n"
        "\n"
        "Options:\n"
        "\n"
        "  -s <server address>    Remote server address/hostname.\n"
        "  -p <server port>       Remote server port number.\n"
        "  -r <request page>      Request page path.\n"
        "  -c <certificate file>  Root certificate file path.\n"
        "  -d <dump level>        Dump level, default is 0.\n"
        "  -h                     Show this help message.\n"
        "",
        app_name(argv[0]));
    return 0;
}
