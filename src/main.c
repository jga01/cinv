#include <stdio.h>
#include <string.h>

#define RAYLIB_NUKLEAR_IMPLEMENTATION
#include "raylib-nuklear.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

int main(int argc, char *argv[])
{
    InitWindow(640, 480, "CINV");

    // Create the Nuklear Context
    int fontSize = 10;
    struct nk_context *ctx = InitNuklear(fontSize);

    char items[10][256] = {0};
    char buf[256] = "Type some item to be added...";
    int index = 0;

    struct nk_image remove_icon = LoadNuklearImage("../resources/remove.png");

    while (!WindowShouldClose())
    {
        // Update the Nuklear context, along with input
        UpdateNuklear(ctx);

        if (nk_begin(ctx, "", nk_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT), 0))
        {
            nk_layout_row_static(ctx, 30, 250, 1);
            nk_label(ctx, "Item:", NK_TEXT_LEFT);
            nk_edit_string_zero_terminated(ctx, NK_EDIT_BOX | NK_EDIT_AUTO_SELECT, buf, sizeof(buf), nk_filter_default);
            if (nk_button_label(ctx, "Add"))
            {
                if (index < 10)
                {
                    strcpy(items[index], buf);
                    index++;
                }
            }

            for (int i = 0; i < index; i++)
            {
                nk_layout_row_static(ctx, 30, 100, 2);
                nk_label(ctx, items[i], NK_TEXT_ALIGN_LEFT);
                if (nk_button_image(ctx, remove_icon))
                {
                    memset(items[i], 0, sizeof(items[i]));
                    for (int j = i; j < 9; j++)
                    {
                        strcpy(items[j], items[j+1]);
                    }
                    index--;
                }
            }
        }
        nk_end(ctx);

        // Render
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Render the Nuklear GUI
        DrawNuklear(ctx);

        EndDrawing();
    }

    // De-initialize the Nuklear GUI
    UnloadNuklear(ctx);

    CloseWindow();
    return 0;
}