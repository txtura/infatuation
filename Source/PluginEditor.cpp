/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
InfatuationAudioProcessorEditor::InfatuationAudioProcessorEditor (InfatuationAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (500, 400);
    
    auto& params = processor.getParameters();
    
    mDryWetLabel.setBounds(25, 25, 100, 30);
    mDryWetLabel.setText("Dry/Wet", juce::dontSendNotification);
    mDryWetLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible (mDryWetLabel);
    
    juce::AudioParameterFloat* dryWetParameter = (juce::AudioParameterFloat*)params.getUnchecked(0);
    
    mDryWetSlider.setBounds(25, 55, 100, 100);
    mDryWetSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mDryWetSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    mDryWetSlider.setRange(dryWetParameter->range.start, dryWetParameter->range.end);
    mDryWetSlider.setValue(*dryWetParameter);
    addAndMakeVisible(mDryWetSlider);
    
    mDryWetSlider.onValueChange = [this, dryWetParameter] { *dryWetParameter = mDryWetSlider.getValue(); };
    mDryWetSlider.onDragStart = [dryWetParameter] { dryWetParameter -> beginChangeGesture(); };
    mDryWetSlider.onDragEnd = [dryWetParameter] { dryWetParameter -> endChangeGesture(); };
    
    mDepthLabel.setBounds(125, 25, 100, 30);
    mDepthLabel.setText("Depth", juce::dontSendNotification);
    mDepthLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible (mDepthLabel);
    
    juce::AudioParameterFloat* depthParameter = (juce::AudioParameterFloat*)params.getUnchecked(1);
    
    mDepthSlider.setBounds(125, 55, 100, 100);
    mDepthSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mDepthSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    mDepthSlider.setRange(depthParameter->range.start, depthParameter->range.end);
    mDepthSlider.setValue(*depthParameter);
    addAndMakeVisible(mDepthSlider);
    
    mDepthSlider.onValueChange = [this, depthParameter] { *depthParameter = mDepthSlider.getValue(); };
    mDepthSlider.onDragStart = [depthParameter] { depthParameter -> beginChangeGesture(); };
    mDepthSlider.onDragEnd = [depthParameter] { depthParameter -> endChangeGesture(); };
    
    mRateLabel.setBounds(225, 25, 100, 30);
    mRateLabel.setText("Speed", juce::dontSendNotification);
    mRateLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible (mRateLabel);
    
    juce::AudioParameterFloat* rateParameter = (juce::AudioParameterFloat*)params.getUnchecked(2);
    
    mRateSlider.setBounds(225, 55, 100, 100);
    mRateSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mRateSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    mRateSlider.setRange(rateParameter->range.start, rateParameter->range.end);
    mRateSlider.setValue(*rateParameter);
    addAndMakeVisible(mRateSlider);
    
    mRateSlider.onValueChange = [this, rateParameter] { *rateParameter = mRateSlider.getValue(); };
    mRateSlider.onDragStart = [rateParameter] { rateParameter -> beginChangeGesture(); };
    mRateSlider.onDragEnd = [rateParameter] { rateParameter -> endChangeGesture(); };
    
    mPhaseOffsetLabel.setBounds(325, 25, 100, 30);
    mPhaseOffsetLabel.setText("Phase", juce::dontSendNotification);
    mPhaseOffsetLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible (mPhaseOffsetLabel);
    
    juce::AudioParameterFloat* phaseOffsetParameter = (juce::AudioParameterFloat*)params.getUnchecked(3);
    
    mPhaseOffsetSlider.setBounds(325, 55, 100, 100);
    mPhaseOffsetSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mPhaseOffsetSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    mPhaseOffsetSlider.setRange(phaseOffsetParameter->range.start, phaseOffsetParameter->range.end);
    mPhaseOffsetSlider.setValue(*phaseOffsetParameter);
    addAndMakeVisible(mPhaseOffsetSlider);
    
    mPhaseOffsetSlider.onValueChange = [this, phaseOffsetParameter] { *phaseOffsetParameter = mPhaseOffsetSlider.getValue(); };
    mPhaseOffsetSlider.onDragStart = [phaseOffsetParameter] { phaseOffsetParameter -> beginChangeGesture(); };
    mPhaseOffsetSlider.onDragEnd = [phaseOffsetParameter] { phaseOffsetParameter -> endChangeGesture(); };
    
    mFeedbackLabel.setText("Feedback", juce::dontSendNotification);
    mFeedbackLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible (mFeedbackLabel);
    
    juce::AudioParameterFloat* feedbackParameter = (juce::AudioParameterFloat*)params.getUnchecked(4);
    
    mFeedbackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mFeedbackSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    mFeedbackSlider.setRange(feedbackParameter->range.start, feedbackParameter->range.end);
    mFeedbackSlider.setValue(*feedbackParameter);
    addAndMakeVisible(mFeedbackSlider);
    
    mFeedbackSlider.onValueChange = [this, feedbackParameter] { *feedbackParameter = mFeedbackSlider.getValue(); };
    mFeedbackSlider.onDragStart = [feedbackParameter] { feedbackParameter -> beginChangeGesture(); };
    mFeedbackSlider.onDragEnd = [feedbackParameter] { feedbackParameter -> endChangeGesture(); };
    
    mTypeLabel.setText("Effect Type", juce::dontSendNotification);
    mTypeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible (mTypeLabel);
    
    juce::AudioParameterInt* typeParameter = (juce::AudioParameterInt*)params.getUnchecked(5);
    juce::AudioParameterInt* typeIDParam = (juce::AudioParameterInt*)params.getUnchecked(6);
    
    mType.addItem("Delay", 1);
    mType.addItem("Chorus", 2);
    mType.addItem("Flanger", 3);
    addAndMakeVisible(mType);
    
    mType.onChange = [this, typeParameter, typeIDParam] {
        typeParameter->beginChangeGesture();
        *typeParameter = mType.getSelectedItemIndex();
        *typeIDParam = mType.getSelectedId();
        mSliderID = mType.getSelectedId();
        typeParameter->endChangeGesture();
    };
    
    mType.setSelectedItemIndex(*typeParameter);
    
    mDelayTimeLabel.setBounds(125, 25, 100, 30);
    mDelayTimeLabel.setText("Delay Time", juce::dontSendNotification);
    mDelayTimeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible (mDelayTimeLabel);
    
    juce::AudioParameterFloat* delayTimeParameter = (juce::AudioParameterFloat*)params.getUnchecked(7);
    
    mDelayTimeSlider.setBounds(125, 55, 100, 100);
    mDelayTimeSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mDelayTimeSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    mDelayTimeSlider.setRange(delayTimeParameter->range.start, delayTimeParameter->range.end);
    mDelayTimeSlider.setValue(*delayTimeParameter);
    addAndMakeVisible(mDelayTimeSlider);
    
    mDelayTimeSlider.onValueChange = [this, delayTimeParameter] { *delayTimeParameter = mDelayTimeSlider.getValue(); };
    mDelayTimeSlider.onDragStart = [delayTimeParameter] { delayTimeParameter -> beginChangeGesture(); };
    mDelayTimeSlider.onDragEnd = [delayTimeParameter] { delayTimeParameter -> endChangeGesture(); };
    
    mNoiseLevelLabel.setText("Noise Level", juce::dontSendNotification);
    mNoiseLevelLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible (mNoiseLevelLabel);
    
    juce::AudioParameterFloat* noiseLevelParameter = (juce::AudioParameterFloat*)params.getUnchecked(8);
    
    mNoiseLevelSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    mNoiseLevelSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    mNoiseLevelSlider.setRange(noiseLevelParameter->range.start, noiseLevelParameter->range.end);
    mNoiseLevelSlider.setValue(*noiseLevelParameter);
    addAndMakeVisible(mNoiseLevelSlider);
    
    mNoiseLevelSlider.onValueChange = [this, noiseLevelParameter] { *noiseLevelParameter = mNoiseLevelSlider.getValue(); };
    mNoiseLevelSlider.onDragStart = [noiseLevelParameter] { noiseLevelParameter -> beginChangeGesture(); };
    mNoiseLevelSlider.onDragEnd = [noiseLevelParameter] { noiseLevelParameter -> endChangeGesture(); };
    
    mWaveTypeLabel.setText("Wave Type", juce::dontSendNotification);
    mWaveTypeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible (mWaveTypeLabel);
    
    juce::AudioParameterInt* waveTypeParameter = (juce::AudioParameterInt*)params.getUnchecked(9);
    juce::AudioParameterInt* waveTypeIDParam = (juce::AudioParameterInt*)params.getUnchecked(10);
    
    mWaveType.addItem("Sin", 1);
    mWaveType.addItem("Saw", 2);
    mWaveType.addItem("Triangle", 3);
    addAndMakeVisible(mWaveType);
    
    mWaveType.onChange = [this, waveTypeParameter, waveTypeIDParam] {
        waveTypeParameter->beginChangeGesture();
        *waveTypeParameter = mWaveType.getSelectedItemIndex();
        *waveTypeIDParam = mWaveType.getSelectedId();
        mWaveSliderID = mWaveType.getSelectedId();
        waveTypeParameter->endChangeGesture();
    };
    
    mWaveType.setSelectedItemIndex(*waveTypeParameter);
    
    mNoiseTypeLabel.setText("Noise Type", juce::dontSendNotification);
    mNoiseTypeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible (mNoiseTypeLabel);
    
    juce::AudioParameterInt* noiseTypeParameter = (juce::AudioParameterInt*)params.getUnchecked(11);
    juce::AudioParameterInt* noiseTypeIDParam = (juce::AudioParameterInt*)params.getUnchecked(12);
    
    mNoiseType.addItem("Type 1", 1);
    mNoiseType.addItem("Type 2", 2);
    mNoiseType.addItem("Type 3", 3);
    addAndMakeVisible(mNoiseType);
    
    mNoiseType.onChange = [this, noiseTypeParameter, noiseTypeIDParam] {
        noiseTypeParameter->beginChangeGesture();
        *noiseTypeParameter = mNoiseType.getSelectedItemIndex();
        *noiseTypeIDParam = mNoiseType.getSelectedId();
        mNoiseSliderID = mNoiseType.getSelectedId();
        noiseTypeParameter->endChangeGesture();
    };
    
    mNoiseType.setSelectedItemIndex(*noiseTypeParameter);
}

InfatuationAudioProcessorEditor::~InfatuationAudioProcessorEditor()
{
}

//==============================================================================
void InfatuationAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::skyblue);
    
    mDryWetLabel.setColour(juce::Label::textColourId, juce::Colours::dimgrey);
    mDepthLabel.setColour(juce::Label::textColourId, juce::Colours::dimgrey);
    mRateLabel.setColour(juce::Label::textColourId, juce::Colours::dimgrey);
    mPhaseOffsetLabel.setColour(juce::Label::textColourId, juce::Colours::dimgrey);
    mFeedbackLabel.setColour(juce::Label::textColourId, juce::Colours::dimgrey);
    mDelayTimeLabel.setColour(juce::Label::textColourId, juce::Colours::dimgrey);
    mNoiseLevelLabel.setColour(juce::Label::textColourId, juce::Colours::dimgrey);
    mWaveTypeLabel.setColour(juce::Label::textColourId, juce::Colours::dimgrey);
    mTypeLabel.setColour(juce::Label::textColourId, juce::Colours::dimgrey);
    mNoiseTypeLabel.setColour(juce::Label::textColourId, juce::Colours::dimgrey);
    
    getLookAndFeel().setColour (juce::Slider::thumbColourId, juce::Colours::steelblue);
    getLookAndFeel().setColour (juce::Slider::trackColourId, juce::Colours::lightpink);
    getLookAndFeel().setColour (juce::Slider::backgroundColourId, juce::Colours::whitesmoke);

   if (mSliderID == 2)
   {
       setSize (350, 420);
       
       mDryWetLabel.setVisible(true);
       mDepthLabel.setVisible(true);
       mRateLabel.setVisible(true);
       mPhaseOffsetLabel.setVisible(false);
       mFeedbackLabel.setVisible(false);
       mDelayTimeLabel.setVisible(false);
       mNoiseLevelSlider.setVisible(true);
       mWaveTypeLabel.setVisible(true);
       
       mDryWetSlider.setVisible(true);
       mDepthSlider.setVisible(true);
       mRateSlider.setVisible(true);
       mPhaseOffsetSlider.setVisible(false);
       mFeedbackSlider.setVisible(false);
       mDelayTimeSlider.setVisible(false);
       mNoiseLevelSlider.setVisible(true);
       mWaveType.setVisible(true);
       
       mTypeLabel.setBounds(60, 155, 100, 30);
       mType.setBounds(60, 185, 100, 30);
       
       mWaveTypeLabel.setBounds(190, 155, 100, 30);
       mWaveType.setBounds(190, 185, 100, 30);
       
       g.setColour (juce::Colours::pink);
       juce::Line<float> line1 (25, 245, 325, 245);
       g.drawLine (line1, 3.0f);

       mNoiseTypeLabel.setBounds(125, 270, 100, 30);
       mNoiseType.setBounds(125, 300, 100, 30);
       
       mNoiseLevelLabel.setBounds(125, 335, 100, 30);
       mNoiseLevelSlider.setBounds(25, 365, 300, 30);
       
   } else if (mSliderID == 3)
   {
       setSize (550, 380);
       g.setColour (juce::Colours::orangered);
       
       mDryWetLabel.setVisible(true);
       mDepthLabel.setVisible(true);
       mRateLabel.setVisible(true);
       mPhaseOffsetLabel.setVisible(true);
       mFeedbackLabel.setVisible(true);
       mDelayTimeLabel.setVisible(false);
       mWaveTypeLabel.setVisible(true);
       
       mDryWetSlider.setVisible(true);
       mDepthSlider.setVisible(true);
       mRateSlider.setVisible(true);
       mPhaseOffsetSlider.setVisible(true);
       mFeedbackSlider.setVisible(true);
       mDelayTimeSlider.setVisible(false);
       mNoiseLevelSlider.setVisible(true);
       mWaveType.setVisible(true);
       
       mFeedbackLabel.setBounds(425, 25, 100, 30);
       mFeedbackSlider.setBounds(425, 55, 100, 100);
       
       mWaveTypeLabel.setBounds(300, 155, 100, 30);
       mWaveType.setBounds(300, 185, 100, 30);
       
       mTypeLabel.setBounds(150, 155, 100, 30);
       mType.setBounds(150, 185, 100, 30);
       
       g.setColour (juce::Colours::pink);
       juce::Line<float> line2 (25, 245, 525, 245);
       g.drawLine (line2, 3.0f);
       
       mNoiseTypeLabel.setBounds(50, 270, 100, 30);
       mNoiseType.setBounds(50, 300, 100, 30);
       
       mNoiseLevelLabel.setBounds(200, 270, 300, 30);
       mNoiseLevelSlider.setBounds(200, 300, 300, 30);
       
   } else if (mSliderID == 1)
   {
       setSize (350, 420);
       g.setColour (juce::Colours::green);
       
       mDryWetLabel.setVisible(true);
       mDepthLabel.setVisible(false);
       mRateLabel.setVisible(false);
       mPhaseOffsetLabel.setVisible(false);
       mFeedbackLabel.setVisible(true);
       mDelayTimeLabel.setVisible(true);
       mWaveTypeLabel.setVisible(false);
       
       mDryWetSlider.setVisible(true);
       mDepthSlider.setVisible(false);
       mRateSlider.setVisible(false);
       mPhaseOffsetSlider.setVisible(false);
       mFeedbackSlider.setVisible(true);
       mDelayTimeSlider.setVisible(true);
       mNoiseLevelSlider.setVisible(true);
       mWaveType.setVisible(false);
       
       mFeedbackLabel.setBounds(225, 25, 100, 30);
       mFeedbackSlider.setBounds(225, 55, 100, 100);
       
       mTypeLabel.setBounds(125, 155, 100, 30);
       mType.setBounds(125, 185, 100, 30);
       
       g.setColour (juce::Colours::pink);
       juce::Line<float> line (25, 245, 325, 245);
       g.drawLine (line, 3.0f);
       
       mNoiseTypeLabel.setBounds(125, 270, 100, 30);
       mNoiseType.setBounds(125, 300, 100, 30);
       
       mNoiseLevelLabel.setBounds(125, 335, 100, 30);
       mNoiseLevelSlider.setBounds(25, 365, 300, 30);
   }
    g.setFont (15.0f);
}

void InfatuationAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
