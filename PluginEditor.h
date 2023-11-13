/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class PS03_GraphicEqualizerAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    PS03_GraphicEqualizerAudioProcessorEditor (PS03_GraphicEqualizerAudioProcessor&);
    ~PS03_GraphicEqualizerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PS03_GraphicEqualizerAudioProcessor& audioProcessor;
    juce::Label pluginLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PS03_GraphicEqualizerAudioProcessorEditor)
};
