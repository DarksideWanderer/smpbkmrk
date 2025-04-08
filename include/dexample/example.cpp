#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <locale.h>

#define MAX_LINES 50
#define LINE_LENGTH 256
#define LINE_HEIGHT 30
#define TEXTBOX_PADDING 5

// 设置UTF-8环境
void initUTF8Environment() {
    setlocale(LC_ALL, "");
}

// UTF-8辅助函数
int utf8_char_len(const char* s) {
    unsigned char c = (unsigned char)*s;
    if (c < 0x80) return 1;
    if ((c & 0xE0) == 0xC0) return 2;
    if ((c & 0xF0) == 0xE0) return 3;
    if ((c & 0xF8) == 0xF0) return 4;
    return 1;
}

int utf8_strlen(const char* s) {
    int count = 0;
    while (*s) {
        int len = utf8_char_len(s);
        s += len;
        count++;
    }
    return count;
}

const char* utf8_index(const char* s, int n) {
    for (int i = 0; i < n && *s; i++) {
        s += utf8_char_len(s);
    }
    return s;
}

typedef struct {
    SDL_Rect rect;
    char lines[MAX_LINES][LINE_LENGTH];
    int line_count;
    int cursor_row, cursor_col;
    int select_start_row, select_start_col;
    int select_end_row, select_end_col;
    bool has_selection;
    bool is_focused;
    Uint32 last_cursor_blink;
    bool cursor_visible;
    int scroll_y;
    TTF_Font* font;
    SDL_Color text_color;
    SDL_Color bg_color;
    SDL_Color border_color;
    SDL_Color select_color;
} AdvancedTextBox;

void initAdvancedTextBox(AdvancedTextBox* box, int x, int y, int w, int h, TTF_Font* font) {
    box->rect.x = x;
    box->rect.y = y;
    box->rect.w = w;
    box->rect.h = h;
    
    box->font = font;
    box->line_count = 1;
    box->lines[0][0] = '\0';
    
    box->cursor_row = box->cursor_col = 0;
    box->select_start_row = box->select_start_col = 0;
    box->select_end_row = box->select_end_col = 0;
    box->has_selection = false;
    box->is_focused = false;
    box->scroll_y = 0;
    box->cursor_visible = true;
    box->last_cursor_blink = SDL_GetTicks();
    
    box->text_color = (SDL_Color){0, 0, 0, 255};
    box->bg_color = (SDL_Color){255, 255, 255, 255};
    box->border_color = (SDL_Color){150, 150, 150, 255};
    box->select_color = (SDL_Color){200, 200, 255, 255};
}

char* getSelectedText(AdvancedTextBox* box) {
    if (!box->has_selection) return NULL;

    int start_row = (box->select_start_row < box->select_end_row || 
                   (box->select_start_row == box->select_end_row && 
                    box->select_start_col <= box->select_end_col)) 
                   ? box->select_start_row : box->select_end_row;
    int start_col = (start_row == box->select_start_row) ? box->select_start_col : box->select_end_col;
    int end_row = (start_row == box->select_start_row) ? box->select_end_row : box->select_start_row;
    int end_col = (end_row == box->select_end_row) ? box->select_end_col : box->select_start_col;

    size_t length = 0;
    for (int row = start_row; row <= end_row; row++) {
        const char* line = box->lines[row];
        int line_len = strlen(line);
        int copy_start = (row == start_row) ? start_col : 0;
        int copy_end = (row == end_row) ? end_col : line_len;
        
        length += (copy_end > copy_start) ? (copy_end - copy_start) : 0;
        if (row < end_row) length += 1;
    }

    char* selected = (char*)malloc(length + 1);
    if (!selected) return NULL;
    
    char* ptr = selected;
    for (int row = start_row; row <= end_row; row++) {
        const char* line = box->lines[row];
        int line_len = strlen(line);
        int copy_start = (row == start_row) ? start_col : 0;
        int copy_end = (row == end_row) ? end_col : line_len;
        
        if (copy_end > copy_start) {
            int copy_len = copy_end - copy_start;
            memcpy(ptr, line + copy_start, copy_len);
            ptr += copy_len;
        }
        
        if (row < end_row) {
            *ptr++ = '\n';
        }
    }
    *ptr = '\0';
    return selected;
}

void deleteSelectedText(AdvancedTextBox* box) {
    if (!box->has_selection) return;

    int start_row = (box->select_start_row < box->select_end_row || 
                   (box->select_start_row == box->select_end_row && 
                    box->select_start_col <= box->select_end_col)) 
                   ? box->select_start_row : box->select_end_row;
    int start_col = (start_row == box->select_start_row) ? box->select_start_col : box->select_end_col;
    int end_row = (start_row == box->select_start_row) ? box->select_end_row : box->select_start_row;
    int end_col = (end_row == box->select_end_row) ? box->select_end_col : box->select_start_col;

    if (start_row == end_row) {
        char* line = box->lines[start_row];
        memmove(line + start_col, line + end_col, strlen(line + end_col) + 1);
        box->cursor_row = start_row;
        box->cursor_col = start_col;
    } else {
        char* first_line = box->lines[start_row];
        char* last_line = box->lines[end_row];
        
        if ((int)(strlen(first_line) + strlen(last_line + end_col)) < LINE_LENGTH) {
            first_line[start_col] = '\0';
            strcat(first_line, last_line + end_col);
            
            int lines_to_remove = end_row - start_row;
            for (int i = start_row + 1; i + lines_to_remove < box->line_count; i++) {
                strcpy(box->lines[i], box->lines[i + lines_to_remove]);
            }
            
            box->line_count -= lines_to_remove;
            box->cursor_row = start_row;
            box->cursor_col = start_col;
        }
    }
    
    box->has_selection = false;
}

void insertTextAtCursor(AdvancedTextBox* box, const char* text) {
    if (!text || !*text) return;

    if (box->has_selection) {
        deleteSelectedText(box);
    }

    const char* ptr = text;
    int current_row = box->cursor_row;
    int current_col = box->cursor_col;
    
    while (*ptr) {
        if (*ptr == '\n') {
            if (box->line_count >= MAX_LINES) break;
            
            char remaining[LINE_LENGTH];
            strcpy(remaining, box->lines[current_row] + current_col);
            box->lines[current_row][current_col] = '\0';
            
            for (int i = box->line_count; i > current_row + 1; i--) {
                strcpy(box->lines[i], box->lines[i-1]);
            }
            
            strcpy(box->lines[current_row + 1], remaining);
            box->line_count++;
            
            current_row++;
            current_col = 0;
            ptr++;
        } else {
            char* line = box->lines[current_row];
            int line_len = strlen(line);
            int char_len = utf8_char_len(ptr);
            
            if (line_len + char_len < LINE_LENGTH) {
                memmove(line + current_col + char_len, line + current_col, line_len - current_col + 1);
                memcpy(line + current_col, ptr, char_len);
                current_col += char_len;
                ptr += char_len;
            } else {
                break;
            }
        }
    }
    
    box->cursor_row = current_row;
    box->cursor_col = current_col;
}

void handleTextBoxEvent(AdvancedTextBox* box, SDL_Event* event) {
    if (!box->is_focused) return;

    switch (event->type) {
        case SDL_MOUSEBUTTONDOWN: {
            if (event->button.button == SDL_BUTTON_LEFT) {
                int click_x = event->button.x - box->rect.x;
                int click_y = event->button.y - box->rect.y + box->scroll_y;
                
                int row = click_y / LINE_HEIGHT;
                if (row >= 0 && row < box->line_count) {
                    box->cursor_row = row;
                    
                    const char* line = box->lines[row];
                    int col = 0;
                    int x_pos = TEXTBOX_PADDING;
                    const char* ptr = line;
                    
                    while (*ptr && x_pos < click_x) {
                        int char_len = utf8_char_len(ptr);
                        char temp[5] = {0};
                        strncpy(temp, ptr, char_len);
                        
                        SDL_Surface* surf = TTF_RenderUTF8_Blended(box->font, temp, box->text_color);
                        x_pos += surf->w;
                        SDL_FreeSurface(surf);
                        
                        ptr += char_len;
                        col++;
                    }
                    
                    box->cursor_col = col;
                    box->select_start_row = box->select_end_row = row;
                    box->select_start_col = box->select_end_col = col;
                    box->has_selection = true;
                }
            }
            break;
        }
        
        case SDL_MOUSEMOTION: {
            if (event->motion.state & SDL_BUTTON_LMASK) {
                int move_x = event->motion.x - box->rect.x;
                int move_y = event->motion.y - box->rect.y + box->scroll_y;
                
                int row = move_y / LINE_HEIGHT;
                if (row >= 0 && row < box->line_count) {
                    box->select_end_row = row;
                    
                    const char* line = box->lines[row];
                    int col = 0;
                    int x_pos = TEXTBOX_PADDING;
                    const char* ptr = line;
                    
                    while (*ptr && x_pos < move_x) {
                        int char_len = utf8_char_len(ptr);
                        char temp[5] = {0};
                        strncpy(temp, ptr, char_len);
                        
                        SDL_Surface* surf = TTF_RenderUTF8_Blended(box->font, temp, box->text_color);
                        x_pos += surf->w;
                        SDL_FreeSurface(surf);
                        
                        ptr += char_len;
                        col++;
                    }
                    
                    box->select_end_col = col;
                }
            }
            break;
        }
        
        case SDL_TEXTINPUT: {
            insertTextAtCursor(box, event->text.text);
            break;
        }
        
        case SDL_KEYDOWN: {
            SDL_Keymod mod = SDL_GetModState();
            bool ctrl_pressed = (mod & KMOD_CTRL);
            
            if (ctrl_pressed) {
                switch (event->key.keysym.sym) {
                    case SDLK_c:
                        if (box->has_selection) {
                            char* selected = getSelectedText(box);
                            if (selected) {
                                SDL_SetClipboardText(selected);
                                free(selected);
                            }
                        }
                        break;
                        
                    case SDLK_x:
                        if (box->has_selection) {
                            char* selected = getSelectedText(box);
                            if (selected) {
                                SDL_SetClipboardText(selected);
                                deleteSelectedText(box);
                                free(selected);
                            }
                        }
                        break;
                        
                    case SDLK_v:
                        if (SDL_HasClipboardText()) {
                            char* text = SDL_GetClipboardText();
                            if (text) {
                                insertTextAtCursor(box, text);
                                SDL_free(text);
                            }
                        }
                        break;
                        
                    case SDLK_a:
                        box->select_start_row = 0;
                        box->select_start_col = 0;
                        box->select_end_row = box->line_count - 1;
                        box->select_end_col = utf8_strlen(box->lines[box->line_count - 1]);
                        box->has_selection = true;
                        break;
                }
            } else {
                switch (event->key.keysym.sym) {
                    case SDLK_RETURN:
                        if (box->line_count < MAX_LINES) {
                            char remaining[LINE_LENGTH];
							const char* cursor_pos = utf8_index(box->lines[box->cursor_row], box->cursor_col);
							strcpy(remaining, cursor_pos);
                            //strcpy(remaining, box->lines[box->cursor_row](const char*)utf8_index(box->lines[box->cursor_row], box->cursor_col) - box->lines[box->cursor_row]);
								  
								  
                            box->lines[box->cursor_row][(const char*)utf8_index(box->lines[box->cursor_row], box->cursor_col) - 
                                                      box->lines[box->cursor_row]] = '\0';
                            
                            for (int i = box->line_count; i > box->cursor_row + 1; i--) {
                                strcpy(box->lines[i], box->lines[i-1]);
                            }
                            
                            strcpy(box->lines[box->cursor_row + 1], remaining);
                            box->line_count++;
                            
                            box->cursor_row++;
                            box->cursor_col = 0;
                            
                            if ((box->cursor_row + 1) * LINE_HEIGHT - box->scroll_y > box->rect.h) {
                                box->scroll_y = (box->cursor_row + 1) * LINE_HEIGHT - box->rect.h;
                            }
                        }
                        break;
                        
                    case SDLK_BACKSPACE:
                        if (box->has_selection) {
                            deleteSelectedText(box);
                        } else if (box->cursor_col > 0) {
                            char* line = box->lines[box->cursor_row];
                            const char* cursor_pos = utf8_index(line, box->cursor_col);
                            const char* prev_pos = utf8_index(line, box->cursor_col - 1);
                            int char_len = cursor_pos - prev_pos;
                            
                            memmove((char*)prev_pos, cursor_pos, strlen(cursor_pos) + 1);
                            box->cursor_col--;
                        } else if (box->cursor_row > 0) {
                            int prev_len = utf8_strlen(box->lines[box->cursor_row - 1]);
                            if (strlen(box->lines[box->cursor_row - 1]) + strlen(box->lines[box->cursor_row]) < LINE_LENGTH) {
                                strcat(box->lines[box->cursor_row - 1], box->lines[box->cursor_row]);
                                
                                for (int i = box->cursor_row; i < box->line_count - 1; i++) {
                                    strcpy(box->lines[i], box->lines[i+1]);
                                }
                                
                                box->line_count--;
                                box->cursor_row--;
                                box->cursor_col = prev_len;
                            }
                        }
                        break;
                        
                    case SDLK_DELETE:
                        if (box->has_selection) {
                            deleteSelectedText(box);
                        } else {
                            char* line = box->lines[box->cursor_row];
                            const char* cursor_pos = utf8_index(line, box->cursor_col);
                            if (*cursor_pos) {
                                int char_len = utf8_char_len(cursor_pos);
                                memmove((char*)cursor_pos, cursor_pos + char_len, strlen(cursor_pos + char_len) + 1);
                            } else if (box->cursor_row < box->line_count - 1) {
                                if (strlen(line) + strlen(box->lines[box->cursor_row + 1]) < LINE_LENGTH) {
                                    strcat(line, box->lines[box->cursor_row + 1]);
                                    
                                    for (int i = box->cursor_row + 1; i < box->line_count - 1; i++) {
                                        strcpy(box->lines[i], box->lines[i+1]);
                                    }
                                    
                                    box->line_count--;
                                }
                            }
                        }
                        break;
                        
                    case SDLK_UP:
                        if (box->cursor_row > 0) {
                            box->cursor_row--;
                            int line_len = utf8_strlen(box->lines[box->cursor_row]);
                            if (box->cursor_col > line_len) box->cursor_col = line_len;
                            
                            if (box->cursor_row * LINE_HEIGHT < box->scroll_y) {
                                box->scroll_y = box->cursor_row * LINE_HEIGHT;
                            }
                        }
                        break;
                        
                    case SDLK_DOWN:
                        if (box->cursor_row < box->line_count - 1) {
                            box->cursor_row++;
                            int line_len = utf8_strlen(box->lines[box->cursor_row]);
                            if (box->cursor_col > line_len) box->cursor_col = line_len;
                            
                            if ((box->cursor_row + 1) * LINE_HEIGHT - box->scroll_y > box->rect.h) {
                                box->scroll_y = (box->cursor_row + 1) * LINE_HEIGHT - box->rect.h;
                            }
                        }
                        break;
                        
                    case SDLK_LEFT:
                        if (box->cursor_col > 0) {
                            box->cursor_col--;
                        } else if (box->cursor_row > 0) {
                            box->cursor_row--;
                            box->cursor_col = utf8_strlen(box->lines[box->cursor_row]);
                        }
                        break;
                        
                    case SDLK_RIGHT:
                        if (box->cursor_col < utf8_strlen(box->lines[box->cursor_row])) {
                            box->cursor_col++;
                        } else if (box->cursor_row < box->line_count - 1) {
                            box->cursor_row++;
                            box->cursor_col = 0;
                        }
                        break;
                        
                    case SDLK_HOME:
                        box->cursor_col = 0;
                        break;
                        
                    case SDLK_END:
                        box->cursor_col = utf8_strlen(box->lines[box->cursor_row]);
                        break;
                }
            }
            break;
        }
    }
}

void renderTextBox(SDL_Renderer* renderer, AdvancedTextBox* box) {
    // 绘制背景
    SDL_SetRenderDrawColor(renderer, box->bg_color.r, box->bg_color.g, box->bg_color.b, box->bg_color.a);
    SDL_RenderFillRect(renderer, &box->rect);
    
    // 绘制边框
    SDL_SetRenderDrawColor(renderer, 
                         box->is_focused ? 0 : box->border_color.r, 
                         box->is_focused ? 120 : box->border_color.g, 
                         box->is_focused ? 215 : box->border_color.b, 
                         box->border_color.a);
    SDL_RenderDrawRect(renderer, &box->rect);
    
    // 设置裁剪区域
    SDL_Rect clip = box->rect;
    SDL_RenderSetClipRect(renderer, &clip);
    
    // 渲染选中文本背景
    if (box->has_selection) {
        int start_row = (box->select_start_row < box->select_end_row || 
                       (box->select_start_row == box->select_end_row && 
                        box->select_start_col <= box->select_end_col)) 
                       ? box->select_start_row : box->select_end_row;
        int start_col = (start_row == box->select_start_row) ? box->select_start_col : box->select_end_col;
        int end_row = (start_row == box->select_start_row) ? box->select_end_row : box->select_start_row;
        int end_col = (end_row == box->select_end_row) ? box->select_end_col : box->select_start_col;
        
        SDL_SetRenderDrawColor(renderer, box->select_color.r, box->select_color.g, box->select_color.b, box->select_color.a);
        
        for (int row = start_row; row <= end_row; row++) {
            if (row < 0 || row >= box->line_count) continue;
            
            const char* line = box->lines[row];
            int sel_start = (row == start_row) ? start_col : 0;
            int sel_end = (row == end_row) ? end_col : utf8_strlen(line);
            
            if (sel_end > sel_start) {
                const char* start_ptr = utf8_index(line, sel_start);
                const char* end_ptr = utf8_index(line, sel_end);
                
                char selected_text[LINE_LENGTH] = {0};
                strncpy(selected_text, start_ptr, end_ptr - start_ptr);
                
                // 计算选中文本的宽度
                SDL_Surface* surf = TTF_RenderUTF8_Blended(box->font, selected_text, box->text_color);
                if (!surf) continue;
                
                // 计算选中文本的位置
                char prefix[LINE_LENGTH] = {0};
                strncpy(prefix, line, start_ptr - line);
                SDL_Surface* prefix_surf = TTF_RenderUTF8_Blended(box->font, prefix, box->text_color);
                
                SDL_Rect select_rect = {
                    box->rect.x + TEXTBOX_PADDING + (prefix_surf ? prefix_surf->w : 0),
                    box->rect.y + row * LINE_HEIGHT - box->scroll_y,
                    surf->w,
                    LINE_HEIGHT
                };
                
                SDL_RenderFillRect(renderer, &select_rect);
                
                SDL_FreeSurface(surf);
                if (prefix_surf) SDL_FreeSurface(prefix_surf);
            }
        }
    }
    
    // 渲染文本
    for (int row = 0; row < box->line_count; row++) {
        int y_pos = box->rect.y + row * LINE_HEIGHT - box->scroll_y;
        
        // 只渲染可见行
        if (y_pos + LINE_HEIGHT < box->rect.y || y_pos > box->rect.y + box->rect.h) {
            continue;
        }
        
        if (strlen(box->lines[row]) > 0) {
            SDL_Surface* text_surface = TTF_RenderUTF8_Blended(box->font, box->lines[row], box->text_color);
            if (text_surface) {
                SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
                
                SDL_Rect text_rect = {
                    box->rect.x + TEXTBOX_PADDING,
                    y_pos,
                    text_surface->w,
                    text_surface->h
                };
                
                SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
                SDL_FreeSurface(text_surface);
                SDL_DestroyTexture(text_texture);
            }
        }
    }
    
    // 渲染光标
    if (box->is_focused) {
        Uint32 current_time = SDL_GetTicks();
        if (current_time - box->last_cursor_blink > 500) {
            box->cursor_visible = !box->cursor_visible;
            box->last_cursor_blink = current_time;
        }
        
        if (box->cursor_visible) {
            const char* line = box->lines[box->cursor_row];
            const char* cursor_pos = utf8_index(line, box->cursor_col);
            
            // 计算光标前的文本宽度
            char prefix[LINE_LENGTH] = {0};
            strncpy(prefix, line, cursor_pos - line);
            SDL_Surface* surf = TTF_RenderUTF8_Blended(box->font, prefix, box->text_color);
            
            SDL_Rect cursor_rect = {
                box->rect.x + TEXTBOX_PADDING + (surf ? surf->w : 0),
                box->rect.y + box->cursor_row * LINE_HEIGHT - box->scroll_y,
                2,
                LINE_HEIGHT
            };
            
            SDL_SetRenderDrawColor(renderer, box->text_color.r, box->text_color.g, box->text_color.b, box->text_color.a);
            SDL_RenderFillRect(renderer, &cursor_rect);
            
            if (surf) SDL_FreeSurface(surf);
        }
    }
    
    // 重置裁剪区域
    SDL_RenderSetClipRect(renderer, NULL);
}

int main(int argc, char* argv[]) {
    initUTF8Environment();
    
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL初始化失败: %s\n", SDL_GetError());
        return 1;
    }
    
    if (TTF_Init() == -1) {
        printf("TTF初始化失败: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }
    
    SDL_Window* window = SDL_CreateWindow("中文文本框示例", 
                                        SDL_WINDOWPOS_CENTERED, 
                                        SDL_WINDOWPOS_CENTERED, 
                                        800, 600, 
                                        SDL_WINDOW_SHOWN);
    if (!window) {
        printf("窗口创建失败: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 
                                              SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        printf("渲染器创建失败: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    
    // 尝试加载中文字体（Windows下可以使用simhei.ttf，Linux/macOS使用其他中文字体）
    TTF_Font* font = NULL;
	const char* font_paths[] = {
		"C:/Windows/Fonts/simhei.ttf",  // 黑体
		"C:/Windows/Fonts/msyh.ttf",    // 微软雅黑
		"simhei.ttf",                   // 当前目录
		NULL
	};/*
	const char* font_paths[] = {
    "C:/Windows/Fonts/simhei.ttf"                   // Windows
 //   ,"/usr/share/fonts/truetype/wqy/wqy-microhei.ttc",  // Linux (修正了wgy->wqy)
 //   "/System/Library/Fonts/STHeiti Medium.ttc",  // macOS
    "msyh.ttf",                     // 微软雅黑
    NULL
};*/
//SDL_Delay(3000);
// 尝试加载字体
for (int i = 0; font_paths[i]; i++) {
    font = TTF_OpenFont(font_paths[i], 16);
    if (font) {
        printf("成功加载字体: %s\n", font_paths[i]);
        break;
    } else {
        printf("无法加载字体: %s, 错误: %s\n", font_paths[i], TTF_GetError());
    }
}

// 如果所有中文字体都加载失败，尝试加载默认字体
if (!font) {
    printf("无法加载中文字体，尝试加载默认字体\n");
    const char* default_fonts[] = {
        "arial.ttf",
        "LiberationSans-Regular.ttf",
        "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
        NULL
    };
    
    for (int i = 0; default_fonts[i]; i++) {
        font = TTF_OpenFont(default_fonts[i], 16);
        if (font) {
            printf("成功加载默认字体: %s\n", default_fonts[i]);
            break;
        }
    }
    
    if (!font) {
        printf("无法加载任何字体，最后错误: %s\n", TTF_GetError());
       //SDL_Delay(3000);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
}
    
    AdvancedTextBox textbox;
    initAdvancedTextBox(&textbox, 50, 50, 700, 500, font);
    textbox.is_focused = true;
    
    SDL_StartTextInput();///
    
    int running = 1;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x;
                int y = event.button.y;
                textbox.is_focused = (x >= textbox.rect.x && x <= textbox.rect.x + textbox.rect.w &&
                                     y >= textbox.rect.y && y <= textbox.rect.y + textbox.rect.h);
            }
            
            handleTextBoxEvent(&textbox, &event);
        }
        
        SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
        SDL_RenderClear(renderer);
        
        renderTextBox(renderer, &textbox);
        
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    
    SDL_StopTextInput();
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    
    return 0;
}