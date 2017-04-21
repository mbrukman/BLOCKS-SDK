/*
  ==============================================================================

   This file is part of the JUCE library.
   Copyright (c) 2015 - ROLI Ltd.

   Permission is granted to use this software under the terms of either:
   a) the GPL v2 (or any later version)
   b) the Affero GPL v3

   Details of these licenses can be found at: www.gnu.org/licenses

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

   ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.juce.com for more information.

  ==============================================================================
*/

#pragma once


//==============================================================================
/**
    A subclass of AudioPlayHead can supply information about the position and
    status of a moving play head during audio playback.

    One of these can be supplied to an AudioProcessor object so that it can find
    out about the position of the audio that it is rendering.

    @see AudioProcessor::setPlayHead, AudioProcessor::getPlayHead
*/
class JUCE_API  AudioPlayHead
{
protected:
    //==============================================================================
    AudioPlayHead() {}

public:
    virtual ~AudioPlayHead() {}

    //==============================================================================
    /** Frame rate types. */
    enum FrameRateType
    {
        fps24           = 0,
        fps25           = 1,
        fps2997         = 2,
        fps30           = 3,
        fps2997drop     = 4,
        fps30drop       = 5,
        fps60           = 6,
        fps60drop       = 7,
        fpsUnknown      = 99
    };

    //==============================================================================
    /** This structure is filled-in by the AudioPlayHead::getCurrentPosition() method.
    */
    struct JUCE_API  CurrentPositionInfo
    {
        /** The tempo in BPM */
        double bpm;

        /** Time signature numerator, e.g. the 3 of a 3/4 time sig */
        int timeSigNumerator;
        /** Time signature denominator, e.g. the 4 of a 3/4 time sig */
        int timeSigDenominator;

        /** The current play position, in samples from the start of the edit. */
        int64 timeInSamples;
        /** The current play position, in seconds from the start of the edit. */
        double timeInSeconds;

        /** For timecode, the position of the start of the edit, in seconds from 00:00:00:00. */
        double editOriginTime;

        /** The current play position, in pulses-per-quarter-note. */
        double ppqPosition;

        /** The position of the start of the last bar, in pulses-per-quarter-note.

            This is the time from the start of the edit to the start of the current
            bar, in ppq units.

            Note - this value may be unavailable on some hosts, e.g. Pro-Tools. If
            it's not available, the value will be 0.
        */
        double ppqPositionOfLastBarStart;

        /** The video frame rate, if applicable. */
        FrameRateType frameRate;

        /** True if the transport is currently playing. */
        bool isPlaying;

        /** True if the transport is currently recording.

            (When isRecording is true, then isPlaying will also be true).
        */
        bool isRecording;

        /** The current cycle start position in pulses-per-quarter-note.
            Note that not all hosts or plugin formats may provide this value.
            @see isLooping
        */
        double ppqLoopStart;

        /** The current cycle end position in pulses-per-quarter-note.
            Note that not all hosts or plugin formats may provide this value.
            @see isLooping
        */
        double ppqLoopEnd;

        /** True if the transport is currently looping. */
        bool isLooping;

        //==============================================================================
        bool operator== (const CurrentPositionInfo& other) const noexcept;
        bool operator!= (const CurrentPositionInfo& other) const noexcept;

        void resetToDefault();
    };

    //==============================================================================
    /** Fills-in the given structure with details about the transport's
        position at the start of the current processing block. If this method returns
        false then the current play head position is not available and the given
        structure will be undefined.

        You can ONLY call this from your processBlock() method! Calling it at other
        times will produce undefined behaviour, as the host may not have any context
        in which a time would make sense, and some hosts will almost certainly have
        multithreading issues if it's not called on the audio thread.
    */
    virtual bool getCurrentPosition (CurrentPositionInfo& result) = 0;

    /** Returns true if this object can control the transport. */
    virtual bool canControlTransport()                         { return false; }

    /** Starts or stops the audio. */
    virtual void transportPlay (bool shouldStartPlaying)       { ignoreUnused (shouldStartPlaying); }

    /** Starts or stops recording the audio. */
    virtual void transportRecord (bool shouldStartRecording)   { ignoreUnused (shouldStartRecording); }

    /** Rewinds the audio. */
    virtual void transportRewind()                             {}
};
