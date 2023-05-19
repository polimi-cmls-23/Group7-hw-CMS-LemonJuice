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
class ColorMixerAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ColorMixerAudioProcessorEditor (ColorMixerAudioProcessor&);
    ~ColorMixerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    void fillRectWithColor(juce::Graphics& g,const juce::Rectangle<int>& rect, juce::Colour color);
    
    ColorMixerAudioProcessor& audioProcessor;
    
    juce::TextButton playButton;
    juce::TextButton loopButton;
    juce::TextButton button3;
    
    juce::TableListBox tableListBox;
    juce::Colour circleColor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ColorMixerAudioProcessorEditor)
};
