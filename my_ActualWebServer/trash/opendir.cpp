#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

void generate_autoindex(const char *directory_path) {
    printf(R"(
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Index of %s</title>
    </head>
    <body>
        <h1>Index of %s</h1>
        <ul>
    )", directory_path, directory_path);

    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(directory_path)) != NULL) {
        while ((entry = readdir(dir)) != NULL) {
            char *item = entry->d_name;
            char item_path[PATH_MAX];
            snprintf(item_path, sizeof(item_path), "%s/%s", directory_path, item);

            if (entry->d_type == DT_DIR) {
                printf("            <li><a href=\"%s/\">%s/</a></li>\n", item, item);
            } else {
                printf("            <li><a href=\"%s\">%s</a></li>\n", item, item);
            }
        }
        closedir(dir);
    }

    printf(R"(
        </ul>
    </body>
    </html>
    )");
}

int main() {
//    const char *directory_path = "/caminho/do/seu/diretorio";
    const char *directory_path = ".";
    generate_autoindex(directory_path);

    return 0;
}

/*
void generate_autoindex(const char *directory_path) {
    printf(R"(
    <!DOCTYPE html>
    <html lang="pt-BR">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Index of AutoIndex%s</title>
    </head>
    <body>
        <h1>Index of %s</h1>
        <ul>
    )", directory_path, directory_path);

    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(directory_path)) != NULL) {
        while ((entry = readdir(dir)) != NULL) {
            char *item = entry->d_name;
            char item_path[PATH_MAX];
            snprintf(item_path, sizeof(item_path), "%s/%s", directory_path, item);

            if (entry->d_type == DT_DIR) {
                printf("            <li><a href=\"%s/\">%s/</a></li>\n", item, item);
            } else {
                printf("            <li><a href=\"%s\">%s</a></li>\n", item, item);
            }
        }
        closedir(dir);
    }

    printf(R"(
        </ul>
    </body>
    </html>
    )");
}

#include <stdio.h>
int main(int argc, char **argv, char **envp) {
//    const char *directory_path = "/nfs/homes/woliveir/Desktop/42Rio/my_webserv";
    const char *directory_path = ".";
    generate_autoindex(directory_path);
/*
    size_t i = 0;
    while (envp[i])
    {
//        printf("envp[%i]: %s\n", i, envp[i]);
        if (envp[i][0] == 'P' && envp[i][1] == 'W' && envp[i][2] == 'D')
        {
            printf("Achou o pwd em envp[%i]: %s!!!!\n", i, envp[i]);
            generate_autoindex(&envp[i][4]);
            break ;
        }
        i++;
    }
    return 0;
}
*/