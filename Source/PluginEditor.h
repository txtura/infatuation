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
class InfatuationAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    InfatuationAudioProcessorEditor (InfatuationAudioProcessor&);
    ~InfatuationAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    InfatuationAudioProcessor& audioProcessor;
    
    juce::Slider mDryWetSlider;
    juce::Slider mDepthSlider;
    juce::Slider mRateSlider;
    juce::Slider mPhaseOffsetSlider;
    juce::Slider mFeedbackSlider;
    juce::Slider mDelayTimeSlider;
    juce::Slider mNoiseLevelSlider;
    
    juce::ComboBox mType;
    juce::ComboBox mWaveType;
    juce::ComboBox mNoiseType;
    
    juce::Label mDryWetLabel;
    juce::Label mDepthLabel;
    juce::Label mRateLabel;
    juce::Label mPhaseOffsetLabel;
    juce::Label mFeedbackLabel;
    juce::Label mDelayTimeLabel;
    juce::Label mNoiseLevelLabel;
    juce::Label mTypeLabel;
    juce::Label mWaveTypeLabel;
    juce::Label mNoiseTypeLabel;
    
    int mSliderID;
    int mWaveSliderID;
    int mNoiseSliderID;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InfatuationAudioProcessorEditor)
};
