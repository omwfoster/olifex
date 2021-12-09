/*
 * pixel.c
 *
 *  Created on: Nov 4, 2021
 *      Author: omwfo
 */

#include "pixel.h"
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>



static bool is_seed_set = false;


void blend(const uint8_t *colourA, const uint8_t *colourB, uint8_t *colourOut,
		float amount) {
	float r, g, b;

	r = ((float) colourB[0] * amount) + ((float) colourA[0] * (1.0 - amount));
	g = ((float) colourB[1] * amount) + ((float) colourA[1] * (1.0 - amount));
	b = ((float) colourB[2] * amount) + ((float) colourA[2] * (1.0 - amount));

	colourOut[0] = (r > 255.0) ? 255.0 : (r < 0.0) ? 0.0 : r;
	colourOut[1] = (g > 255.0) ? 255.0 : (g < 0.0) ? 0.0 : g;
	colourOut[2] = (b > 255.0) ? 255.0 : (b < 0.0) ? 0.0 : b;
}

static void randomSeed(uint32_t seed)
{
    if(seed != 0)
    {
        is_seed_set = true;
        srand(seed);
    }
}

uint32_t random_gen(uint32_t a, uint32_t b)
{
   return (random() % (b-a)) + a;
}

void RunningLights(const UINT32_RGB *c, int delay_ms, float time_s)
{
    UINT32_RGB new_color;
    int num_of_frames = time_s * 1000 / delay_ms;
    for (int j = 0; j < num_of_frames; j++)
    {
        for (int i = 0; i < NUMBER_OF_PIXELS; i++)
        {
            setColorBrightness(c,&new_color,(sin(i + j) * 127 + 128) / 255.0);
         //   setPixel_GRB(&new_color, i);
        }
    }
}

void FadeInOut(const UINT32_RGB *c, int delay_ms, float time_s)
{
    UINT32_RGB new_color;
    if (time_s < 1.0)
        time_s = 1.0;
    int num_of_frames = time_s * 1000/delay_ms;
    float k = num_of_frames - 1;
    for (int i = 0; i < num_of_frames; i++)
    {
        setColorBrightness(c,&new_color,i/k);
        setAll_GRB(&new_color);
    }
    for (int i = num_of_frames - 1; i >= 0; i--)
    {
        setColorBrightness(c,&new_color,i/k);
        setAll_GRB(&new_color);

    }
}

void Twinkle(const UINT32_RGB *c, int delay_ms, int count, bool clear)
{}

void CyloneBounce(const UINT32_RGB *c,int eye_size)
{
    int j = NUMBER_OF_PIXELS - (eye_size + 2);
    UINT32_RGB faded_color;
    setColorBrightness(c,&faded_color,0.1);
    for (int i = 0; i < j; i++)
    {
        clearAll();
     //   setPixel_GRB(&faded_color,i);
        setRange_GRB(c,i+1,eye_size);
     //   setPixel_GRB(&faded_color,i+eye_size+1);


    }

    for (int i = j; i >=0; i--)
    {
        clearAll();
    //    setPixel_GRB(&faded_color,i);
        setRange_GRB(c,i+1,eye_size);
    //    setPixel_GRB(&faded_color,i+eye_size+1);

    }

}

void ColorWipe(const UINT32_RGB *c, int delay_ms, bool reverse)
{
    if (reverse)
    {
        for (int i = NUMBER_OF_PIXELS-1; i >= 0; i--)
        {
        //    setPixel_GRB(c, i);
        }
    }
    else
    {
        for (int i = 0; i < NUMBER_OF_PIXELS; i++)
        {
       //     setPixel_GRB(c, i);
        }
    }
}



void RainbowCycle(int delay_ms, int cycle_num)
{
    UINT32_RGB color;
    for(int i=0; i<256 * cycle_num; i++)
    {
        for(int j=0; j < NUMBER_OF_PIXELS; j++)
        {
      //      Wheel(&color, ((j * 256 / NUMBER_OF_PIXELS) + i) & 0xff);
         //   setPixel_GRB(&color, j);
        }
    }
}

void TheatreChase(const UINT32_RGB * c, int delay_ms, int cycle_num)
{
    for(int i=0; i<cycle_num; i++)
    {
        for(int q=0;q<3;q++)
        {
            clearAll();
            for(int j=0; (j+q)< NUMBER_OF_PIXELS; j+=3)
            {
        //        setPixel_GRB(c,j+q);
            }
        }
    }
}

void TheatreChaseRainbow(int delay_ms, int cycle_num)
{
    UINT32_RGB color;
    for(int i=0; i< 256 * cycle_num; i++)
    {
        for(int q=0;q<3;q++)
        {
            clearAll();
            for(int j=0; j< NUMBER_OF_PIXELS; j+=3)
            {
   //             Wheel(&color,(i+j) % 255);
           //     setPixel_GRB(&color,j+q);
            }
        //    wait_ms(delay_ms);
        }
    }
}


void HalfBlink(const UINT32_RGB * c, int delay_ms)
{
    clearAll();
    setRange_GRB(c,0,NUMBER_OF_PIXELS/2);
//   wait_ms(delay_ms);
    clearAll();
    setRange_GRB(c, NUMBER_OF_PIXELS/2, NUMBER_OF_PIXELS/2);
 //   wait_ms(delay_ms);
}




void setPixel_GRB(XRGB * _Color, UINT32_RGB * _Pixel)
{

	_Pixel->xRGB=* _Color;

}
