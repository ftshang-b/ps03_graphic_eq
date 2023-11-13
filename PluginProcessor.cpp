/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PS03_GraphicEqualizerAudioProcessor::PS03_GraphicEqualizerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), parameters(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
}

PS03_GraphicEqualizerAudioProcessor::~PS03_GraphicEqualizerAudioProcessor()
{
}

//==============================================================================
const juce::String PS03_GraphicEqualizerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PS03_GraphicEqualizerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PS03_GraphicEqualizerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PS03_GraphicEqualizerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PS03_GraphicEqualizerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PS03_GraphicEqualizerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PS03_GraphicEqualizerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PS03_GraphicEqualizerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String PS03_GraphicEqualizerAudioProcessor::getProgramName (int index)
{
    return {};
}

void PS03_GraphicEqualizerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void PS03_GraphicEqualizerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    for (int bandNum = 0; bandNum < 10; bandNum += 1)
    {
        eqBands[bandNum].prepare(bandFrequencies[bandNum], sampleRate, 1.0f);
        eqBands[bandNum].initializeVTS(parameters, gainIDs[bandNum]);
    }
}

void PS03_GraphicEqualizerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PS03_GraphicEqualizerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void PS03_GraphicEqualizerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int bandNum = 0; bandNum < 10; bandNum += 1)
    {
        eqBands[bandNum].process(buffer);
    }
}

//==============================================================================
bool PS03_GraphicEqualizerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PS03_GraphicEqualizerAudioProcessor::createEditor()
{
    return new PS03_GraphicEqualizerAudioProcessorEditor (*this);
}

//==============================================================================
void PS03_GraphicEqualizerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::MemoryOutputStream stream(destData, false);
    parameters.state.writeToStream(stream);
}

void PS03_GraphicEqualizerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    auto tree = juce::ValueTree::readFromData(data, size_t(sizeInBytes));

    if (tree.isValid())
    {
        parameters.replaceState(tree);

        for (int i = 0; i < 10; i += 1)
        {
            DBG("Parameter Value of " << gainIDs[i] << ": " << *parameters.getRawParameterValue(gainIDs[i]));
            eqBands[i].prepare(bandFrequencies[i], getSampleRate(), *parameters.getRawParameterValue(gainIDs[i]));
            DBG("Gain Value of " << i <<  ": " << eqBands[i].getGain() << "\tSlider Value : " << eqBands[i].getSliderValue());
            eqBands[i].getSlider()->setValue(*parameters.getRawParameterValue(gainIDs[i]));
            DBG("Gain Value of " << i << ": " << eqBands[i].getGain() << "\tSlider Value : " << eqBands[i].getSliderValue());
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PS03_GraphicEqualizerAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout PS03_GraphicEqualizerAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>>params;
    for (int bandNum = 0; bandNum < 10; bandNum += 1)
    {
        juce::String id(juce::String(bandFrequencies[bandNum]) + "gain");
        juce::String name(juce::String(bandFrequencies[bandNum]) + "GainValue");
        auto pGain = std::make_unique<juce::AudioParameterFloat>(id, name, 0.01f, 2.0f, 1.0f);
        params.push_back(std::move(pGain));
        gainIDs.push_back(id);
    }

    return { params.begin(), params.end() };

}