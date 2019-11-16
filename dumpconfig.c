#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>

typedef unsigned char byte;

#define SIZEOF_CONFIG 148
struct config {
	byte header[32];
	byte font_name[64];
	int move_btn_mode;
	int attk_btn_mode;
	int okay_btn_mode;
	int display_mode;
	int use_gamepad;
	int gamepad_btns[8];
};

void
read_config(struct config *cfg) {
	int fd;

	fd = open("Config.dat", O_RDONLY);
	if (fd == -1) {
		fprintf(stderr, "Failed to open config file");
		exit(1);
	}

	if (read(fd, (void *) cfg, sizeof(struct config)) == -1) {
		close(fd);
		fprintf(stderr, "read() failed");
		exit(1);
	}

	close(fd);
}

void
dump_config(const struct config *cfg) {
	printf("HEADER: %s\n", (const char *) cfg->header);
	printf("FONT: %s\n", (const char *) cfg->font_name);
	printf("MOVE BTN MODE: %d\n", cfg->move_btn_mode);
	printf("ATTK BTN MODE: %d\n", cfg->attk_btn_mode);
	printf("OKAY BTN MODE: %d\n", cfg->okay_btn_mode);

	printf("DISP MODE: %d (", cfg->display_mode);
	switch (cfg->display_mode) {
	case 0:
		printf("Fullscreen 16 bit)\n");
		break;
	case 1:
		printf("Windowed 320x240)\n");
		break;
	case 2:
		printf("Windowed 640x480)\n");
		break;
	case 3:
		printf("Fullscreen 24 bit)\n");
		break;
	case 4:
		printf("Fullscreen 32 bit)\n");
		break;
	}

	printf("USE GAMEPAD: %d\n", cfg->use_gamepad);
	for (int i = 0; i < 8; i++) {
		printf("Button %d: ", i);
		switch (cfg->gamepad_btns[i]) {
		case 1:
			printf("Jump\n");
			break;
		case 2:
			printf("Attack\n");
			break;
		case 3:
			printf("Weapon +\n");
			break;
		case 4:
			printf("Weapon -\n");
			break;
		case 5:
			printf("Item\n");
			break;
		case 6:
			printf("Map\n");
			break;
		}
	}
}

int
main(int argc, char *argv[]) {
	struct config cfg;

	assert(sizeof(struct config) == SIZEOF_CONFIG);

	memset(&cfg, 0, sizeof(struct config));
	read_config(&cfg);
	dump_config(&cfg);

	return EXIT_SUCCESS;
}
