#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define MAX_CONTAINERS 100

typedef struct {
    char id[32];
    pid_t pid;
    int active;
} container_t;

container_t containers[MAX_CONTAINERS];

/* ================= LOAD FROM FILE ================= */
void load_containers() {
    FILE *f = fopen("containers.txt", "r");
    if (!f) return;

    int i = 0;
    while (fscanf(f, "%s %d", containers[i].id, &containers[i].pid) == 2) {
        containers[i].active = 1;
        i++;
    }
    fclose(f);
}

/* ================= SAVE TO FILE ================= */
void save_container(const char *id, pid_t pid) {
    FILE *f = fopen("containers.txt", "a");
    if (!f) return;

    fprintf(f, "%s %d\n", id, pid);
    fclose(f);
}

/* ================= START ================= */
int cmd_start(int argc, char *argv[]) {
    if (argc < 5) {
        printf("Usage: start <id> <rootfs> <command>\n");
        return 1;
    }

    char *id = argv[2];
    char *cmd = argv[4];

    pid_t pid = fork();

    if (pid == 0) {
        execlp(cmd, cmd, NULL);
        perror("exec failed");
        exit(1);
    }

    printf("Container started PID: %d\n", pid);

    save_container(id, pid);
    return 0;
}

/* ================= PS ================= */
int cmd_ps() {
    load_containers();

    printf("Running containers:\n");

    for (int i = 0; i < MAX_CONTAINERS; i++) {
        if (containers[i].active) {
            printf("ID: %s | PID: %d\n",
                   containers[i].id,
                   containers[i].pid);
        }
    }

    return 0;
}

/* ================= STOP ================= */
int cmd_stop(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: stop <id>\n");
        return 1;
    }

    char *target = argv[2];

    printf("Stopping container: %s\n", target);

    FILE *f = fopen("containers.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!f || !temp) return 1;

    char id[32];
    int pid;

    while (fscanf(f, "%s %d", id, &pid) == 2) {

        if (strcmp(id, target) == 0) {
            kill(pid, SIGKILL);
        } else {
            fprintf(temp, "%s %d\n", id, pid);
        }
    }

    fclose(f);
    fclose(temp);

    remove("containers.txt");
    rename("temp.txt", "containers.txt");

    return 0;
}

/* ================= MAIN ================= */
int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Usage: ./engine <command>\n");
        return 1;
    }

    if (strcmp(argv[1], "start") == 0)
        return cmd_start(argc, argv);

    if (strcmp(argv[1], "ps") == 0)
        return cmd_ps();

    if (strcmp(argv[1], "stop") == 0)
        return cmd_stop(argc, argv);

    printf("Unknown command\n");
    return 1;
}
