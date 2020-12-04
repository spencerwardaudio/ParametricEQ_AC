/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "ACPresetManager.h"

#include "ACParameters.h"
#include "ACAnalyzer.h"


//==============================================================================
/**
*/
class ParametricEQ_ACAudioProcessor  : public AudioProcessor, public AudioProcessorValueTreeState::Listener, public ChangeBroadcaster
{
public:
    
    enum FilterType
    {
        HighPass = 0,
        Peak,
        LowPass,
        LastFilterId
    };

    //==============================================================================
    ParametricEQ_ACAudioProcessor();
    ~ParametricEQ_ACAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    
    #ifndef JucePlugin_PreferredChannelConfigurations
     bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    #endif
    
    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;
    
    void parameterChanged (const String& param, float value) override;
    
    AudioProcessorValueTreeState& getPluginState();
    
    
    size_t getNumBands () const;
    String getBandName   (size_t index) const;
    
    void setClickedBypass(int index);

    static StringArray getFilterTypeNames();

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    
    void createAnalyserPlot (Path& p, const Rectangle<int> bounds, float minFreq, bool input);

    bool checkForNewAnalyserData();
    
    const std::vector<double>& getMagnitudes();
    
    void createFrequencyPlot (Path& p, const std::vector<double>& mags, const Rectangle<int> bounds, float pixelsPerDouble);
    
    //==============================================================================
    const String getName() const override;
    
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    ACPresetManager* getPresetManager()
    {
        return mPresetManager.get();
    }
    

    struct FilterInstance {
        FilterInstance (const String& ACParameterID,
                        FilterType typeToUse,
                        float frequencyToUse,
                    float qualityToUse,
                        float gainToUse = 1.0f,
                        bool activeStateToUse=true)
                         :
        name (ACParameterID),
        type  (typeToUse),
        Freq (frequencyToUse),
        Q (qualityToUse),
        Gain    (gainToUse),
        ActiveState (activeStateToUse)
        {}
        
        String name;
        
        const Colour magColour = Colours::deepskyblue;
        
        FilterType type = HighPass;
        float Freq = 1200.0f;
        float Q = 0.707f;
        float Gain = 1.0f;
        bool ActiveState = true;
        
        std::vector<double> magnitudes;

        };
    
    FilterInstance* getBand (size_t index);

    int getFilterIndexFromID (String paramID);
    
    auto& getTreeState(){return treeState;}
    
    private:
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParametricEQ_ACAudioProcessor)
    
    void updateBand (const size_t index);
    
    void updatePlot();
    
    void bypassState();

    AudioProcessorValueTreeState treeState;
    
    std::vector<FilterInstance> filterInstances;
    
    std::vector<double> frequencies;
    std::vector<double> magnitudes;
    
    using filter = dsp::ProcessorDuplicator<dsp::IIR::Filter <float>, dsp::IIR::Coefficients<float>>;

    using Gain = dsp::Gain<float>;
    dsp::ProcessorChain<filter, filter, filter, filter, filter, Gain> filterChain1;

    double sampleRate = 0;
    
    int clicked = -1;
    
    ACAnalyzer<float> inputAnalyser;
    
    std::unique_ptr<ACPresetManager> mPresetManager;
};
