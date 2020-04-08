/*
 * Animation.h
 * (c) 2019 Oliver Hanka - mail@oliverhanka.de
 */

#ifndef ANIMATION_H
#define ANIMATION_H

#include <NeoPixelBrightnessBus.h>


/* WS2812b CONFIG */
#define BODY_LED_PIN  17
#define TAIL_LED_PIN  16

#define BODY_LED_NUM  9
#define TAIL_LED_NUM  6

#define LED_BRIGHTNESS  100

/* ANIMATION CONFIG */
#define ANIMATION_TICK_INTERVALL (10*1000)  

class Animation {
  
  public:
    Animation(NeoPixelBrightnessBus<NeoGrbFeature, NeoEsp32Rmt0800KbpsMethod> *body_pixels, NeoPixelBrightnessBus<NeoGrbFeature, NeoEsp32Rmt1800KbpsMethod> *tail_pixels) {
      _body_pixels = body_pixels;
      _tail_pixels = tail_pixels;
    }
    
    virtual void init() = 0;
    virtual void advance() = 0;
    
  protected:

    NeoPixelBrightnessBus<NeoGrbFeature, NeoEsp32Rmt0800KbpsMethod> *_body_pixels;
    NeoPixelBrightnessBus<NeoGrbFeature, NeoEsp32Rmt1800KbpsMethod> *_tail_pixels;

    // hold the values for the current animation state
    struct currentState_t {
      RgbColor body[BODY_LED_NUM];
      RgbColor tail[TAIL_LED_NUM];
    } currentState;

    void allOff() {
      for (uint16_t loop = 0; loop < BODY_LED_NUM; loop++)
        currentState.body[loop] = 0;
      for (uint16_t loop = 0; loop < TAIL_LED_NUM; loop++)
        currentState.tail[loop] = 0;
      show();
    }

    // show current animation state
    void show() {
      if (_body_pixels->GetBrightness() != LED_BRIGHTNESS) {
        _body_pixels->SetBrightness(LED_BRIGHTNESS);
        _tail_pixels->SetBrightness(LED_BRIGHTNESS);
      }
      for (uint16_t loop = 0; loop < BODY_LED_NUM; loop++)
        _body_pixels->SetPixelColor(loop,currentState.body[loop]);
      for (uint16_t loop = 0; loop < TAIL_LED_NUM; loop++)
        _tail_pixels->SetPixelColor(loop,currentState.tail[loop]);
      _body_pixels->Show();
      _tail_pixels->Show();
    }

    // convert seperate red, green, blue 8bit values into a packed 32bit value
    static RgbColor Color(uint8_t r, uint8_t g, uint8_t b) {
        RgbColor colorObject(r,g,b);
        return colorObject;
    }
};
  

#endif /* ANIMATION_H */