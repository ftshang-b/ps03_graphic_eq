/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PS03_GraphicEqualizerAudioProcessorEditor::PS03_GraphicEqualizerAudioProcessorEditor (PS03_GraphicEqualizerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    setSize(920, 380);
    addAndMakeVisible(pluginLabel);
    pluginLabel.setText("Band Center Frequencies", juce::dontSendNotification);
    pluginLabel.setJustificationType(juce::Justification::centred);

    for (int bandNum = 0; bandNum < 10; bandNum += 1)
    {
        addAndMakeVisible(audioProcessor.eqBands[bandNum]);
        addAndMakeVisible(audioProcessor.eqBands[bandNum].getBandLabel());
        juce::String bandFreq(audioProcessor.bandFrequencies[bandNum]);
        audioProcessor.eqBands[bandNum].getBandLabel()->setText(bandFreq, juce::dontSendNotification);
        audioProcessor.eqBands[bandNum].getBandLabel()->setJustificationType(juce::Justification::horizontallyCentred);
    }
}

PS03_GraphicEqualizerAudioProcessorEditor::~PS03_GraphicEqualizerAudioProcessorEditor()
{
}

//==============================================================================
void PS03_GraphicEqualizerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void PS03_GraphicEqualizerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    pluginLabel.setBounds(0, 10, 920, 36);

    int spacing = 90;
    for (int i = 0; i < 10; i += 1)
    {
        audioProcessor.eqBands[i].setBounds(10 + i * spacing, 80, spacing, 250);
        audioProcessor.eqBands[i].getBandLabel()->setBounds(18 + i * spacing, 50, 72, 36);
    }
}
