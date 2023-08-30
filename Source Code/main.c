#include <stdio.h>
#include <raylib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <math.h>

#include "src/button.h"
// #include "src/buttons.h"

#define WIN_LEN 400

// #define TESTING

int music_index = 0;
bool paused = false;
bool looping_mode = false;

int main(void) {
    InitWindow(WIN_LEN, WIN_LEN, "LightWeight Music Player");
    InitAudioDevice();

    DIR * directory = opendir(".");
    if (directory == NULL) return 1;

	struct dirent * entry;
	char array[200][255] = { 0 };

	int counter = 0;

    while ((entry = readdir(directory)) != NULL) {
        if (strstr(entry -> d_name, ".mp3")) {
            strcpy(array[counter], entry -> d_name);
            counter++;
        }
    }

    closedir(directory);

	// GRAPHICAL USER INTERFACE

	Music playlist[counter];
	Music current_music;
	float text_x = 10.0f;
	Rectangle bar = {
		.x = 10,
		.y = 330,
		.width = WIN_LEN - 20,
		.height = 10
    };

    for (int i = 0; i < counter; i++) {
        playlist[i] = LoadMusicStream(array[i]);
        playlist[i].looping = true;
        PlayMusicStream(playlist[i]);
    }

	// APP ICON
	Image icon = LoadImage("ICON.png");
    SetWindowIcon(icon);

	// BUTTONS
	Button play, playback, playnext, replay, looping_mode_button;
    init_button(
    	&play,
        (Rectangle){.x = WIN_LEN / 2 - 20, .y = 350, .width = 40, .height = 40},
        GRAY,
        (Color){160, 160, 160, 255},
        (Color){70, 70, 70, 255},
        "=",
        40,
        WHITE
    );

    init_button(
        &playback,
        (Rectangle){.x = (play.rectangle.x - 30) - 5, .y = 355, .width = 30, .height = 30},
        GRAY,
        (Color){160, 160, 160, 255},
        (Color){70, 70, 70, 155},
        "<<",
        30,
        WHITE
    );

    init_button(
        &playnext,
        (Rectangle){.x = play.rectangle.x + play.rectangle.width + 5, .y = 355, .width = 30, .height = 30},
        GRAY,
        (Color){160, 160, 160, 255},
        (Color){70, 70, 70, 155},
        ">>",
        30,
        WHITE
    );

    init_button(
        &replay,
        (Rectangle){.x = playback.rectangle.x - playback.rectangle.width - 5, .y = 355, .width = 30, .height = 30},
        GRAY,
        (Color){160, 160, 160, 255},
        (Color){70, 70, 70, 155},
        "<",
        30,
        WHITE
    );

    init_button(
        &looping_mode_button,
        (Rectangle){.x = playnext.rectangle.x + playnext.rectangle.width + 5, .y = 355, .width = 30, .height = 30},
        GRAY,
        (Color){160, 160, 160, 255},
        (Color){70, 70, 70, 155},
        "LM",
        20,
        WHITE
    );

    // WHILE LOOP

    while (!WindowShouldClose()) {
        current_music = playlist[music_index];
		int music_len = (int) GetMusicTimeLength(current_music);
		int music_play_time = (int) GetMusicTimePlayed(current_music);
        SetWindowState(FLAG_WINDOW_ALWAYS_RUN);
        UpdateMusicStream(current_music);
		int bar_x = music_play_time * (WIN_LEN - 20) / music_len;
		Vector2 mouse_pos = GetMousePosition();
		char LM[4];

        looping_mode ? strcpy(LM, "ON") : strcpy(LM, "OFF");

        if (paused) PauseMusicStream(current_music);
        else ResumeMusicStream(current_music);

        text_x -= 50.0f * GetFrameTime();

        if (text_x + MeasureText(TextFormat("Currently playing: %s", array[abs(music_index % counter)]), 20) < 0)
            text_x = WIN_LEN;

        if (music_play_time == music_len) {
            if (!looping_mode) {
                StopMusicStream(current_music);
                if (music_index < counter - 1) music_index++;
                else music_index = 0;
                PlayMusicStream(current_music);
            }
        }

        #ifdef TESTING
			if (IsKeyPressed(KEY_SPACE)) SeekMusicStream(current_music, music_len - 3);
        #endif

        if (play.is_clicked) {
            paused = !paused;
        }

        if (CheckCollisionPointRec(GetMousePosition(), playback.rectangle)) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                StopMusicStream(current_music);
                if (music_index != 0) music_index--;
                else music_index = counter - 1;
                PlayMusicStream(current_music);
            }
        }

        if (CheckCollisionPointRec(GetMousePosition(), playnext.rectangle)) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                StopMusicStream(current_music);
                if (music_index < counter - 1) music_index++;
                else music_index = 0;
                PlayMusicStream(current_music);
            }
        }

        if (CheckCollisionPointRec(GetMousePosition(), bar)) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				// Calculate the click position as a percentage of the bar's width
				float click_position_percentage = (mouse_pos.x - bar.x) / bar.width;

				// Calculate the new position in the music stream
				float new_position = (float)(click_position_percentage * GetMusicTimeLength(current_music));

                // Set the new position in the music stream
                SeekMusicStream(current_music, (float) new_position);
            }
        }

        if (CheckCollisionPointRec(GetMousePosition(), replay.rectangle)) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                StopMusicStream(current_music);
                PlayMusicStream(current_music);
            }
        }

        if (CheckCollisionPointRec(GetMousePosition(), looping_mode_button.rectangle)) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                looping_mode = !looping_mode;
            }
        }

        BeginDrawing();
        ClearBackground((Color) { 30, 30, 30, 125 });
        // DrawText(TextFormat("(%d,%d)", (int) GetMousePosition().x, (int) GetMousePosition().y), 100, 10, 10, GREEN);
        // DrawText(TextFormat("%d%%", ((int) mouse_pos.x * 100 / (int) bar.width) - (int) bar.x), 5, 5, 10, GREEN);

        if (counter != 0) {
            DrawLine(0, 290, WIN_LEN, 290, GRAY);
            DrawText(TextFormat("Music: %d/%d ", (music_index) + 1, counter), 10, 20, 20, WHITE);
            DrawText(TextFormat("Currently playing: %s", array[music_index]), text_x, 300, 20, WHITE);
            DrawText(TextFormat("%d/%d", music_play_time, music_len), 310, 360, 20, WHITE);
            DrawText(TextFormat("%.2f%%", GetMusicTimePlayed(current_music) * 100 / GetMusicTimeLength(current_music)), 310, 385, 10, WHITE);
            DrawText(TextFormat("LM: %s", LM), 300, 20, 20, WHITE);

            DrawButton(& play);
            DrawButton(& playback);
            DrawButton(& playnext);
            DrawButton(& replay);
            DrawButton(& looping_mode_button);

            DrawRectangleRec(bar, GRAY);
            DrawRectangle(10, 330, bar_x, 10, WHITE);

            if (CheckCollisionPointRec(GetMousePosition(), bar)) {
                DrawText(TextFormat("%d%%", (int) mouse_pos.x * 100 / (int) bar.width - 2), 10, 5, 10, WHITE);
            }

            DrawRectangleLines(10, 50, WIN_LEN - 20, 190, WHITE);
            DrawRectangle(10, 50, WIN_LEN - 20, 25, LIGHTGRAY);
            DrawText(TextFormat(">> %s", array[music_index]), 15, 55, 20, (Color) { 30, 30, 30, 255 });

            for (int i = 1; i < (counter > 8 ? 8 : counter); i++)
            DrawText(TextFormat("%s", array[abs(music_index + i) % counter]), 15, 55 + i * 23, 17, WHITE);
        }
        else {
            DrawText("No music detected in the current directory/folder.", 5, 10, 17, WHITE);
        }

        EndDrawing();
    }

    for (int i = 0; i < counter; i++) UnloadMusicStream(playlist[i]);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
