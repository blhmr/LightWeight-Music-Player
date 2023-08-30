#ifndef BUTTONS_H
#define BUTTONS_H

#include "button.h"
#include <string.h>

#define WIN_LEN 400
//
//Button play;
//Button playback;
//Button playnext;
//Button replay;
//Button looping_mode_button;

void init_buttons() {}

// BUTTONS
Button play = (Button) {
    .rectangle = (Rectangle) {
        .x = WIN_LEN / 2 - 20,
	    .y = 350,
	    .width = 40,
	    .height = 40
	},
	.color = GRAY,
	.text_color = WHITE,
	.text_font_size = 40,
	.color_hover = (Color) { 160, 160, 160, 255 },
	.color_click = (Color) { 70, 70, 70, 255 },
};
strcpy(play.text, "=");

Button playback = (Button) {
    .rectangle = (Rectangle) {
		.x = (play.rectangle.x - 30) - 5,
		.y = 355,
		.width = 30,
		.height = 30
	},
	.color = GRAY,
	.text_color = WHITE,
	.text_font_size = 30,
	.color_hover = (Color) { 160, 160, 160, 255 },
	.color_click = (Color) { 70, 70, 70, 155 },
};
strcpy(playback.text, "<<");

Button playnext = (Button) {
	.rectangle = (Rectangle) {
		.x = play.rectangle.x + play.rectangle.width + 5,
		.y = 355,
		.width = 30,
		.height = 30
    },
	.color = GRAY,
	.text_color = WHITE,
	.text_font_size = 30,
	.color_hover = (Color) { 160, 160, 160, 255 },
	.color_click = (Color) { 70, 70, 70, 155 },
};
strcpy(playnext.text, ">>");

Button replay = (Button) {
	.rectangle = (Rectangle) {
		.x = playback.rectangle.x - playback.rectangle.width - 5,
		.y = 355,
		.width = 30,
		.height = 30
    },
	.color = GRAY,
	.text_color = WHITE,
	.text_font_size = 30,
	.color_hover = (Color) { 160, 160, 160, 255 },
	.color_click = (Color) { 70, 70, 70, 155 },
};
strcpy(replay.text, "<");

Button looping_mode_button = (Button) {
	.rectangle = (Rectangle) {
		.x = playnext.rectangle.x + playnext.rectangle.width + 5,
		.y = 355,
		.width = 30,
		.height = 30
    },
	.color = GRAY,
	.text_color = WHITE,
	.text_font_size = 20,
	.color_hover = (Color) { 160, 160, 160, 255 },
	.color_click = (Color) { 70, 70, 70, 155 },
};
strcpy(looping_mode_button.text, "LM");

#endif
