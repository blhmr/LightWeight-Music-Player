#include <stdio.h>
#include <raylib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

#include "src/buttons.h"
#include "macros/colors.h"

#define WIN_LEN 400

int main(void) {
    InitWindow(WIN_LEN, WIN_LEN, "LightWeight Music Player");
    InitAudioDevice();

    DIR * directory = opendir(".");
    if (directory == NULL) return 1;
	struct dirent * entry;
	char FileNames[200][255] = { 0 };
	int NumMusic = 0;
	int MusicIndex = 0;
	bool paused = false;
	bool LoopingMode = false;
	Image icon = LoadImage("icon/lwmp.xmp");
	SetWindowIcon(icon);
    
	while ((entry = readdir(directory)) != NULL) {
        if (strstr(entry->d_name, ".mp3")) {
            strcpy(FileNames[NumMusic], entry->d_name);
            NumMusic++;
        }
    }

    closedir(directory);

	// GRAPHICAL USER INTERFACE

	Music playlist[NumMusic];
	Music CurrentMusic;
	float MovingTitleX = 10.0f;
	Rectangle  bar = {10, 330, WIN_LEN - 20, 10};

	if (NumMusic != 0) {
		for (int i = 0; i < NumMusic; i++) {
			playlist[i] = LoadMusicStream(FileNames[i]);
			PlayMusicStream(playlist[i]);
		}
	}

	// BUTTONS
	struct Button play, playback, playnext, replay, lm;
    
	// void InitButton(struct Button* b, int x, int y, size_t width, size_t height, Color color, Color color_click, Color color_hover, char* text, Color text_color, size_t text_font_size) 
	InitButton(&play, WIN_LEN / 2 - 20, 350, 40, 40, (Color) BUTTON_DEFAULT, (Color) BUTTON_CLICK, (Color) BUTTON_HOVER, "P", WHITE, 35);
	InitButton(&playback, play.rec.x - 30 - 5, 355, 30, 30, (Color) BUTTON_DEFAULT, (Color) BUTTON_CLICK, (Color) BUTTON_HOVER, "<", WHITE, 30);
	InitButton(&playnext, play.rec.x + play.rec.width + 5, 355, 30, 30, (Color) BUTTON_DEFAULT, (Color) BUTTON_CLICK, (Color) BUTTON_HOVER, ">", WHITE, 30);
	InitButton(&replay, playback.rec.x - playback.rec.width - 5, 355, 30, 30, (Color) BUTTON_DEFAULT, (Color) BUTTON_CLICK, (Color) BUTTON_HOVER, "R", WHITE, 20);
	InitButton(&lm, playnext.rec.x + playnext.rec.width + 5, 355, 30, 30, (Color) BUTTON_DEFAULT, (Color) BUTTON_CLICK, (Color) BUTTON_HOVER, "LM", WHITE, 20);

    // WHILE LOOP

    while (!WindowShouldClose()) {
        CurrentMusic = playlist[MusicIndex];
		int MusicLength = (int) GetMusicTimeLength(CurrentMusic);
		int MusicPlayInstance = (int) GetMusicTimePlayed(CurrentMusic);
        SetWindowState(FLAG_WINDOW_ALWAYS_RUN);
        UpdateMusicStream(CurrentMusic);
		int bar_x = MusicPlayInstance * (WIN_LEN - 20) / MusicLength;
		Vector2 MousePosition = GetMousePosition();
		char LM[4];
        MovingTitleX -= 50.0f * GetFrameTime();
        LoopingMode ? strcpy(LM, "ON") : strcpy(LM, "OFF");

        if (paused) PauseMusicStream(CurrentMusic);
        else ResumeMusicStream(CurrentMusic);

        if (MovingTitleX + MeasureText(TextFormat("Currently playing: %s", FileNames[abs(MusicIndex % NumMusic)]), 20) < 0)
            MovingTitleX = WIN_LEN;

        if (MusicPlayInstance == MusicLength) {
            if (!LoopingMode) {
                StopMusicStream(CurrentMusic);
                if (MusicIndex < NumMusic - 1) MusicIndex++;
                else MusicIndex = 0;
                PlayMusicStream(CurrentMusic);
            }
        }


		// Buttons functions
		
		if (ButtonIsClicked(&play) || IsKeyPressed(KEY_SPACE)) paused = !paused;

		if (ButtonIsClicked(&playback) || IsKeyPressed(KEY_LEFT)) {
			StopMusicStream(CurrentMusic);
			if (MusicIndex != 0) MusicIndex--;
			else MusicIndex = NumMusic - 1;
			PlayMusicStream(CurrentMusic);
        }

        if (ButtonIsClicked(&playnext) || IsKeyPressed(KEY_RIGHT)) {    
			StopMusicStream(CurrentMusic);
			if (MusicIndex < NumMusic - 1) MusicIndex++;
			else MusicIndex = 0;
			PlayMusicStream(CurrentMusic);
        }

		if (ButtonIsClicked(&replay) || IsKeyPressed(KEY_R)) {
			StopMusicStream(CurrentMusic);
			PlayMusicStream(CurrentMusic);
        }

		if (ButtonIsClicked(&lm) || IsKeyPressed(KEY_L)) LoopingMode = !LoopingMode;

		// Moving Bar function
        if (CheckCollisionPointRec(GetMousePosition(), bar)) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				// Calculate the click position as a percentage of the bar's width
				float ClickPositionPercentage = (MousePosition.x - bar.x) / bar.width;
				// Calculate the new position in the music stream
				float NewPosition = (float)(ClickPositionPercentage * GetMusicTimeLength(CurrentMusic));
                // Set the new position in the music stream
                SeekMusicStream(CurrentMusic, (float) NewPosition);
            }
        }

        BeginDrawing();
        ClearBackground((Color) BG_DARK);

        if (NumMusic != 0) {
            DrawLine(0, 290, WIN_LEN, 290, BG_LIGHT);
            
			DrawText(TextFormat("Music: %d/%d ", (MusicIndex) + 1, NumMusic), 10, 20, 20, WHITE);
            DrawText(TextFormat("Currently playing: %s", FileNames[MusicIndex]), MovingTitleX, 300, 20, WHITE);
            DrawText(TextFormat("%d/%d", MusicPlayInstance, MusicLength), 310, 360, 20, WHITE);
            DrawText(TextFormat("%.2f%%", GetMusicTimePlayed(CurrentMusic) * 100 / GetMusicTimeLength(CurrentMusic)), 310, 385, 10, WHITE);
            DrawText(TextFormat("LM: %s", LM), 300, 20, 20, WHITE);

            DrawButton(&play);
            DrawButton(&playback);
            DrawButton(&playnext);
            DrawButton(&replay);
            DrawButton(&lm);

            DrawRectangleRec(bar, GRAY);
            DrawRectangle(10, 330, bar_x, 10, WHITE);

            if (CheckCollisionPointRec(GetMousePosition(), bar)) {
                DrawText(TextFormat("%d%%", (int) MousePosition.x * 100 / (int) bar.width - 2), 10, 5, 10, WHITE);
            }

            DrawRectangle(10, 50, WIN_LEN - 20, 25, LIGHTGRAY);
            DrawRectangleLines(10, 50, WIN_LEN - 20, 190, WHITE);
            DrawText(TextFormat(">> %s", FileNames[MusicIndex]), 15, 55, 20, (Color) BG_DARK);

            for (int i = 1; i < (NumMusic > 8 ? 8 : NumMusic); i++)
				DrawText(FileNames[abs(MusicIndex + i) % NumMusic], 15, 55 + i * 23, 17, BG_LIGHT);
        }
        else {
            DrawText("No music detected in the current directory/folder.", 5, 10, 17, WHITE);
        }

        EndDrawing();
    }

    for (int i = 0; i < NumMusic; i++) UnloadMusicStream(playlist[i]);
	UnloadImage(icon);
	CloseAudioDevice();
    CloseWindow();
    return 0;
}
