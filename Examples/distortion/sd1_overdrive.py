#!/usr/bin/env python

from ATK.Core import DoubleInPointerFilter, DoubleOutPointerFilter
from ATK.Tools import DoubleOversampling6points5order_32Filter, DoubleOversampling6points5order_16Filter, DoubleOversampling6points5order_8Filter, DoubleOversampling6points5order_4Filter, DoubleDecimationFilter
from ATK.EQ import DoubleButterworthLowPassFilter, DoubleSD1ToneFilter, DoubleChamberlinFilter
from ATK.Distortion import DoubleSD1OverdriveFilter

sample_rate = 48000

def filter_32(input):
  import numpy as np
  output = np.zeros(input.shape, dtype=np.float64)

  infilter = DoubleInPointerFilter(input, False)
  infilter.set_input_sampling_rate(sample_rate)
  overfilter = DoubleOversampling6points5order_32Filter()
  overfilter.set_input_sampling_rate(sample_rate)
  overfilter.set_output_sampling_rate(sample_rate * 32)
  overfilter.set_input_port(0, infilter, 0)
  overdrivefilter = DoubleSD1OverdriveFilter()
  overdrivefilter.set_input_sampling_rate(sample_rate * 32)
  overdrivefilter.set_input_port(0, overfilter, 0)
  overdrivefilter.set_drive(0.9)
  lowpassfilter = DoubleButterworthLowPassFilter()
  lowpassfilter.set_input_sampling_rate(sample_rate * 32)
  lowpassfilter.set_cut_frequency(20000)
  lowpassfilter.set_order(5)
  lowpassfilter.set_input_port(0, overdrivefilter, 0)
#  lowpassfilter2 = DoubleButterworthLowPassFilter()
#  lowpassfilter2.set_input_sampling_rate(sample_rate * 32)
#  lowpassfilter2.set_cut_frequency(20000)
#  lowpassfilter2.set_order(5)
#  lowpassfilter2.set_input_port(0, lowpassfilter, 0)
#  lowpassfilter3 = DoubleButterworthLowPassFilter()
#  lowpassfilter3.set_input_sampling_rate(sample_rate * 32)
#  lowpassfilter3.set_cut_frequency(20000)
#  lowpassfilter3.set_order(5)
#  lowpassfilter3.set_input_port(0, lowpassfilter2, 0)
  decimationfilter = DoubleDecimationFilter(1)
  decimationfilter.set_input_sampling_rate(sample_rate * 32)
  decimationfilter.set_output_sampling_rate(sample_rate)
  decimationfilter.set_input_port(0, lowpassfilter, 0)
  tonefilter = DoubleSD1ToneFilter()
  tonefilter.set_input_sampling_rate(sample_rate)
  tonefilter.set_input_port(0, decimationfilter, 0)
  tonefilter.set_tone(1)
  highpassfilter = DoubleChamberlinFilter()
  highpassfilter.set_input_sampling_rate(sample_rate)
  highpassfilter.set_input_port(0, tonefilter, 0)
  highpassfilter.select(2)
  highpassfilter.set_attenuation(1)
  highpassfilter.set_cut_frequency(20)
  outfilter = DoubleOutPointerFilter(output, False)
  outfilter.set_input_sampling_rate(sample_rate)
  outfilter.set_input_port(0, highpassfilter, 0)
  outfilter.process(input.shape[1])
  return output

def filter_16(input):
  import numpy as np
  output = np.zeros(input.shape, dtype=np.float64)

  infilter = DoubleInPointerFilter(input, False)
  infilter.set_input_sampling_rate(sample_rate)
  overfilter = DoubleOversampling6points5order_16Filter()
  overfilter.set_input_sampling_rate(sample_rate)
  overfilter.set_output_sampling_rate(sample_rate * 16)
  overfilter.set_input_port(0, infilter, 0)
  overdrivefilter = DoubleSD1OverdriveFilter()
  overdrivefilter.set_input_sampling_rate(sample_rate * 16)
  overdrivefilter.set_input_port(0, overfilter, 0)
  overdrivefilter.set_drive(0.9)
  lowpassfilter = DoubleButterworthLowPassFilter()
  lowpassfilter.set_input_sampling_rate(sample_rate * 16)
  lowpassfilter.set_cut_frequency(sample_rate)
  lowpassfilter.set_order(5)
  lowpassfilter.set_input_port(0, overdrivefilter, 0)
  decimationfilter = DoubleDecimationFilter(1)
  decimationfilter.set_input_sampling_rate(sample_rate * 16)
  decimationfilter.set_output_sampling_rate(sample_rate)
  decimationfilter.set_input_port(0, lowpassfilter, 0)
  tonefilter = DoubleSD1ToneFilter()
  tonefilter.set_input_sampling_rate(sample_rate)
  tonefilter.set_input_port(0, decimationfilter, 0)
  tonefilter.set_tone(1)
  highpassfilter = DoubleChamberlinFilter()
  highpassfilter.set_input_sampling_rate(sample_rate)
  highpassfilter.set_input_port(0, tonefilter, 0)
  highpassfilter.select(2)
  highpassfilter.set_attenuation(1)
  highpassfilter.set_cut_frequency(20)
  outfilter = DoubleOutPointerFilter(output, False)
  outfilter.set_input_sampling_rate(sample_rate)
  outfilter.set_input_port(0, highpassfilter, 0)
  outfilter.process(input.shape[1])
  return output

def filter_8(input):
  import numpy as np
  output = np.zeros(input.shape, dtype=np.float64)

  infilter = DoubleInPointerFilter(input, False)
  infilter.set_input_sampling_rate(sample_rate)
  overfilter = DoubleOversampling6points5order_8Filter()
  overfilter.set_input_sampling_rate(sample_rate)
  overfilter.set_output_sampling_rate(sample_rate * 8)
  overfilter.set_input_port(0, infilter, 0)
  overdrivefilter = DoubleSD1OverdriveFilter()
  overdrivefilter.set_input_sampling_rate(sample_rate * 8)
  overdrivefilter.set_input_port(0, overfilter, 0)
  overdrivefilter.set_drive(0.9)
  lowpassfilter = DoubleButterworthLowPassFilter()
  lowpassfilter.set_input_sampling_rate(sample_rate * 8)
  lowpassfilter.set_cut_frequency(sample_rate)
  lowpassfilter.set_order(5)
  lowpassfilter.set_input_port(0, overdrivefilter, 0)
  decimationfilter = DoubleDecimationFilter(1)
  decimationfilter.set_input_sampling_rate(sample_rate * 8)
  decimationfilter.set_output_sampling_rate(sample_rate)
  decimationfilter.set_input_port(0, lowpassfilter, 0)
  tonefilter = DoubleSD1ToneFilter()
  tonefilter.set_input_sampling_rate(sample_rate)
  tonefilter.set_input_port(0, decimationfilter, 0)
  tonefilter.set_tone(1)
  highpassfilter = DoubleChamberlinFilter()
  highpassfilter.set_input_sampling_rate(sample_rate)
  highpassfilter.set_input_port(0, tonefilter, 0)
  highpassfilter.select(2)
  highpassfilter.set_attenuation(1)
  highpassfilter.set_cut_frequency(20)
  outfilter = DoubleOutPointerFilter(output, False)
  outfilter.set_input_sampling_rate(sample_rate)
  outfilter.set_input_port(0, highpassfilter, 0)
  outfilter.process(input.shape[1])
  return output

def filter_4(input):
  import numpy as np
  output = np.zeros(input.shape, dtype=np.float64)

  infilter = DoubleInPointerFilter(input, False)
  infilter.set_input_sampling_rate(sample_rate)
  overfilter = DoubleOversampling6points5order_4Filter()
  overfilter.set_input_sampling_rate(sample_rate)
  overfilter.set_output_sampling_rate(sample_rate * 4)
  overfilter.set_input_port(0, infilter, 0)
  overdrivefilter = DoubleSD1OverdriveFilter()
  overdrivefilter.set_input_sampling_rate(sample_rate * 4)
  overdrivefilter.set_input_port(0, overfilter, 0)
  overdrivefilter.set_drive(0.9)
  lowpassfilter = DoubleButterworthLowPassFilter()
  lowpassfilter.set_input_sampling_rate(sample_rate * 4)
  lowpassfilter.set_cut_frequency(sample_rate)
  lowpassfilter.set_order(5)
  lowpassfilter.set_input_port(0, overdrivefilter, 0)
  decimationfilter = DoubleDecimationFilter(1)
  decimationfilter.set_input_sampling_rate(sample_rate * 4)
  decimationfilter.set_output_sampling_rate(sample_rate)
  decimationfilter.set_input_port(0, lowpassfilter, 0)
  tonefilter = DoubleSD1ToneFilter()
  tonefilter.set_input_sampling_rate(sample_rate)
  tonefilter.set_input_port(0, decimationfilter, 0)
  tonefilter.set_tone(1)
  highpassfilter = DoubleChamberlinFilter()
  highpassfilter.set_input_sampling_rate(sample_rate)
  highpassfilter.set_input_port(0, tonefilter, 0)
  highpassfilter.select(2)
  highpassfilter.set_attenuation(1)
  highpassfilter.set_cut_frequency(20)
  outfilter = DoubleOutPointerFilter(output, False)
  outfilter.set_input_sampling_rate(sample_rate)
  outfilter.set_input_port(0, highpassfilter, 0)
  outfilter.process(input.shape[1])
  return output

if __name__ == "__main__":
  import numpy as np
  size = 100000
  
  d = np.arange(size).reshape(1, -1)
  d = np.sin(d * 2 * np.pi * 1000 / sample_rate)
  np.savetxt("input.txt", d)
  out = filter_32(d)
  np.savetxt("output32.txt", out)
  out = filter_16(d)
  np.savetxt("output16.txt", out)
  out = filter_8(d)
  np.savetxt("output8.txt", out)
  out = filter_4(d)
  np.savetxt("output4.txt", out)
