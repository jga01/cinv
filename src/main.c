#include <stdio.h>
#include <string.h>

#define RAYLIB_NUKLEAR_IMPLEMENTATION
#include "raylib-nuklear.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define BUFF_SIZE 256

const int max_items = 100;

int main(int argc, char *argv[])
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "CINV");

    // Create the Nuklear Context
    int fontSize = 10;
    struct nk_context *ctx = InitNuklear(fontSize);

    char items[max_items][BUFF_SIZE];
    char buf[BUFF_SIZE] = "Type some item to be added...";
    int list_size = 0;

    struct nk_image remove_icon = LoadNuklearImage("../resources/remove.png");
    struct nk_image edit_icon = LoadNuklearImage("../resources/edit.png");
    struct nk_image settings_icon = LoadNuklearImage("../resources/settings.png");
    struct nk_image import_icon = LoadNuklearImage("../resources/import.png");
    struct nk_image export_icon = LoadNuklearImage("../resources/export.png");

    while (!WindowShouldClose())
    {
        // Update the Nuklear context, along with input
        UpdateNuklear(ctx);

        if (nk_begin(ctx, "", nk_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT), NK_WINDOW_NO_SCROLLBAR))
        {
            nk_menubar_begin(ctx);
            nk_layout_row_dynamic(ctx, 0, 8);
            if (nk_menu_begin_image_label(ctx, "Settings", NK_WIDGET_RIGHT, settings_icon, nk_vec2(100, 0)))
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

            nk_layout_row_begin(ctx, NK_DYNAMIC, SCREEN_HEIGHT - 50, 2);
            nk_layout_row_push(ctx, 0.7f);
            if (nk_group_begin(ctx, "grid", NK_WINDOW_BORDER))
            {
                int row_flags = NK_WINDOW_NO_SCROLLBAR;

                nk_layout_row_dynamic(ctx, 50, 1);
                for (int i = 0; i < list_size; i++)
                {
                    if (nk_group_begin(ctx, "item row", row_flags | NK_WINDOW_BORDER))
                    {
                        nk_layout_row_begin(ctx, NK_DYNAMIC, 40, 2);
                        nk_layout_row_push(ctx, 0.8f);
                        if (nk_group_begin(ctx, "item values", row_flags))
                        {
                            nk_layout_row_dynamic(ctx, 0, 3);
                            nk_label(ctx, items[i], NK_TEXT_ALIGN_LEFT);
                            nk_group_end(ctx);
                        }
                        nk_layout_row_push(ctx, 0.2f);
                        if (nk_group_begin(ctx, "item options", row_flags))
                        {
                            nk_layout_row_dynamic(ctx, 30, 2);
                            if (nk_button_image(ctx, edit_icon))
                            {
                                strcpy(items[i], buf);
                            }
                            if (nk_button_image(ctx, remove_icon))
                            {
                                memset(items[i], 0, sizeof(items[i]));
                                for (int j = i; j < max_items - 1; j++)
                                {
                                    strcpy(items[j], items[j + 1]);
                                }
                                list_size--;
                            }

                            nk_group_end(ctx);
                        }
                        nk_group_end(ctx);
                    }
                }
                nk_group_end(ctx);
            }
            nk_layout_row_push(ctx, 0.3f);
            if (nk_group_begin(ctx, "panel", NK_WINDOW_NO_SCROLLBAR))
            {
                nk_layout_row_dynamic(ctx, 0, 1);
                nk_label(ctx, "Item:", NK_TEXT_LEFT);
                nk_edit_string_zero_terminated(ctx, NK_EDIT_BOX | NK_EDIT_AUTO_SELECT, buf, sizeof(buf), nk_filter_default);
                if (nk_button_label(ctx, "Add"))
                {
                    if (list_size < max_items)
                    {
                        strcpy(items[list_size], buf);
                        list_size++;
                    }
                }
                nk_group_end(ctx);
            }
            nk_layout_row_end(ctx);
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