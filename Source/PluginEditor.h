/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MuSnetSenderReceiverAudioProcessorEditor  : public AudioProcessorEditor {
public:
  MuSnetSenderReceiverAudioProcessorEditor (MuSnetSenderReceiverAudioProcessor&);
  ~MuSnetSenderReceiverAudioProcessorEditor();

  //==============================================================================
  void paint (Graphics&) override;
  void resized() override;

private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  MuSnetSenderReceiverAudioProcessor& processor;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MuSnetSenderReceiverAudioProcessorEditor)

  Label ipInfoLabel;
  Label ipInputLabel;
  TextButton connButton;
  Slider sendReceiveSlider;
  Label sendReceiveLabel;
  String lastIP;

public:
  std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> sendReceiveValue;

};
