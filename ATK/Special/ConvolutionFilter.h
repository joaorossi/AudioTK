/**
 * \file ConvolutionFilter.h
 */

#ifndef ATK_SPECIAL_CONVOLUTIONFILTER_H
#define ATK_SPECIAL_CONVOLUTIONFILTER_H

#include <list>
#include <vector>

#include <ATK/Special/config.h>

#include <ATK/Core/TypedBaseFilter.h>
#include <ATK/Utility/FFT.h>

namespace ATK
{
  /// A zero-delay convolution filter based on FFT
  template<typename DataType_>
  class ATK_SPECIAL_EXPORT ConvolutionFilter final : public TypedBaseFilter<DataType_>
  {
  public:
    /// Simplify parent calls
    typedef TypedBaseFilter<DataType_> Parent;
    using typename Parent::DataType;
    using Parent::setup;
    using Parent::converted_inputs;
    using Parent::outputs;
    using Parent::input_delay;
    using Parent::input_sampling_rate;
    using Parent::output_sampling_rate;
  protected:
    /// Current amount of data in the buffer
    mutable unsigned int split_position;
    /// Size of the individual FFTs that are processed
    unsigned int split_size;
    
    /// FFT object for fast FFT/iFFT
    FFT<DataType> processor;
    
    /// Impulse convolved with the input signal
    std::vector<DataType> impulse;
    /// This buffer contains the head of the last convolution (easier to have 2 parts)
    mutable std::vector<DataType> temp_out_buffer;
    /// Called partial convolutions, but actually contains the former temp_in_buffers
    mutable std::list<std::vector<std::complex<DataType> > > partial_frequency_input;
    /// Copied so that it's not reallocated each time
    mutable std::vector<std::complex<DataType> > result;

    /// The impulse is stored here in a unique vector, split in split_size FFTs, one after the other
    std::vector<std::complex<DataType> > partial_frequency_impulse;

    /// Compute the partial convolutions
    void compute_convolutions() const;
    /// Create a new chunk and compute the convolution
    void process_new_chunk(int64_t position) const;

    /// Process the first split_size elements of the convolution
    void process_impulse_beginning(int64_t processed_size, unsigned int position) const;

  public:
    /// Build a new convolution filter
    ConvolutionFilter();

    /**
     * @brief Set the impulse for the convolution
     * @param impulse is the impulse for the convolution
     */
    void set_impulse(std::vector<DataType> impulse);
    
    /*!
    * @brief Set the split size
    * @param split_size is the size of the individual FFTs
    */
    void set_split_size(unsigned int split_size);
  protected:
    void process_impl(std::size_t size) const override final;
    
    void setup() override final;
    
  };
}

#endif
