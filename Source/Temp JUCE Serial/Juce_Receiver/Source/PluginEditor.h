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
class Juce_ReceiverAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    Juce_ReceiverAudioProcessorEditor (Juce_ReceiverAudioProcessor&);
    ~Juce_ReceiverAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Juce_ReceiverAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Juce_ReceiverAudioProcessorEditor)
};
