#include <stdio.h>
#include <string.h>

#define RAYLIB_NUKLEAR_IMPLEMENTATION
#include "raylib-nuklear.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main(int argc, char *argv[])
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "CINV");

    // Create the Nuklear Context
    int fontSize = 10;
    struct nk_context *ctx = InitNuklear(fontSize);

    char items[10][256] = {0};
    char buf[256] = "Type some item to be added...";
    int index = 0;

    struct nk_image remove_icon = LoadNuklearImage("../resources/remove.png");
    struct nk_image edit_icon = LoadNuklearImage("../resources/edit.png");
    struct nk_image settings_icon = LoadNuklearImage("../resources/settings.png");
    struct nk_image import_icon = LoadNuklearImage("../resources/import.png");
    struct nk_image export_icon = LoadNuklearImage("../resources/export.png");

    while (!WindowShouldClose())
    {
        // Update the Nuklear context, along with input
        UpdateNuklear(ctx);

        if (nk_begin(ctx, "", nk_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT), 0))
        {
            nk_menubar_begin(ctx);

            nk_layout_row_dynamic(ctx, 0, 8);
            if (nk_menu_begin_image_label(ctx, "Settings", NK_WIDGET_RIGHT, settings_icon, nk_vec2(100, 1000)))
            {
                nk_menu_end(ctx);
            }

            if (nk_menu_begin_image_label(ctx, "Import", NK_WIDGET_RIGHT, import_icon, nk_vec2(100, 0)))
            {
                nk_menu_end(ctx);
            }

            if (nk_menu_begin_image_label(ctx, "Export", NK_WIDGET_RIGHT, export_icon, nk_vec2(100, 0)))
            {
                nk_menu_end(ctx);
            }    
            nk_menubar_end(ctx);

            nk_layout_row_dynamic(ctx, 0, 1);
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
                nk_layout_row_dynamic(ctx, 50, 1);
                if (nk_group_begin(ctx, "item row", NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR))
                {
                    nk_layout_row_dynamic(ctx, 50, 2);
                    nk_label(ctx, items[i], NK_TEXT_ALIGN_LEFT);
                    if (nk_group_begin(ctx, "item options", NK_WINDOW_NO_SCROLLBAR))
                    {
                        nk_layout_row_dynamic(ctx, 0, 10);
                        if (nk_button_image(ctx, edit_icon))
                        {
                            strcpy(items[i], buf);
                        }
                        if (nk_button_image(ctx, remove_icon))
                        {
                            memset(items[i], 0, sizeof(items[i]));
                            for (int j = i; j < 9; j++)
                            {
                                strcpy(items[j], items[j + 1]);
                            }
                            index--;
                        }

                        nk_group_end(ctx);
                    }
                    nk_group_end(ctx);
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