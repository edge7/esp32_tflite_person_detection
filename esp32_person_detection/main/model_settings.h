#ifndef TENSORFLOW_LITE_MICRO_EXAMPLES_PERSON_DETECTION_MODEL_SETTINGS_H_
#define TENSORFLOW_LITE_MICRO_EXAMPLES_PERSON_DETECTION_MODEL_SETTINGS_H_

// Keeping these as constant expressions allow us to allocate fixed-sized arrays
// on the stack for our working memory.

// All of these values are derived from the values used during model training,
// if you change your model you'll need to update these constants.
constexpr int kNumCols = 128;
constexpr int kNumRows = 128;
constexpr int kNumChannels = 1;


#endif  // TENSORFLOW_LITE_MICRO_EXAMPLES_PERSON_DETECTION_MODEL_SETTINGS_H_
