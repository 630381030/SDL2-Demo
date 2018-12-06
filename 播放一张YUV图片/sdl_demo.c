#include <stdio.h>
#include <SDL2/SDL.h>

// 显示窗口大小
int WINDOW_W = 640;
int WINDOW_H = 480;

int thread_exit = 0;

int refresh_video(void *opaque)
{
    while(thread_exit == 0)
    {
        SDL_Event event;
        event.type = SDL_USEREVENT + 1;
        SDL_PushEvent(&event);
        SDL_Delay(40);
    }
    printf("exit refresh video thread.\n");
    return 0;
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("SDL Demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_W, WINDOW_H, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, WINDOW_W, WINDOW_H);

    FILE *fp = fopen("test.iyuv", "rb");
    if (!fp)
    {
        printf("Cannot open the file.\n");
        return -1;
    }

    // 图片的宽高
    const int IMG_W = 640;
    const int IMG_H = 480;

    uint8_t buff[IMG_W * IMG_H * 3 / 2];
    fread(buff, 1, IMG_W * IMG_H * 3 / 2, fp);
    fclose(fp);

    SDL_Thread *thread = SDL_CreateThread(refresh_video, NULL, NULL);

    SDL_Event event;
    SDL_Rect rect;

    while (1)
    {
        SDL_WaitEvent(&event);
        if (event.type == SDL_USEREVENT + 1)
        {
            SDL_UpdateTexture(texture, NULL, buff, IMG_W);

            rect.x = 0;
            rect.y = 0;
            rect.w = WINDOW_W;
            rect.h = WINDOW_H;

            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, NULL, &rect);
            SDL_RenderPresent(renderer);

            SDL_Delay(40);
        }
        else if(event.type == SDL_WINDOWEVENT)
        {
            SDL_GetWindowSize(window, &WINDOW_W, &WINDOW_H);
            printf("window resize, w=%d h=%d\n", WINDOW_W, WINDOW_H);
        }
        else if(event.type == SDL_QUIT)
        {
            printf("Exit.\n");
            break;
        }
    }

    return 0;
}