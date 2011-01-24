#ifdef __cplusplus
#include <cstdlib>
#else
#include <stdlib.h>
#endif

#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <inttypes.h>

#include "System.h"

int32_t Max;//FileSize(s); i-1

bool     setka  =false;
int32_t Offs   =0;
int32_t Offst  =0;
int32_t WIDT   =42;
int32_t HIGH   =100;
int32_t rejim  =4;
int32_t DX     =0;

FILE *fl;


void Render(SDL_Surface *srf,uint32_t rej,uint32_t wi,uint32_t hi, uint32_t dx);

void FillRect(SDL_Surface *srf,int x, int y, int w, int h, uint32_t color)
{
    SDL_Rect tmp;
    tmp.x=x;
    tmp.y=y;
    tmp.w=w;
    tmp.h=h;
    SDL_FillRect(srf, &tmp , color);
}

int main ( int argc, char** argv )
{
    // initialize SDL video
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }

    // make sure SDL cleans up before exit
    atexit(SDL_Quit);

    // create a new window
    SDL_Surface* screen = SDL_SetVideoMode(800, 600, 24,
                                           SDL_SWSURFACE);
    if ( !screen )
    {
        printf("Unable to set 640x480 video: %s\n", SDL_GetError());
        return 1;
    }

    SDL_WM_SetCaption("Raw Viewer","Raw Viewer");

    // load an image
    SDL_Surface* buffer = SDL_CreateRGBSurface(SDL_SWSURFACE,600,524,32,0,0,0,0);

    fl=fopen(argv[1],"rb");
    fseek(fl,0,SEEK_END);
    Max=ftell(fl);

    Render(buffer,rejim,WIDT,HIGH,DX);


    // program main loop
    bool done = false;
    while (!done)
    {
        // message processing loop
        FlushHits();
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            // check for messages
            switch (event.type)
            {
                // exit if the window is closed
            case SDL_QUIT:
                done = true;
                break;

                // check for keypresses
            case SDL_KEYDOWN:
            {
                SetHit(event.key.keysym.sym);
                break;
            }
            } // end switch
        } // end of message processing
        UpdateKeyboard();

        // DRAWING STARTS HERE

        if (KeyDown(SDLK_ESCAPE))
            done=true;


        // clear screen
        SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));

        SDL_BlitSurface(buffer,0,screen,0);

        lineColor(screen,0,524,600,524,0xFFFFFFFF);
        lineColor(screen,600,0,600,524,0xFFFFFFFF);

        char buf[255];
        sprintf(buf,"Offset:%X",Offs);
        stringColor(screen,700,10,buf,0xFFFFFFFF);
        sprintf(buf,"Width:%d",WIDT);
        stringColor(screen,700,20,buf,0xFFFFFFFF);

        rectangleColor(screen,663,7,670,509,0xFF0000FF);
        FillRect(screen,665,9,4,501*(Offs/Max),0x00FF00);//rectangleColor(screen,664,8,669,8+500*Offs/Max,0x00FF00FF);
        if (WIDT>150)
        {
            rectangleColor(screen,9,549,511,556,0xFF0000FF); //Rect  9,549,502,7,0
            FillRect(screen,10,550,500*DX/(WIDT-150),5,0x00FF00);//	Rect  10,550,500*(Float(DX)/Float(widt-150)),5,1
        }

        switch (rejim)
        {
        case 1:
            sprintf(buf,"1 Bit mode");
            break;
        case 2:
            sprintf(buf,"2 Bit mode");
            break;
        case 3:
            sprintf(buf,"4 Bit mode");
            break;
        case 4:
            sprintf(buf,"8 Bit mode");
            break;
        case 5:
            sprintf(buf,"16 Bit mode RGBA5551");
            break;
        case 6:
            sprintf(buf,"16 Bit mode RGBA4444");
            break;
        case 7:
            sprintf(buf,"16 Bit mode RGB565");
            break;
        case 8:
            sprintf(buf,"16 Bit mode BGRA5551");
            break;
        case 9:
            sprintf(buf,"16 Bit mode BGRA4444");
            break;
        case 10:
            sprintf(buf,"16 Bit mode BGR565");
            break;
        case 11:
            sprintf(buf,"24 Bit mode RGB");
            break;
        case 12:
            sprintf(buf,"24 Bit mode BGR");
            break;
        }

        stringColor(screen,5,580,buf,0xFFFFFFFF);


        if (KeyHit(SDLK_SPACE))
        {
            setka=!setka;
            Render(buffer,rejim,WIDT,HIGH,DX);
        }



        if (KeyDown(SDLK_PERIOD))
        {
            WIDT++;
            DX=0;
            if (WIDT<1)
                WIDT =1;
            Render(buffer,rejim,WIDT,HIGH,DX);
        }
        if (KeyDown(SDLK_COMMA))
        {
            WIDT--;
            DX=0;
            if (WIDT<1)
                WIDT =1;
            Render(buffer,rejim,WIDT,HIGH,DX);
        }
        if (KeyDown(SDLK_SEMICOLON))
        {
            WIDT+=10;
            DX=0;
            if (WIDT<1)
                WIDT =1;
            Render(buffer,rejim,WIDT,HIGH,DX);
        }
        if (KeyDown(SDLK_l))
        {
            WIDT-=10;
            DX=0;
            if (WIDT<1)
                WIDT =1;
            Render(buffer,rejim,WIDT,HIGH,DX);
        }
        if (KeyDown(SDLK_LEFTBRACKET))
        {
            WIDT+=100;
            DX=0;
            if (WIDT<1)
                WIDT =1;
            Render(buffer,rejim,WIDT,HIGH,DX);
        }
        if (KeyDown(SDLK_p))
        {
            WIDT-=100;
            DX=0;
            if (WIDT<1)
                WIDT =1;
            Render(buffer,rejim,WIDT,HIGH,DX);
        }



        if (MouseDown(1))
        {

            if (MouseX()>663 && MouseX()<670 && MouseY()>7 && MouseY()<507)
            {
                Offs=Max*((MouseY()-8))/500.0;
                Render(buffer,rejim,WIDT,HIGH,DX);
            }


            if (WIDT>150)
            {
                if (MouseX()>9 && MouseX()<511 && MouseY()>549 && MouseY()<556)
                {
                    DX=(WIDT -150)*((MouseX()-10)/500.0);
                    Render(buffer,rejim,WIDT,HIGH,DX);
                }
            }
        }
        if (MouseDown(3))
        {
            HIGH=MouseY()/4;
            if (HIGH<1)
                HIGH=1;

            if (HIGH> 130)
                HIGH=130;
            Render(buffer,rejim,WIDT,HIGH,DX);
        }


        if (KeyHit(SDLK_1))
        {
            rejim=1;
            Render(buffer,rejim,WIDT,HIGH,DX);
        }
        if (KeyHit(SDLK_2))
        {
            rejim=2;
            Render(buffer,rejim,WIDT,HIGH,DX);
        }
        if (KeyHit(SDLK_3))
        {
            rejim=3;
            Render(buffer,rejim,WIDT,HIGH,DX);
        }
        if (KeyHit(SDLK_4))
        {
            rejim=4;
            Render(buffer,rejim,WIDT,HIGH,DX);
        }
        if (KeyHit(SDLK_5))
        {
            rejim=5;
            Render(buffer,rejim,WIDT,HIGH,DX);
        }
        if (KeyHit(SDLK_6))
        {
            rejim=6;
            Render(buffer,rejim,WIDT,HIGH,DX);
        }
        if (KeyHit(SDLK_7))
        {
            rejim=7;
            Render(buffer,rejim,WIDT,HIGH,DX);
        }
        if (KeyHit(SDLK_8))
        {
            rejim=8;
            Render(buffer,rejim,WIDT,HIGH,DX);
        }
        if (KeyHit(SDLK_9))
        {
            rejim=9;
            Render(buffer,rejim,WIDT,HIGH,DX);
        }
        if (KeyHit(SDLK_0))
        {
            rejim=10;
            Render(buffer,rejim,WIDT,HIGH,DX);
        }

        if (KeyHit(SDLK_MINUS))
        {
            rejim=11;
            Render(buffer,rejim,WIDT,HIGH,DX);
        }

        if (KeyHit(SDLK_EQUALS))
        {
            rejim=12;
            Render(buffer,rejim,WIDT,HIGH,DX);
        }

        if (KeyDown(SDLK_RIGHT))
        {
            Offs++;
            Render(buffer,rejim,WIDT,HIGH,DX);
        }
        if (KeyDown(SDLK_LEFT))
        {
            Offs--;
            Render(buffer,rejim,WIDT,HIGH,DX);
        }

        if (rejim<5)
        {
            if (KeyDown(SDLK_DOWN))
            {
                Offs+=WIDT;
                Render(buffer,rejim,WIDT,HIGH,DX);
            }
            if (KeyDown(SDLK_UP))
            {
                Offs-=WIDT;
                Render(buffer,rejim,WIDT,HIGH,DX);
            }
            if (KeyDown(SDLK_PAGEUP))
            {
                Offs-=WIDT*10;
                Render(buffer,rejim,WIDT,HIGH,DX);
            }
            if (KeyDown(SDLK_PAGEDOWN))
            {
                Offs+=WIDT*10;
                Render(buffer,rejim,WIDT,HIGH,DX);
            }
            if (KeyDown(SDLK_HOME))
            {
                Offs-=WIDT*100;
                Render(buffer,rejim,WIDT,HIGH,DX);
            }
            if (KeyDown(SDLK_END))
            {
                Offs+=WIDT*100;
                Render(buffer,rejim,WIDT,HIGH,DX);
            }
        }
        else if (rejim < 11)
        {
            if (KeyDown(SDLK_DOWN))
            {
                Offs+=WIDT*2;
                Render(buffer,rejim,WIDT,HIGH,DX);
            }
            if (KeyDown(SDLK_UP))
            {
                Offs-=WIDT*2;
                Render(buffer,rejim,WIDT,HIGH,DX);
            }
            if (KeyDown(SDLK_PAGEUP))
            {
                Offs-=WIDT*20;
                Render(buffer,rejim,WIDT,HIGH,DX);
            }
            if (KeyDown(SDLK_PAGEDOWN))
            {
                Offs+=WIDT*20;
                Render(buffer,rejim,WIDT,HIGH,DX);
            }
            if (KeyDown(SDLK_HOME))
            {
                Offs-=WIDT*200;
                Render(buffer,rejim,WIDT,HIGH,DX);
            }
            if (KeyDown(SDLK_END))
            {
                Offs+=WIDT*200;
                Render(buffer,rejim,WIDT,HIGH,DX);
            }
        }
        else if (rejim < 13)
        {
            if (KeyDown(SDLK_DOWN))
            {
                Offs+=WIDT*3;
                Render(buffer,rejim,WIDT,HIGH,DX);
            }
            if (KeyDown(SDLK_UP))
            {
                Offs-=WIDT*3;
                Render(buffer,rejim,WIDT,HIGH,DX);
            }
            if (KeyDown(SDLK_PAGEUP))
            {
                Offs-=WIDT*30;
                Render(buffer,rejim,WIDT,HIGH,DX);
            }
            if (KeyDown(SDLK_PAGEDOWN))
            {
                Offs+=WIDT*30;
                Render(buffer,rejim,WIDT,HIGH,DX);
            }
            if (KeyDown(SDLK_HOME))
            {
                Offs-=WIDT*300;
                Render(buffer,rejim,WIDT,HIGH,DX);
            }
            if (KeyDown(SDLK_END))
            {
                Offs+=WIDT*300;
                Render(buffer,rejim,WIDT,HIGH,DX);
            }
        }


        // DRAWING ENDS HERE


        // finally, update the screen :)
        SDL_Flip(screen);
        SDL_Delay(10);
    } // end main loop

    fclose(fl);

    return 0;
}



void Render(SDL_Surface *srf,uint32_t rej,uint32_t wi,uint32_t hi, uint32_t dx)
{
    FillRect(srf,0,0,600,524,0);
    int ddX=dx;
    int ii;
    int jj;
    int tmp,tmp2;

    if (wi<151)
        ddX=0;
    else if (dx>(wi-150))
        ddX=(wi-150);

    if (Offs>=Max)
        Offs=Max-1;
    else if (Offs<0)
        Offs=0;


    int off=ddX+Offs;
    tmp=0;

    switch (rej)
    {

    case 1:
        ii=0;
        jj=0;
        while (ii<wi*hi)
        {
            if (((ii/8)+off)>=Max)
                break;
            if ((ii % wi)< 150)
            {

                fseek(fl,(ii/8)+off,SEEK_SET);
                fread(&tmp,1,1,fl);
                for (int kl=0; kl<8; kl++)
                {
                    tmp2=(tmp >> kl) & 1;
                    if (tmp2==1)
                        tmp2=0x7F7F7F;

                    jj= ii/wi;
                    if ((ii % wi)< 150)
                        FillRect(srf,(ii % wi)*4,jj*4,4,4,tmp2);
                    ii++;
                }
                if (setka)
                {
                    jj= (ii-8)/wi;
                    lineColor (srf,((ii-8) % wi)*4,jj*4,((ii-8) % wi)*4,jj*4+4,0xFF0000FF);
                }
            }
            else
                ii+=8;
        }
        break;
    case 2:
        ii=0;
        jj=0;
        while (ii<wi*hi)
        {
            if (((ii/4)+off)>=Max)
                break;
            if ((ii % wi)< 150)
            {

                fseek(fl,(ii/4)+off,SEEK_SET);
                fread(&tmp,1,1,fl);
                for (int kl=0; kl<7; kl+=2)
                {
                    tmp2=(tmp >> kl) & 3;
                    if (tmp2==1)
                        tmp2=0x404040;
                    if (tmp2==2)
                        tmp2=0x808080;
                    if (tmp2==3)
                        tmp2=0xB0B0B0;

                    jj= ii/wi;
                    if ((ii % wi)< 150)
                        FillRect(srf,(ii % wi)*4,jj*4,4,4,tmp2);
                    ii++;
                }
                if (setka)
                {
                    jj= (ii-4)/wi;
                    lineColor (srf,((ii-4) % wi)*4,jj*4,((ii-4) % wi)*4,jj*4+4,0xFF0000FF);
                }
            }
            else
                ii+=4;
        }
        break;

    case 3: //4bit
        ii=0;
        jj=0;
        while (ii<wi*hi)
        {
            if (((ii/2)+off)>=Max)
                break;
            if ((ii % wi)< 150)
            {

                fseek(fl,(ii/2)+off,SEEK_SET);
                fread(&tmp,1,1,fl);
                for (int kl=0; kl<8; kl+=4)
                {
                    tmp2=(tmp >> kl) & 0x0F;
                    tmp2= (tmp2 << 4) | (tmp2 << 12) | (tmp2 << 20);

                    jj= ii/wi;
                    if ((ii % wi)< 150)
                        FillRect(srf,(ii % wi)*4,jj*4,4,4,tmp2);
                    ii++;
                }
                if (setka)
                {
                    jj= (ii-2)/wi;
                    lineColor (srf,((ii-2) % wi)*4,jj*4,((ii-2) % wi)*4,jj*4+4,0xFF0000FF);
                }
            }
            else
                ii+=2;
        }
        break;
    case 4: //8bit
        ii=0;
        while (ii<wi*hi)
        {
            if ((ii+off)>=Max)
                break;
            if ((ii % wi)< 150)
            {
                fseek(fl,ii+off,SEEK_SET);
                fread(&tmp,1,1,fl);
                tmp2= (tmp << 8) | (tmp << 16) | tmp;

                jj= ii/wi;
                if ((ii % wi)< 150)
                    FillRect(srf,(ii % wi)*4,jj*4,4,4,tmp2);
                ii++;
            }
            else
                ii++;
        }
        break;

    case 5: //16bit RGB5551
        ii=0;
        while (ii<wi*hi)
        {
            if ((ii*2+1+off)>=Max)
                break;
            if ((ii % wi)< 150)
            {
                fseek(fl,ii*2+off,SEEK_SET);
                fread(&tmp,2,1,fl);
                tmp2 = ((tmp & 0x1F)*8)<<16 | (((tmp >> 5) & 0x1F)*8)<<8 | (((tmp >> 10) & 0x1F)*8);
                jj= ii/wi;
                if ((ii % wi)< 150)
                    FillRect(srf,(ii % wi)*4,jj*4,4,4,tmp2 );
                ii++;
            }
            else
                ii++;
        }
        break;
    case 6: //16bit RGB444A
        ii=0;
        while (ii<wi*hi)
        {
            if ((ii*2+1+off)>=Max)
                break;
            if ((ii % wi)< 150)
            {
                fseek(fl,ii*2+off,SEEK_SET);
                fread(&tmp,2,1,fl);
                tmp2= (((tmp >> 8) & 0xF)*0x10)<<16  | (((tmp >> 4) & 0xF)*0x10)<<8  | ((tmp & 0xF)*0x10);

                jj= ii/wi;
                if ((ii % wi)< 150)
                    FillRect(srf,(ii % wi)*4,jj*4,4,4,tmp2);
                ii++;
            }
            else
                ii++;
        }
        break;
    case 7: //16bit RGB565
        ii=0;
        while (ii<wi*hi)
        {
            if ((ii*2+1+off)>=Max)
                break;
            if ((ii % wi)< 150)
            {
                fseek(fl,ii*2+off,SEEK_SET);
                fread(&tmp,2,1,fl);
                tmp2= (((tmp >> 11) & 0x1F)*8) <<16 | (((tmp >> 5) & 0x3F)*4)<<8 | ((tmp & 0x1F)*8);

                jj= ii/wi;
                if ((ii % wi)< 150)
                    FillRect(srf,(ii % wi)*4,jj*4,4,4,tmp2);
                ii++;
            }
            else
                ii++;
        }
        break;
    case 8: //16bit BGR5551
        ii=0;
        while (ii<wi*hi)
        {
            if ((ii*2+1+off)>=Max)
                break;
            if ((ii % wi)< 150)
            {
                fseek(fl,ii*2+off,SEEK_SET);
                fread(&tmp,2,1,fl);
                tmp2 = ((tmp & 0x1F)*8) | (((tmp >> 5) & 0x1F)*8)<<8 | (((tmp >> 10) & 0x1F)*8)<<16;

                jj= ii/wi;
                if ((ii % wi)< 150)
                    FillRect(srf,(ii % wi)*4,jj*4,4,4,tmp2 & 0xFFFFFF);
                ii++;
            }
            else
                ii++;
        }
        break;
    case 9: //16bit BGR444A
        ii=0;
        while (ii<wi*hi)
        {
            if ((ii*2+1+off)>=Max)
                break;
            if ((ii % wi)< 150)
            {
                fseek(fl,ii*2+off,SEEK_SET);
                fread(&tmp,2,1,fl);
                tmp2= (((tmp >> 8) & 0xF)*0x10)  | (((tmp >> 4) & 0xF)*0x10)<<8  | ((tmp & 0xF)*0x10) <<16;

                jj= ii/wi;
                if ((ii % wi)< 150)
                    FillRect(srf,(ii % wi)*4,jj*4,4,4,tmp2 & 0xFFFFFF);
                ii++;
            }
            else
                ii++;
        }
        break;
    case 10: //16bit BGR565
        ii=0;
        while (ii<wi*hi)
        {
            if ((ii*2+1+off)>=Max)
                break;
            if ((ii % wi)< 150)
            {
                fseek(fl,ii*2+off,SEEK_SET);
                fread(&tmp,2,1,fl);
                tmp2= (((tmp >> 11) & 0x1F)*8) | (((tmp >> 5) & 0x3F)*4)<<8 | ((tmp & 0x1F)*8)<<16 ;

                jj= ii/wi;
                if ((ii % wi)< 150)
                    FillRect(srf,(ii % wi)*4,jj*4,4,4,tmp2);
                ii++;
            }
            else
                ii++;
        }
        break;
    case 11: //24bit RGB888
        ii=0;
        while (ii<wi*hi)
        {
            if ((ii*3+2+off)>=Max)
                break;
            if ((ii % wi)< 150)
            {
                fseek(fl,ii*3+off,SEEK_SET);
                fread(&tmp,3,1,fl);
                tmp2= tmp & 0xFFFFFF;

                jj= ii/wi;
                if ((ii % wi)< 150)
                    FillRect(srf,(ii % wi)*4,jj*4,4,4,tmp2);
                ii++;
            }
            else
                ii++;
        }
        break;
    case 12: //24bit BGR
        ii=0;
        while (ii<wi*hi)
        {
            if ((ii*3+2+off)>=Max)
                break;
            if ((ii % wi)< 150)
            {
                fseek(fl,ii*3+off,SEEK_SET);
                fread(&tmp,3,1,fl);
                tmp2= ((tmp & 0xFF) << 16) | (tmp & 0x00FF00) | ((tmp >> 16) & 0xFF);

                jj= ii/wi;
                if ((ii % wi)< 150)
                    FillRect(srf,(ii % wi)*4,jj*4,4,4,tmp2);
                ii++;
            }
            else
                ii++;
        }
        break;
    }
}
