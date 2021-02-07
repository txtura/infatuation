/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#define MAX_DELAY_TIME 2

//==============================================================================
/**
*/
class InfatuationAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    InfatuationAudioProcessor();
    ~InfatuationAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    float linearInterpolation(float sample_r, float sample_x1, float inPhase);
    

private:
    
    juce::AudioParameterFloat* mDryWetParameter;
    juce::AudioParameterFloat* mDepthParameter;
    juce::AudioParameterFloat* mRateParameter;
    juce::AudioParameterFloat* mPhaseOffsetParameter;
    juce::AudioParameterFloat* mFeedbackParameter;
    juce::AudioParameterFloat* mDelayTimeParameter;
    juce::AudioParameterFloat* mNoiseLevelParameter;
    
    juce::AudioParameterInt* mTypeParameter;
    juce::AudioParameterInt* mTypeIDParam;
    juce::AudioParameterInt* mWaveTypeParameter;
    juce::AudioParameterInt* mWaveTypeIDParam;
    juce::AudioParameterInt* mNoiseTypeParameter;
    juce::AudioParameterInt* mNoiseTypeIDParam;
    
    float mDryWet;
    float mLFOPhase;
    
    float mFeedbackLeft;
    float mFeedbackRight;
    
    float mDelayReadHead;
    float mDelayTimeInSamples;
    float mDelayTimeSmoothed;

    int mCircularBufferLength;
    int mCircularBufferWriteHead;
    float* mCircularBufferLeft;
    float* mCircularBufferRight;
    
    juce::Random mRandom;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InfatuationAudioProcessor)
};
