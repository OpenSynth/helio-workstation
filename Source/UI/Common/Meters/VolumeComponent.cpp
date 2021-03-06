/*
    This file is part of Helio Workstation.

    Helio is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Helio is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Helio. If not, see <http://www.gnu.org/licenses/>.
*/

#include "Common.h"
#include "VolumeComponent.h"
#include "AudioMonitor.h"
#include "AudioCore.h"

#define HQ_METER_MAXDB (+4.0f)
#define HQ_METER_MINDB (-70.0f)
#define HQ_METER_DECAY_RATE1 (1.0f - 3E-5f)
#define HQ_METER_DECAY_RATE2 (1.0f - 3E-7f)
#define HQ_METER_CYCLES_BEFORE_PEAK_FALLOFF 100

VolumeComponent::VolumeComponent(WeakReference<AudioMonitor> targetAnalyzer,
                                 int targetChannel,
                                 Orientation bandOrientation) :
    Thread("Volume Component"),
    volumeAnalyzer(std::move(targetAnalyzer)),
    peakBand(this),
    channel(targetChannel),
    skewTime(0),
    orientation(bandOrientation)
{
    this->setInterceptsMouseClicks(false, false);
    
    if (this->volumeAnalyzer != nullptr)
    {
        this->startThread(5);
    }
}

VolumeComponent::~VolumeComponent()
{
    this->stopThread(1000);
}

void VolumeComponent::setTargetAnalyzer(WeakReference<AudioMonitor> targetAnalyzer)
{
    if (targetAnalyzer != nullptr)
    {
        this->volumeAnalyzer = targetAnalyzer;
        this->startThread(5);
    }
}

void VolumeComponent::run()
{
    while (! this->threadShouldExit())
    {
        Thread::sleep(jlimit(10, 100, 35 - this->skewTime));
        const double b = Time::getMillisecondCounterHiRes();
        this->triggerAsyncUpdate();
        const double a = Time::getMillisecondCounterHiRes();
        this->skewTime = int(a - b);
    }
}

void VolumeComponent::handleAsyncUpdate()
{
    this->repaint();
}

inline float VolumeComponent::iecLevel(const float dB) const
{
    float fDef = 1.0;
    
    if (dB < -70.0) {
        fDef = 0.0;
    } else if (dB < -60.0) {
        fDef = (dB + 70.0) * 0.0025;
    } else if (dB < -50.0) {
        fDef = (dB + 60.0) * 0.005 + 0.025;
    } else if (dB < -40.0) {
        fDef = (dB + 50.0) * 0.0075 + 0.075;
    } else if (dB < -30.0) {
        fDef = (dB + 40.0) * 0.015 + 0.15;
    } else if (dB < -20.0) {
        fDef = (dB + 30.0) * 0.02 + 0.3;
    } else { // if (dB < 0.0)
        fDef = (dB + 20.0) * 0.025 + 0.5;
}
    
    return fDef;
}

//===----------------------------------------------------------------------===//
// Component
//===----------------------------------------------------------------------===//

void VolumeComponent::paint(Graphics &g)
{
    if (this->volumeAnalyzer == nullptr)
    {
        return;
    }
    
    const int w = float(this->getWidth());
    const int h = float(this->getHeight());
    
    Image img(Image::ARGB, this->getWidth(), this->getHeight(), true);
    
    {
        Graphics g1(img);
        
        this->peakBand.setValue(this->volumeAnalyzer->getPeak(this->channel));
        const float left = (this->orientation == Left) ? 0.f : w;
        const float right = w - left;
        this->peakBand.drawBand(g1, left, right, h);
    }
    
    ColourGradient cg(Colours::red, 0.f, 0.f,
                      Colours::white.withAlpha(0.2f), 0.f, h,
                      false);
    
    cg.addColour(0.05f, Colours::red);
    cg.addColour(0.06f, Colours::yellow);
    cg.addColour(0.21f, Colours::white.withAlpha(0.2f));
    
    FillType brush3(cg);
    g.setFillType(brush3);
    
    g.drawImageAt(img, 0, 0, true);
}

//===----------------------------------------------------------------------===//
// Volume Band
//===----------------------------------------------------------------------===//

VolumeComponent::Band::Band(VolumeComponent *parent) :
    meter(parent),
    value(0.0f),
    valueHold(0.0f),
    valueDecay(HQ_METER_DECAY_RATE1),
    peak(0.0f),
    peakHold(0.0f),
    peakDecay(HQ_METER_DECAY_RATE2)
{
}

void VolumeComponent::Band::setValue(float value)
{
    this->value = value;
}

void VolumeComponent::Band::reset()
{
    this->value = 0.0f;
    this->valueHold = 0.0f;
    this->valueDecay = HQ_METER_DECAY_RATE1;
    this->peak = 0.0f;
    this->peakHold = 0.0f;
    this->peakDecay = HQ_METER_DECAY_RATE2;
}

inline void VolumeComponent::Band::drawBand(Graphics &g, float left, float right, float height)
{
    g.setColour(Colours::white);
    
    const float vauleInDb = jlimit(HQ_METER_MINDB, HQ_METER_MAXDB, 20.0f * AudioCore::fastLog10(this->value));
    float valueInY = float(this->meter->iecLevel(vauleInDb) * height);
    
    if (this->valueHold < valueInY)
    {
        this->valueHold = valueInY;
        this->valueDecay = HQ_METER_DECAY_RATE1;
    }
    else
    {
        this->valueHold = int(this->valueHold * this->valueDecay);
        
        if (this->valueHold < valueInY)
        {
            this->valueHold = valueInY;
        }
        else
        {
            this->valueDecay = this->valueDecay * this->valueDecay;
            valueInY = this->valueHold;
        }
    }
    
    if (this->peak < valueInY)
    {
        this->peak = valueInY;
        this->peakHold = 0.0;
        this->peakDecay = HQ_METER_DECAY_RATE2;
    }
    else if (++this->peakHold > HQ_METER_CYCLES_BEFORE_PEAK_FALLOFF)
    {
        this->peak = this->peak * this->peakDecay;
        this->peakDecay = this->peakDecay * this->peakDecay;
    }

    const float peakHeight = 4.f;
    const float levelHeight = 8.f;
    const float levelY = jmax(0.f, (height - valueInY + peakHeight));

    Path levelTriangle;
    levelTriangle.startNewSubPath(left, levelY);
    levelTriangle.lineTo(right, levelY);
    levelTriangle.lineTo(right, levelY + 1.5f);
    levelTriangle.lineTo(left, levelY + levelHeight);
    levelTriangle.closeSubPath();
    g.fillPath(levelTriangle);

    const float peakY = jmax(0.f, (height - this->peak));
    g.fillRect(0.f, peakY, std::abs(right - left), peakHeight);
}
