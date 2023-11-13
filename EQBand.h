/*
  ==============================================================================

    EQBand.h
    Created: 13 Nov 2023 12:25:55am
    Author:  tatao

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class EQBand : public juce::Component, juce::Slider::Listener
{
public:
    EQBand();
    ~EQBand();
    void resized() override;

    void initializeVTS(juce::AudioProcessorValueTreeState& vts, const juce::String& gainID);
    void prepare(float frequency, int sampleRate, float gain);
    void process(juce::AudioBuffer<float>& buffer);
    void reset();
    void sliderValueChanged(juce::Slider* slider) override;

    juce::Label* getBandLabel();
    juce::Slider* getSlider();
    int getNumChannels();
    int getSampleRate();
    float getFrequency();
    float getGain();

    void setNumChannels(int num);
    void setSampleRate(int sRate);
    void setFrequency(float freq);
    void setGain(float newGain);

    float getSliderValue();

private:
    std::vector<juce::IIRFilter> filter;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;
    juce::Label bandLabel;
    juce::Slider bandSlider;
    int numChannels, sampleRate;
    float frequency, gain;
};