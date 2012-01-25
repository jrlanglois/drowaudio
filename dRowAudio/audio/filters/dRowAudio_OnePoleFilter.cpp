/*
  ==============================================================================
  
  This file is part of the dRowAudio JUCE module
  Copyright 2004-12 by dRowAudio.
  
  ------------------------------------------------------------------------------
 
  dRowAudio can be redistributed and/or modified under the terms of the GNU General
  Public License (Version 2), as published by the Free Software Foundation.
  A copy of the license is included in the module distribution, or can be found
  online at www.gnu.org/licenses.
  
  dRowAudio is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
  A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
  
  ==============================================================================
*/

BEGIN_JUCE_NAMESPACE

OnePoleFilter::OnePoleFilter() noexcept
    : y1 (0.0), b0 (1.0), a1 (0.0)
{
}

OnePoleFilter::~OnePoleFilter() noexcept
{
}

void OnePoleFilter::processSamples (float* const samples,
                                    const int numSamples) noexcept
{
    // make sure sample values are locked
    const ScopedLock sl (lock);
    
    for (int i = 0; i < numSamples; ++i)
    {
        samples[i] = (b0 * samples[i]) + (a1 * y1);
        y1 = samples[i];
    }
}


void OnePoleFilter::makeLowPass (const double sampleRate,
                                 const double frequency) noexcept
{
    float w0 = 2.0 * double_Pi * (frequency / sampleRate);
    float cos_w0 = cos (w0);

    float alpha = (2.0 - cos_w0) - sqrt ((2.0 - cos_w0) * (2.0 - cos_w0) - 1.0);

    const ScopedLock sl (lock);
    
    b0 = 1.0 - alpha;
    a1 = alpha;
}

void OnePoleFilter::makeHighPass (const double sampleRate,
                                  const double frequency) noexcept
{
    float w0 = 2.0 * double_Pi * (frequency / sampleRate);
    float cos_w0 = cos (w0);
    
    float alpha = (2.0 + cos_w0) - sqrt ((2.0 + cos_w0) * (2.0 + cos_w0) - 1.0);

    const ScopedLock sl (lock);

    b0 = alpha - 1.0;
    a1 = -alpha;
}

END_JUCE_NAMESPACE