/**
 * \file MultipleUniversalFixedDelayLineFilter.cpp
 */

#include "MultipleUniversalFixedDelayLineFilter.h"

#include <cmath>
#include <cstring>
#include <complex>
#include <stdexcept>

#include <ATK/Core/TypeTraits.h>

namespace ATK
{
  template<class DataType, unsigned int nb_channels>
  class MultipleUniversalFixedDelayLineFilter<DataType, nb_channels>::SUFDLF_Impl
  {
  public:
    std::array<std::vector<DataType>, nb_channels> delay_line;
    std::array<std::vector<DataType>, nb_channels> processed_input;

    SUFDLF_Impl(std::size_t max_delay)
    {
      // reset the delay line
      for(unsigned int channel = 0; channel < nb_channels; ++channel)
      {
        processed_input[channel].assign(max_delay, 0);
      }
    }

    void update_delay_line(std::size_t max_delay)
    {
      auto array_size = processed_input[0].size();
      // Update delay line
      ATK_VECTORIZE for (std::size_t i = 0; i < max_delay; ++i)
      {
        for(unsigned int channel = 0; channel < nb_channels; ++channel)
        {
          processed_input[channel][i] = processed_input[channel][array_size + i - max_delay];
        }
      }
    }
  };

  template<class DataType, unsigned int nb_channels>
  MultipleUniversalFixedDelayLineFilter<DataType, nb_channels>::MultipleUniversalFixedDelayLineFilter(std::size_t max_delay)
    :Parent(nb_channels, nb_channels), impl(new SUFDLF_Impl(max_delay)), max_delay(max_delay)
  {
    delay.fill(0);
    blend.fill(0);
    feedback.fill(0);
    feedforward.fill(0);
  }
  
  template<class DataType, unsigned int nb_channels>
  MultipleUniversalFixedDelayLineFilter<DataType, nb_channels>::~MultipleUniversalFixedDelayLineFilter()
  {
  }
  
  template<class DataType, unsigned int nb_channels>
  void MultipleUniversalFixedDelayLineFilter<DataType, nb_channels>::set_delay(unsigned int channel, std::size_t delay)
  {
    if(delay == 0)
    {
      throw std::out_of_range("Delay must be strictly positive");
    }
    if(delay >= max_delay)
    {
      throw std::out_of_range("Delay must be less than delay line size");
    }

    this->delay[channel] = delay;
  }

  template<class DataType, unsigned int nb_channels>
  std::size_t MultipleUniversalFixedDelayLineFilter<DataType, nb_channels>::get_delay(unsigned int channel) const
  {
    return delay[channel];
  }

  template<class DataType, unsigned int nb_channels>
  void MultipleUniversalFixedDelayLineFilter<DataType, nb_channels>::set_blend(unsigned int channel, double blend)
  {
    this->blend[channel] = blend;
  }

  template<class DataType, unsigned int nb_channels>
  double MultipleUniversalFixedDelayLineFilter<DataType, nb_channels>::get_blend(unsigned int channel) const
  {
    return blend[channel];
  }

  template<class DataType, unsigned int nb_channels>
  void MultipleUniversalFixedDelayLineFilter<DataType, nb_channels>::set_feedback(unsigned int from_channel, unsigned int to_channel, double feedback)
  {
    if(std::abs(feedback) > 1)
    {
      throw std::out_of_range("Feedback must be between -1 and 1 to avoid divergence");
    }
    this->feedback[from_channel * nb_channels + to_channel] = feedback;
  }

  template<class DataType, unsigned int nb_channels>
  double MultipleUniversalFixedDelayLineFilter<DataType, nb_channels>::get_feedback(unsigned int from_channel, unsigned int to_channel) const
  {
    return feedback[from_channel * nb_channels + to_channel];
  }

  template<class DataType, unsigned int nb_channels>
  void MultipleUniversalFixedDelayLineFilter<DataType, nb_channels>::set_feedforward(unsigned int from_channel, unsigned int to_channel, double feedforward)
  {
    this->feedforward[from_channel * nb_channels + to_channel]  = feedforward;
  }

  template<class DataType, unsigned int nb_channels>
  double MultipleUniversalFixedDelayLineFilter<DataType, nb_channels>::get_feedforward(unsigned int from_channel, unsigned int to_channel) const
  {
    return feedforward[from_channel * nb_channels + to_channel] ;
  }

  template<class DataType, unsigned int nb_channels>
  void MultipleUniversalFixedDelayLineFilter<DataType, nb_channels>::full_setup()
  {
    // reset the delay line
    for(unsigned int channel = 0; channel < nb_channels; ++channel)
    {
      impl->processed_input[channel].assign(max_delay, 0);
    }
  }

  template<class DataType, unsigned int nb_channels>
  void MultipleUniversalFixedDelayLineFilter<DataType, nb_channels>::process_impl(std::size_t size) const
  {
    impl->update_delay_line(max_delay);

    for(unsigned int channel = 0; channel < nb_channels; ++channel)
    {
      impl->delay_line[channel].resize(size);
      impl->processed_input[channel].resize(max_delay + size);
    }

    ATK_VECTORIZE for(std::size_t i = 0; i < size; ++i)
    {
      for(unsigned int channel = 0; channel < nb_channels; ++channel)
      {
        impl->delay_line[channel][i] = impl->processed_input[channel][i + max_delay - delay[channel]];
      }
      for(unsigned int from_channel = 0; from_channel < nb_channels; ++from_channel)
      {
        impl->processed_input[from_channel][max_delay + i] = converted_inputs[from_channel][i];
        for(unsigned int to_channel = 0; to_channel < nb_channels; ++to_channel)
        {
          impl->processed_input[from_channel][max_delay + i] += feedback[from_channel * nb_channels + to_channel] * static_cast<typename TypeTraits<DataType>::Scalar>(impl->processed_input[to_channel][max_delay + i - delay[to_channel]]);
        }
      }
      for(unsigned int from_channel = 0; from_channel < nb_channels; ++from_channel)
      {
        outputs[from_channel][i] = blend[from_channel] * static_cast<typename TypeTraits<DataType>::Scalar>(impl->processed_input[from_channel][max_delay + i]);
        for(unsigned int to_channel = 0; to_channel < nb_channels; ++to_channel)
        {
          outputs[from_channel][i] += feedforward[from_channel * nb_channels + to_channel] * static_cast<typename TypeTraits<DataType>::Scalar>(impl->delay_line[to_channel][i]);
        }
      }
    }
  }
  
  template class MultipleUniversalFixedDelayLineFilter<float, 2>;
  template class MultipleUniversalFixedDelayLineFilter<double, 2>;
  template class MultipleUniversalFixedDelayLineFilter<std::complex<float>, 2>;
  template class MultipleUniversalFixedDelayLineFilter<std::complex<double>, 2>;

  template class MultipleUniversalFixedDelayLineFilter<float, 4>;
  template class MultipleUniversalFixedDelayLineFilter<double, 4>;
  template class MultipleUniversalFixedDelayLineFilter<std::complex<float>, 4>;
  template class MultipleUniversalFixedDelayLineFilter<std::complex<double>, 4>;

  template class MultipleUniversalFixedDelayLineFilter<float, 8>;
  template class MultipleUniversalFixedDelayLineFilter<double, 8>;
  template class MultipleUniversalFixedDelayLineFilter<std::complex<float>, 8>;
  template class MultipleUniversalFixedDelayLineFilter<std::complex<double>, 8>;
}