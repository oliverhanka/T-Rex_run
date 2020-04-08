/*
 * AnimationRainbow.h
 * (c) 2019 Oliver Hanka - mail@oliverhanka.de
 */

#ifndef ANIMATIONRAINBOW_H
#define ANIMATIONRAINBOW_H

#include <NeoPixelBrightnessBus.h>
#include "Animation.h"

class AnimationRainbow : public Animation {
  public:
    AnimationRainbow(NeoPixelBrightnessBus<NeoGrbFeature, NeoEsp32Rmt0800KbpsMethod> *body_pixels, NeoPixelBrightnessBus<NeoGrbFeature, NeoEsp32Rmt1800KbpsMethod> *tail_pixels);
    void init();
    void advance();
  private:
    uint16_t rainbowCycle;
    RgbColor Wheel(byte WheelPos);
};


#endif /* ANIMATIONRAINBOW_H */