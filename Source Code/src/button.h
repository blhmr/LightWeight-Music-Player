#ifndef BUTTON_H
#define BUTTON_H

#include <raylib.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct Button {
    Rectangle rectangle;
    Sound click;
    Color color, color_hover, color_click;
    bool is_clicked;

    char text[50];
    int text_font_size;
    Color text_color;
} Button;

void  init_button(
	Button* button,
	Rectangle rectangle,
	Color color,
	Color color_hover,
	Color color_click,
	char* text,
	int text_font_size,
	Color text_color
) {
	button->rectangle = rectangle;
	button->color = color;
	button->color_hover = color_hover;
	button->color_click = color_click;
	strcpy(button->text, text);
	button->text_font_size = text_font_size;
	button->text_color = text_color;
}

void DrawButton(Button* button) {
    Color color;
    button->is_clicked = false;

    if (CheckCollisionPointRec(GetMousePosition(), button->rectangle)) {
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            // color = button->color_click;
            PlaySound(button->click);
            button->is_clicked = true;
        }
        else {
            color = button->color_hover;
            button->is_clicked = false;
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) color = button->color_click;
    }
    else color = button->color;

    int textWidth = MeasureText(button->text, button->text_font_size);
    int textHeight = button->text_font_size;
    int posX = button->rectangle.x + (button->rectangle.width - textWidth) / 2;
    int posY = button->rectangle.y + (button->rectangle.height - textHeight) / 2;

    DrawRectangleRec(button->rectangle, color);
    DrawText(button->text, posX, posY, button->text_font_size, WHITE);
}

#endif
