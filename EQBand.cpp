/*
  ==============================================================================

    EQBand.cpp
    Created: 13 Nov 2023 12:25:55am
    Author:  tatao

  ==============================================================================
*/

#include "EQBand.h"

// Constructor
EQBand::EQBand()
{
    addAndMakeVisible(bandSlider);
    bandSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    bandSlider.setRange(0.01, 2.0);
    bandSlider.setValue(1.0);
    bandSlider.addListener(this);
    bandSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, true, 72, 32);
    bandSlider.setRange(0.01, 2.0, 0.01);
    gain = 1.0f;
}

EQBand::~EQBand()
{
    bandSlider.removeListener(this);
    gainAttachment = nullptr;
}


void EQBand::resized()
{
    bandSlider.setBounds(getLocalBounds());
}

void EQBand::initializeVTS(juce::AudioProcessorValueTreeState& vts, const juce::String& gainID)
{
    gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(vts, gainID, bandSlider);
}

void EQBand::prepare(float frequency, int sampleRate, float gain)
{
    this->frequency = frequency;
    this->sampleRate = sampleRate;
    this->gain = gain;

    juce::IIRFilter peakFilter;
    auto coefficients = juce::IIRCoefficients::makePeakFilter(this->sampleRate, this->frequency, 1.0f, this->gain);
    peakFilter.setCoefficients(coefficients);
    filter.push_back(peakFilter);
}

void EQBand::process(juce::AudioBuffer<float>& buffer)
{
    int size = filter.size();
    numChannels = buffer.getNumChannels();

    if (size != numChannels)
    {
        filter.clear();
        for (int i = 0; i < numChannels; i += 1)
        {
            prepare(frequency, sampleRate, gain);
        }
    }

    int numSamples = buffer.getNumSamples();

    // Process the audio input using the filter(s) and the processSample() method of the filter
    for (int channel = 0; channel < numChannels; channel += 1)
    {
        auto* channelData = buffer.getWritePointer(channel);
        filter[channel].processSamples(channelData, numSamples);
    }
}

void EQBand::reset()
{
    for (int i = 0; i < filter.size(); i += 1)
    {
        filter[i].reset();
    }
}

void EQBand::sliderValueChanged(juce::Slider* slider)
{
    gain = slider->getValue();
    prepare(frequency, sampleRate, gain);
}

juce::Label* EQBand::getBandLabel()
{
    return &bandLabel;
}


int EQBand::getNumChannels()
{
    return numChannels;
}


int EQBand::getSampleRate()
{
    return sampleRate;
}

float EQBand::getFrequency()
{
    return frequency;
}


float EQBand::getGain()
{
    return gain;
}

void EQBand::setNumChannels(int num)
{
    numChannels = num;
}

void EQBand::setSampleRate(int sRate)
{
    sampleRate = sRate;
}

void EQBand::setFrequency(float freq)
{
    frequency = freq;
}

void EQBand::setGain(float newGain)
{
    gain = newGain;
}

float EQBand::getSliderValue()
{
    return bandSlider.getValue();
}

juce::Slider* EQBand::getSlider()
{
    return &bandSlider;
}