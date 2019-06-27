/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MuSnetSenderReceiverAudioProcessorEditor::MuSnetSenderReceiverAudioProcessorEditor (MuSnetSenderReceiverAudioProcessor& p)
  : AudioProcessorEditor (&p), processor (p), lastIP("0.0.0.0") {
  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize (195, 300);

  // *******************

  ipInfoLabel.setBounds(10, 10, 75, 25);
  ipInfoLabel.setText("Server IP:", dontSendNotification);
  ipInfoLabel.setJustificationType(Justification::left);
  addAndMakeVisible(ipInfoLabel);

  ipInputLabel.setBounds(85, 10, 100, 25);
  ipInputLabel.setText(processor.lastIP(), dontSendNotification);
  ipInfoLabel.setJustificationType(Justification::right);
  ipInputLabel.setEditable(true);
  addAndMakeVisible(ipInputLabel);

  // *******************

  String connStatus = processor.currentStatus() == CONNECTED ? "Disconnect" : "Connect";
  connButton.setBounds(10, 40, 175, 25);
  connButton.setButtonText(connStatus);
  connButton.onClick = [this] {
    if (processor.currentStatus() == DISCONNECTED) {
      String s = ipInputLabel.getText();
      processor.updateIP(s);
      int res = processor.connect(s);
      if (!(res)) {
        connButton.setButtonText("Disconnect");
      }
    } else {
      int res = processor.disconnect();
      if (!(res)) {
        connButton.setButtonText("Connect");
      }
    }
  };
  addAndMakeVisible(connButton);

  // *******************

  sendReceiveValue = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.parameters, "sendReceive", sendReceiveSlider);

  sendReceiveSlider.setBounds(25, 75, 145, 145);
  sendReceiveSlider.setSliderStyle(Slider::Rotary);
  sendReceiveSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 30);
  addAndMakeVisible(sendReceiveSlider);

  sendReceiveLabel.setBounds(25, 230, 145, 25);
  sendReceiveLabel.setText("Send vs. Receive", dontSendNotification);
  sendReceiveLabel.setJustificationType(Justification::centred);
  addAndMakeVisible(sendReceiveLabel);

}

MuSnetSenderReceiverAudioProcessorEditor::~MuSnetSenderReceiverAudioProcessorEditor() {
}

//==============================================================================
void MuSnetSenderReceiverAudioProcessorEditor::paint (Graphics& g) {
  // (Our component is opaque, so we must completely fill the background with a solid colour)
  g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

  g.setColour (Colours::white);
  g.setFont (15.0f);
  //g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void MuSnetSenderReceiverAudioProcessorEditor::resized() {
  // This is generally where you'll want to lay out the positions of any
  // subcomponents in your editor..
}
