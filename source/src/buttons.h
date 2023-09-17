#ifndef __BUTTONS_H__
#define __BUTTONS_H__

#include <stdio.h>
#include <raylib.h> // Vector2, Color, Rectangle structs, maybe fonts for inner text
#include <stdbool.h>

struct Button {
	Rectangle rec; // Rectangle (Width, Height, Coords (x, y));
	Color color; // Color of the button (RGBA)
	Color color_click; // Color of the button when it is clicked
	Color color_hover; // Color of button when mouse cursor is on top of the button

	bool is_clicked;

	char text[50];
	Color text_color;
	size_t text_font_size;
};

void DrawButton(struct Button* b); 
void InitButton(struct Button* b, int x, int y, size_t width, size_t height, Color color, Color color_click, Color color_hover, char* text, Color text_color, size_t text_font_size);
bool ButtonIsClicked(struct Button *b);


// DEFINITION
void DrawButton(struct Button* b) {

	Color c, h; // Color when clicked and hover

	c = b->color_click;
	h = b->color_hover;


	// Check if cursor is collided with button layer

	if (CheckCollisionPointRec(GetMousePosition(), b->rec)) {
		DrawRectangleRec(b->rec, h); // Drawing the button

		// Check if button is clicked
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			DrawRectangleRec(b->rec, c); // Drawing the button
			b->is_clicked = true;
		}
	}
	else {
		DrawRectangleRec(b->rec, b->color);
		b->is_clicked = false;
	}

	int text_width = MeasureText(b->text, b->text_font_size);
	int text_height = b->text_font_size;
	int pos_x = b->rec.x + (b->rec.width - text_width) / 2;
	int pos_y = b->rec.y + (b->rec.height - text_height) / 2;

	DrawText(b->text, pos_x, pos_y, b->text_font_size, b->text_color);
}

// Initialize button
void InitButton(struct Button* b, int x, int y, size_t width, size_t height, Color color, Color color_click, Color color_hover, char* text, Color text_color, size_t text_font_size) {

	b->rec.width = width;
	b->rec.height = height;

	b->rec.x = x;
	b->rec.y = y;

	b->color = color;
	b->color_click = color_click;
	b->color_hover = color_hover;

	strcpy(b->text, text);
	b->text_color = text_color;
	b->text_font_size = text_font_size;

	b->is_clicked = false;
}

bool ButtonIsClicked(struct Button *b) {
	if (CheckCollisionPointRec(GetMousePosition(), b->rec)) {
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			return true;
		}
		else return false;
	}
	else return false;
}

#endif
