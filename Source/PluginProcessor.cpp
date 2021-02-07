/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
InfatuationAudioProcessor::InfatuationAudioProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true))
{
    addParameter(mDryWetParameter = new juce::AudioParameterFloat("drywet", "Dry Wet", 0.0, 1.0, 0.5));
    addParameter(mDepthParameter = new juce::AudioParameterFloat("depth", "Depth", 0.0, 1.0, 0.5));
    addParameter(mRateParameter = new juce::AudioParameterFloat("rate", "Rate", 0.0, 1.0, 0.5));
    addParameter(mPhaseOffsetParameter = new juce::AudioParameterFloat("phaseoffset", "Phase Offset", 0.0, 1.0, 0.5));
    addParameter(mFeedbackParameter = new juce::AudioParameterFloat("feedback", "Feedback", 0, 0.98, 0.5));
    addParameter(mTypeParameter = new juce::AudioParameterInt("type", "Type", 0, 1, 0));
    addParameter(mTypeIDParam = new juce::AudioParameterInt("typeID", "TypeID", 1, 3, 1));
    addParameter(mDelayTimeParameter = new juce::AudioParameterFloat("delaytime", "Delay Time", 0.01, MAX_DELAY_TIME, 1.0));
    addParameter(mNoiseLevelParameter = new juce::AudioParameterFloat("noiselevel", "Noise Level", 0.00, 1.00, 0));
    addParameter(mWaveTypeParameter = new juce::AudioParameterInt("waveType", "Wave Type", 0, 1, 0));
    addParameter(mWaveTypeIDParam = new juce::AudioParameterInt("waveTypeID", "Wave TypeID", 1, 3, 1));
    addParameter(mNoiseTypeParameter = new juce::AudioParameterInt("noiseType", "Noise Type", 0, 1, 0));
    addParameter(mNoiseTypeIDParam = new juce::AudioParameterInt("noiseTypeID", "Noise TypeID", 1, 3, 1));
    
    mDelayTimeSmoothed = 0;
    mCircularBufferLeft = nullptr;
    mCircularBufferRight = nullptr;
    mCircularBufferWriteHead = 0;
    mCircularBufferLength = 0;
    mDelayTimeInSamples = 0;
    mDelayReadHead = 0;
    
    mFeedbackLeft = 0;
    mFeedbackRight = 0;
    
    mLFOPhase = 0;
    mDryWet = 0.5;
}

InfatuationAudioProcessor::~InfatuationAudioProcessor()
{
    if (mCircularBufferLeft != nullptr) {
        delete[] mCircularBufferLeft;
        mCircularBufferLeft = nullptr;
    }
    if (mCircularBufferRight != nullptr) {
        delete[] mCircularBufferRight;
        mCircularBufferRight = nullptr;
    }
}

//==============================================================================
const juce::String InfatuationAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool InfatuationAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool InfatuationAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool InfatuationAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double InfatuationAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int InfatuationAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int InfatuationAudioProcessor::getCurrentProgram()
{
    return 0;
}

void InfatuationAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String InfatuationAudioProcessor::getProgramName (int index)
{
    return {};
}

void InfatuationAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void InfatuationAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    
    mLFOPhase = 0;
    
    mDelayTimeInSamples = sampleRate * *mDelayTimeParameter;
    mCircularBufferLength = sampleRate * MAX_DELAY_TIME;
    
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    if (mCircularBufferLeft != nullptr ) {
      delete [] mCircularBufferLeft;
      mCircularBufferLeft = nullptr;
   }
    
    if (mCircularBufferLeft == nullptr ) {
        mCircularBufferLeft = new float[(int)mCircularBufferLength];
    }
    
    juce::zeromem(mCircularBufferLeft, mCircularBufferLength * sizeof(float));
    
    if (mCircularBufferRight != nullptr ) {
        delete [] mCircularBufferRight;
        mCircularBufferRight = nullptr;
    }
         
    if (mCircularBufferRight == nullptr ) {
        mCircularBufferRight = new float[(int)mCircularBufferLength];
    }
    
   juce:: zeromem(mCircularBufferRight, mCircularBufferLength * sizeof(float));
    
    mCircularBufferWriteHead = 0;
    mDelayTimeSmoothed = *mDelayTimeParameter;
}

void InfatuationAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool InfatuationAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainInputChannelSet() == juce::AudioChannelSet::stereo() &&
        layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo()) {
        return true;
    } else {
        return false;
    }
}

void InfatuationAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
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
    
    float* leftChannel = buffer.getWritePointer(0);
    float* rightChannel = buffer.getWritePointer(0);
    
    float noise;
    
    if (*mNoiseTypeIDParam == 1) {
        noise = (mRandom.nextFloat()) * *mNoiseLevelParameter;
    } else if (*mNoiseTypeIDParam == 2) {
        noise = (mRandom.nextFloat() * 2.0f - 1.0f) * *mNoiseLevelParameter;
    } else if (*mNoiseTypeIDParam == 3) {
        noise = (mRandom.nextFloat() * 0.5f - 0.25f) * *mNoiseLevelParameter;
    }
    
    for (int i = 0; i < buffer.getNumSamples(); i++) {
        
        // LFO/phase starts
        
        float delay_sample_left;
        float delay_sample_right;
        
        if ((*mTypeIDParam == 2) || (*mTypeIDParam == 3)) {
            
            float lfoOutLeft;
            if (*mWaveTypeIDParam == 1) {
                lfoOutLeft = sin(2*M_PI * mLFOPhase);
            } else if (*mWaveTypeIDParam == 2) {
                lfoOutLeft = 2*mLFOPhase - 1;
            } else if (*mWaveTypeIDParam == 3) {
                lfoOutLeft = 4*fabs(mLFOPhase - 0.5);
            }
            
            float lfoPhaseRight;
            
            if (*mTypeIDParam == 2) {
                lfoPhaseRight = mLFOPhase + 0.5;
            } else if (*mTypeIDParam == 3) {
                lfoPhaseRight = mLFOPhase + *mPhaseOffsetParameter;
            }
            
            if (lfoPhaseRight > 1)
            {
                lfoPhaseRight -= 1;
            }
            
            float lfoOutRight;
            if (*mWaveTypeIDParam == 1) {
                lfoOutRight = sin(2*M_PI * lfoPhaseRight);
            } else if (*mWaveTypeIDParam == 2) {
                lfoOutRight = 2*lfoPhaseRight - 1;
            } else if (*mWaveTypeIDParam == 3) {
                lfoOutRight = 4*fabs(lfoPhaseRight - 0.5);
            }
            
            lfoOutLeft *= *mDepthParameter;
            lfoOutRight *= *mDepthParameter;
            
            float lfoOutMappedLeft = 0;
            float lfoOutMappedRight = 0;
            
            if (*mTypeIDParam == 2)
            {
                lfoOutMappedLeft = juce::jmap((float)lfoOutLeft, -1.f, 1.f, 0.005f, 0.03f);
                lfoOutMappedRight = juce::jmap((float)lfoOutRight, -1.f, 1.f, 0.005f, 0.03f);
            } else if (*mTypeIDParam == 3)
            {
                lfoOutMappedLeft = juce::jmap((float)lfoOutLeft, -1.f, 1.f, 0.001f, 0.005f);
                lfoOutMappedRight = juce::jmap((float)lfoOutRight, -1.f, 1.f, 0.001f, 0.005f);
            }
            
            float delayTimeSamplesLeft = getSampleRate() * lfoOutMappedLeft;
            float delayTimeSamplesRight = getSampleRate() * lfoOutMappedRight;
            
            mLFOPhase += *mRateParameter / getSampleRate();
            
            if (mLFOPhase > 1)
            {
                mLFOPhase -= 1;
            }
            
            if (*mTypeIDParam == 2)
            {
                mCircularBufferLeft[mCircularBufferWriteHead] = leftChannel[i];
                mCircularBufferRight[mCircularBufferWriteHead] = rightChannel[i];
            } else if (*mTypeIDParam == 3) {
                mCircularBufferLeft[mCircularBufferWriteHead] = leftChannel[i] + mFeedbackLeft;
                mCircularBufferRight[mCircularBufferWriteHead] = rightChannel[i] + mFeedbackRight;
            }
            
            float delayReadHeadLeft = mCircularBufferWriteHead - delayTimeSamplesLeft;
            
            if (delayReadHeadLeft < 0)
            {
                delayReadHeadLeft += mCircularBufferLength;
            }
            
            float delayReadHeadRight = mCircularBufferWriteHead - delayTimeSamplesRight;
            
            if (delayReadHeadRight < 0)
            {
                delayReadHeadRight += mCircularBufferLength;
            }
            
            int readHeadLeft_x = (int)delayReadHeadLeft;
            int readHeadLeft_x1 = readHeadLeft_x + 1;
            float readHeadFloatLeft = delayReadHeadLeft - readHeadLeft_x;
            
            if (readHeadLeft_x1 >= mCircularBufferLength)
            {
                readHeadLeft_x1 -= mCircularBufferLength;
            }
            
            int readHeadRight_x = (int)delayReadHeadRight;
            int readHeadRight_x1 = readHeadRight_x + 1;
            float readHeadFloatRight = delayReadHeadRight - readHeadRight_x;
            
            if (readHeadRight_x1 >= mCircularBufferLength)
            {
                readHeadRight_x1 -= mCircularBufferLength;
            }
            
            delay_sample_left = linearInterpolation(mCircularBufferLeft[readHeadLeft_x], mCircularBufferLeft[readHeadLeft_x1], readHeadFloatLeft);
            delay_sample_right = linearInterpolation(mCircularBufferRight[readHeadRight_x], mCircularBufferRight[readHeadRight_x1], readHeadFloatRight);
        } else if (*mTypeIDParam == 1)
        {
            mDelayTimeSmoothed = mDelayTimeSmoothed - 0.001 * (mDelayTimeSmoothed - *mDelayTimeParameter);
            mDelayTimeInSamples = getSampleRate() * mDelayTimeSmoothed;
            
            mCircularBufferLeft[mCircularBufferWriteHead] = leftChannel[i] + mFeedbackLeft;
            mCircularBufferRight[mCircularBufferWriteHead] = rightChannel[i] + mFeedbackRight;
            
            mDelayReadHead = mCircularBufferWriteHead - mDelayTimeInSamples;
            
            if (mDelayReadHead < 0) {
                mDelayReadHead += mCircularBufferLength;
            }
            
            int readHead_x = (int)mDelayReadHead;
            int readHead_x1 = readHead_x + 1;
            float readHeadFloat = mDelayReadHead - readHead_x;
            
            if (readHead_x1 >= mCircularBufferLength) {
                readHead_x1 -= mCircularBufferLength;
            }
            
            delay_sample_left = linearInterpolation(mCircularBufferLeft[readHead_x], mCircularBufferLeft[readHead_x1], readHeadFloat);
            delay_sample_right = linearInterpolation(mCircularBufferRight[readHead_x], mCircularBufferRight[readHead_x1], readHeadFloat);
        }
        
        // same as Infatuation
        
        mFeedbackLeft = delay_sample_left * *mFeedbackParameter;
        mFeedbackRight = delay_sample_right * *mFeedbackParameter;
        
        mCircularBufferWriteHead++;
        
        buffer.setSample(0, i, (buffer.getSample(0, i) * (1 - *mDryWetParameter) + delay_sample_left * *mDryWetParameter) + noise);
        buffer.setSample(1, i, (buffer.getSample(1, i) * (1 - *mDryWetParameter) + delay_sample_right * *mDryWetParameter) + noise);
        
        if (mCircularBufferWriteHead >= mCircularBufferLength) {
            mCircularBufferWriteHead = 0;
        }
    }
}

//==============================================================================
bool InfatuationAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* InfatuationAudioProcessor::createEditor()
{
    return new InfatuationAudioProcessorEditor (*this);
}

//==============================================================================
void InfatuationAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    std::unique_ptr<juce::XmlElement> xml(new juce::XmlElement("InfatuationData"));
    xml->setAttribute("DryWet", *mDryWetParameter);
    xml->setAttribute("Depth", *mDepthParameter);
    xml->setAttribute("Rate", *mRateParameter);
    xml->setAttribute("PhaseOffset", *mPhaseOffsetParameter);
    xml->setAttribute("Feedback", *mFeedbackParameter);
    xml->setAttribute("Type", *mTypeParameter);
    xml->setAttribute("TypeID", *mTypeIDParam);
    xml->setAttribute("DelayTime", *mDelayTimeParameter);
    xml->setAttribute("NoiseLevel", *mNoiseLevelParameter);
    xml->setAttribute("WaveType", *mWaveTypeParameter);
    xml->setAttribute("WaveTypeID", *mWaveTypeIDParam);
    xml->setAttribute("NoiseType", *mNoiseTypeParameter);
    xml->setAttribute("NoiseTypeID", *mNoiseTypeIDParam);
    
    copyXmlToBinary(*xml, destData);
}

void InfatuationAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    
    if (xml.get() != nullptr && xml->hasTagName("InfatuationData"))
    {
        *mDryWetParameter = xml->getDoubleAttribute("DryWet");
        *mDepthParameter = xml->getDoubleAttribute("Depth");
        *mRateParameter = xml->getDoubleAttribute("Rate");
        *mPhaseOffsetParameter = xml->getDoubleAttribute("PhaseOffset");
        *mFeedbackParameter = xml->getDoubleAttribute("Feedback");
        *mDelayTimeParameter = xml->getDoubleAttribute("DelayTime");
        *mNoiseLevelParameter = xml->getDoubleAttribute("NoiseLevel");
        
        *mTypeParameter = xml->getIntAttribute("Type");
        *mTypeIDParam = xml->getIntAttribute("TypeID");
        *mWaveTypeParameter = xml->getIntAttribute("WaveType");
        *mWaveTypeIDParam = xml->getIntAttribute("WaveTypeID");
        *mNoiseTypeParameter = xml->getIntAttribute("NoiseType");
        *mNoiseTypeIDParam = xml->getIntAttribute("NoiseTypeID");
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new InfatuationAudioProcessor();
}

float InfatuationAudioProcessor::linearInterpolation(float sample_r, float sample_x1, float inPhase) {
    return (1 - inPhase) * sample_r + inPhase * sample_x1;
}
