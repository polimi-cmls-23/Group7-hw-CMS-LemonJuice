/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

using namespace juce;

//==============================================================================
ColorMixerAudioProcessor::ColorMixerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
                    #endif
                    {
                        loadSong("You_Make_Me_Feel");
                        setLoopEnd(1);
                        setPlayHeadStart(0);
                        setLoopStart(0.1);
                        setLoopEnd(0.2);
                    }

ColorMixerAudioProcessor::~ColorMixerAudioProcessor()
{
}

//==============================================================================
const juce::String ColorMixerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ColorMixerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ColorMixerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ColorMixerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ColorMixerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ColorMixerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ColorMixerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ColorMixerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ColorMixerAudioProcessor::getProgramName (int index)
{
    return {};
}

void ColorMixerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ColorMixerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void ColorMixerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ColorMixerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void ColorMixerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's

    auto* leftChannel = buffer.getWritePointer(0);
    auto* rightChannel = buffer.getWritePointer(1);
    int bufferSize = buffer.getNumSamples();
    int ntracks = trackBuffers.size();
    if(isPlaying)
    {
    for( int sample=0; sample < bufferSize; ++sample)
       {
        for( int n = 0; n < ntracks; n++ )
           {
            if( playHead >= loopEnd ) playHead = loopStart;
                   
            leftChannel[sample] += trackVolumes[n] * (trackBuffers[n]->getSample(0, playHead));
            rightChannel[sample] += trackVolumes[n] * (trackBuffers[n]->getSample(1, playHead));
           }
           playHead++;
        }
    }
}



//==============================================================================
bool ColorMixerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ColorMixerAudioProcessor::createEditor()
{
    return new ColorMixerAudioProcessorEditor (*this);
}

//==============================================================================
void ColorMixerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ColorMixerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ColorMixerAudioProcessor();
}

void ColorMixerAudioProcessor::loadSong(const juce::String& song)
{
    // Clear the existing track buffers
    trackBuffers.clear();
    trackVolumes.clear();
    
    // Get the directory containing the song files
//    juce::File songsDirectory = File::getCurrentWorkingDirectory().getChildFile("Resources").getChildFile("Songs").getChildFile(song);
    
    juce::File songDirectory = juce::File::getSpecialLocation(juce::File::currentApplicationFile).getChildFile("Contents").getChildFile("Resources").getChildFile("Songs").getChildFile(song);
    
    // Iterate through each .wav file and load it into a track buffer
    for (DirectoryEntry entry : RangedDirectoryIterator (songDirectory, true))
    {
        
        juce::File file(entry.getFile());
        if(file.getFileExtension()==".wav")
        {
            juce::AudioFormatManager formatManager;
            formatManager.registerBasicFormats();
            
            // Load the file into an AudioSampleBuffer
            juce::AudioSampleBuffer buffer;
            loadFileIntoBuffer(file, buffer);
            
            // Add the buffer to the trackBuffers array
            trackBuffers.add(new juce::AudioSampleBuffer(buffer));
            trackVolumes.push_back(0.5);
            trackLength = jmax(trackLength, buffer.getNumSamples());
        }
    }
}

void ColorMixerAudioProcessor::loadFileIntoBuffer(juce::File& file, juce::AudioSampleBuffer& buffer)
{
    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();

    // Create an AudioFormatReader for the file
    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(file));

    if (reader != nullptr)
    {
        // Set the buffer size based on the number of channels and samples in the file
        int numChannels = reader->numChannels;
        int numSamples = reader->lengthInSamples;
        buffer.setSize(numChannels, numSamples, false, true, true);

        // Read the audio data from the file into the buffer
        reader->read(&buffer, 0, numSamples, 0, true, true);
    }
}


void ColorMixerAudioProcessor::setTrackVolume(int n_track , float volume)
{
    trackVolumes[n_track] = volume;
}

void ColorMixerAudioProcessor::setPlayHeadStart(float position)
{
    playHead = floorf(trackLength * position);
}

void ColorMixerAudioProcessor::setLoopEnd(float position)
{
    loopEnd = floorf(trackLength * position);
}

void ColorMixerAudioProcessor::setLoopStart(float position)
{
    loopStart = floorf(trackLength * position);
}
