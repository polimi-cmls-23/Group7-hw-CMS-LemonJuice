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
                            
                        }
ColorMixerAudioProcessorEditor* editor;

ColorMixerAudioProcessor::~ColorMixerAudioProcessor()
{
    // Clear the trackBuffers map
        for (auto& pair : trackBuffers)
        {
            delete pair.second; // Delete each AudioSampleBuffer object
        }
        trackBuffers.clear(); // Clear the map

        // Clear the trackVolumes vector
        trackVolumes.clear();

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
    trackVolumes.reserve(nrTracks);

    
    //JUCE NEED TO BE STARTED BEFORE PROCESSING
    //connect to port 6448 with address /color
    int port = 8080;
    if(!OSCReceiver::connect(port)){
        std::cout << "Could not connect to the port " << port << "\n";
    }else std::cout << "Connected to port " << port << "\n";
    OSCReceiver::addListener(this, "/color");
    
     
}


void ColorMixerAudioProcessor::oscMessageReceived (const juce::OSCMessage &message)
{
    std::cout << "Values received ";

    if (message.size() == 3+nrTracks)
    {
        // Retrieve the values for r, g, and b from the message
        int r = message[0].getInt32();
        int g = message[1].getInt32();
        int b = message[2].getInt32();
        std::cout << r << " " << g << " " << b << std::endl;
        
        
        for( int i=0; i < nrTracks; i++){
        
            // Update the trackVolumes array with the values from the message
            trackVolumes[i] = message[3+i].getFloat32();
        }
        
    }
    
    // Create a juce array to store the track volumes
    juce::Array<float> volumesArray;
    
    for (int i=0; i<nrTracks; i++) {
       
        // Iterate through the trackVolumes array and add the values to the volumesArray
        float val = trackVolumes[i];
        volumesArray.add(val);
    }

    if(editor != nullptr){
        // Set the volume array in the editor
        editor->setVolumeArray(volumesArray);
    }
        
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
    // Scoped object to handle denormalized numbers
    juce::ScopedNoDenormals noDenormals;
    
    // Get the total number of input channels
    auto totalNumInputChannels  = getTotalNumInputChannels();
    
    // Get the total number of output channels
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, clear any additional output channels
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    
    // Get pointers to the left and right audio channels in the buffer
    auto* leftChannel = buffer.getWritePointer(0);
    auto* rightChannel = buffer.getWritePointer(1);
    
    // Get the buffer size
    int bufferSize = buffer.getNumSamples();
    
    // Get the number of tracks
    int ntracks = trackBuffers.size();
    
    // If the audio is stopped, set the playHead to the loop start position
    if(isStopped) playHead = loopStart;
    
    // If the audio is playing
    if(isPlaying)
    {
        // Iterate over each sample in the buffer
        for( int sample=0; sample < bufferSize; ++sample)
        {
            // Iterate over each track
            for( int n = 0; n < ntracks; n++ )
                {
                    // If the playHead exceeds the loop end, wrap it around to the loop start
                    if( playHead >= loopEnd) playHead = loopStart;
        
                    // Read the sample from the track buffer and add it to the left and right channels
                    leftChannel[sample] += trackVolumes[n] * (trackBuffers[n]->getSample(0, playHead));
                    rightChannel[sample] += trackVolumes[n] * (trackBuffers[n]->getSample(1, playHead));
                }
            // Increment the playHead
            playHead++;
        }
        
        // Calculate the current playHead position in minutes
        float currentPlayHead = (playHead/44100)/60.0;
            
        // If an editor exists and the current playHead position differs from the editor's position, update the editor
        if(editor!=nullptr){
            if(editor->tempPosDiffers(currentPlayHead)) editor->setCurrent(currentPlayHead);
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
    // Create a new instance of ColorMixerAudioProcessorEditor
    editor = new ColorMixerAudioProcessorEditor(*this);
    return editor;

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

void ColorMixerAudioProcessor::loadSong(juce::String& song)
{
    // Clear the existing track buffers
    trackBuffers.clear();
    trackVolumes.clear();
    
    std::cout << "Loading folder:  " << song << std::endl;
    
    // Get the directory containing the song files
    juce::File songDirectory = juce::File::getSpecialLocation(juce::File::currentApplicationFile).getChildFile("Contents").getChildFile("Resources").getChildFile("Songs").getChildFile(song);
    
    int index = 0;
    
    // Iterate through each .wav file and load it into a track buffer
    for (DirectoryEntry entry : RangedDirectoryIterator (songDirectory, true))
    {
        juce::File file(entry.getFile());
       
        if(file.getFileExtension()==".wav")
        {
            // Create an AudioFormatManager and register basic audio formats
            juce::AudioFormatManager formatManager;
            formatManager.registerBasicFormats();
            
            //Use a defined order for the multitracks, so that you can access the right tracks with indexes instead of names
            index = file.getFileName().replace(".wav", "").getIntValue();
            
            
            
            // Load the file into an AudioSampleBuffer
            juce::AudioSampleBuffer buffer;
            loadFileIntoBuffer(file, buffer);
        
            
            
            // Add the buffer to the trackBuffers array
            trackBuffers.insert(std::make_pair(index, new juce::AudioSampleBuffer(buffer)));
            
            // Set an initial volume for the track
            trackVolumes[index]=0.5;
            
            // Update the track length
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
        numSamples = reader->lengthInSamples;
        buffer.setSize(numChannels, numSamples, false, true, true);

        // Read the audio data from the file into the buffer
        reader->read(&buffer, 0, numSamples, 0, true, true);
    }
    
    // Calculate the duration of the song in seconds
    durationInSeconds = ((numSamples)/(44100));
    
   
    
}

void ColorMixerAudioProcessor::sendSeconds(){
    
    // Set the duration of the song in the Editor
    if(editor != nullptr){
        editor->setSeconds(durationInSeconds);
        
    }
    
}


void ColorMixerAudioProcessor::setTrackVolume(int n_track , float volume)
{
    trackVolumes[n_track]=volume;
    
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

void ColorMixerAudioProcessor::setIsPlaying(bool playing){
    isPlaying = playing;
   
        
}

void ColorMixerAudioProcessor::setIsLooping(bool looping){
    isLooping = looping;
   
}

void ColorMixerAudioProcessor::setIsStopped(bool stopping){
    isStopped = stopping;
}

void ColorMixerAudioProcessor::setSongArray(juce::String selectedSong){
    songName = selectedSong;

    loadSong(songName);

}
