/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MuSnetSenderReceiverAudioProcessor::MuSnetSenderReceiverAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
  : AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                    .withInput  ("Input",  AudioChannelSet::stereo(), true)
#endif
                    .withOutput ("Output", AudioChannelSet::stereo(), true)
#endif
                   ),
    parameters(*this, nullptr, "MuSnetSenderReceiverParams", createParameters()),
#endif
    myManager(),
    myStatus(DISCONNECTED),
    myLastIP("0.0.0.0") {
  sendReceive = parameters.getRawParameterValue("sendReceive");
}

MuSnetSenderReceiverAudioProcessor::~MuSnetSenderReceiverAudioProcessor() {
}

//==============================================================================
const String MuSnetSenderReceiverAudioProcessor::getName() const {
  return JucePlugin_Name;
}

bool MuSnetSenderReceiverAudioProcessor::acceptsMidi() const {
#if JucePlugin_WantsMidiInput
  return true;
#else
  return false;
#endif
}

bool MuSnetSenderReceiverAudioProcessor::producesMidi() const {
#if JucePlugin_ProducesMidiOutput
  return true;
#else
  return false;
#endif
}

bool MuSnetSenderReceiverAudioProcessor::isMidiEffect() const {
#if JucePlugin_IsMidiEffect
  return true;
#else
  return false;
#endif
}

double MuSnetSenderReceiverAudioProcessor::getTailLengthSeconds() const {
  return 0.0;
}

int MuSnetSenderReceiverAudioProcessor::getNumPrograms() {
  return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
  // so this should be at least 1, even if you're not really implementing programs.
}

int MuSnetSenderReceiverAudioProcessor::getCurrentProgram() {
  return 0;
}

void MuSnetSenderReceiverAudioProcessor::setCurrentProgram (int index) {
}

const String MuSnetSenderReceiverAudioProcessor::getProgramName (int index) {
  return {};
}

void MuSnetSenderReceiverAudioProcessor::changeProgramName (int index, const String& newName) {
}

//==============================================================================
void MuSnetSenderReceiverAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock) {
  // Use this method as the place to do any pre-playback
  // initialisation that you need..

}

void MuSnetSenderReceiverAudioProcessor::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
  if (myStatus == CONNECTED) {
    int res = myManager.disconnect();
    if (!(res)) {
      myStatus = DISCONNECTED;
    }
  }

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MuSnetSenderReceiverAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const {
#if JucePlugin_IsMidiEffect
  ignoreUnused (layouts);
  return true;
#else
  // This is the place where you check if the layout is supported.
  // In this template code we only support mono or stereo.
  if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
      && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void MuSnetSenderReceiverAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages) {
  ScopedNoDenormals noDenormals;
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

  // This is the place where you'd normally do the guts of your plugin's
  // audio processing...
  // Make sure to reset the state if your inner loop is processing
  // the samples and the outer loop is handling the channels.
  // Alternatively, you can process the samples with the channels
  // interleaved by keeping the same state.

  auto* leftChan = buffer.getWritePointer(0);
  auto* rightChan = buffer.getWritePointer(1);

  if (buffer.getNumSamples() == AUDIO_VECTOR_SIZE) {
    AudioVector v(AUDIO_VECTOR_SIZE * NUM_CHANNELS, 0);

    if (myStatus == CONNECTED) {
      for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
        v[sample * 2] = static_cast<AudioSample>(floor(leftChan[sample] * MAX_INT16)); // Interleaved buffer content...
        v[sample * 2 + 1] = static_cast<AudioSample>(floor(rightChan[sample] * MAX_INT16));
      }
      myManager.sendStream(v);
      v = myManager.retrieveStream();
    }

    for (int sample = 0; sample < buffer.getNumSamples(); sample++) {
      leftChan[sample] = leftChan[sample] * (1.0 - *sendReceive) + (v[sample * 2] / static_cast<double>(MAX_INT16)) * *sendReceive;
      rightChan[sample] = rightChan[sample] * (1.0 - *sendReceive) + (v[sample * 2 + 1] / static_cast<double>(MAX_INT16)) * *sendReceive;
    }
  }

}

//==============================================================================
bool MuSnetSenderReceiverAudioProcessor::hasEditor() const {
  return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* MuSnetSenderReceiverAudioProcessor::createEditor() {
  return new MuSnetSenderReceiverAudioProcessorEditor (*this);
}

//==============================================================================
void MuSnetSenderReceiverAudioProcessor::getStateInformation (MemoryBlock& destData) {
  // You should use this method to store your parameters in the memory block.
  // You could do that either as raw data, or use the XML or ValueTree classes
  // as intermediaries to make it easy to save and load complex data.
}

void MuSnetSenderReceiverAudioProcessor::setStateInformation (const void* data, int sizeInBytes) {
  // You should use this method to restore your parameters from this memory block,
  // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
  return new MuSnetSenderReceiverAudioProcessor();
}

// ********************
AudioProcessorValueTreeState::ParameterLayout MuSnetSenderReceiverAudioProcessor::createParameters() {
  std::vector<std::unique_ptr<RangedAudioParameter>> params;

  params.push_back(std::make_unique<AudioParameterFloat>("sendReceive", "Send vs. Receive", NormalisableRange<float>(0.0, 1.0, 0.01, 1.0), 0.5));

  return {params.begin(), params.end()};
}

// ********************
int MuSnetSenderReceiverAudioProcessor::connect(juce::String s) {
  if (myStatus == DISCONNECTED) {
    myManager.setIPAddress(s.toStdString());
    if (myManager.connect() < 0) {
      return -1;
    }
    myStatus = CONNECTED;
  }
  return 0;
}

int MuSnetSenderReceiverAudioProcessor::disconnect() {
  if (myStatus == CONNECTED) {
    if (myManager.disconnect() < 0) {
      return -1;
    }
    myStatus = DISCONNECTED;
  }
  return 0;
}

Status MuSnetSenderReceiverAudioProcessor::currentStatus() {
  return myStatus;
}

String MuSnetSenderReceiverAudioProcessor::lastIP() {
  return myLastIP;
}

void MuSnetSenderReceiverAudioProcessor::updateIP(String ip) {
  myLastIP = ip;
}
