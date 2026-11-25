#pragma once
#include "amdva/runtime.hpp"
namespace amdva {
struct Detection {
    float x, y, w, h, score;
    int class_id;
};
// Compact GPU postprocessing result: count followed by indexed candidates.
inline constexpr unsigned detection_limit = 300;
inline constexpr size_t detection_readback_bytes =
    sizeof(unsigned) + detection_limit * (sizeof(Detection) + sizeof(unsigned));
class BatchEngine {
  public:
    BatchEngine(const std::string &model, const std::string &info, unsigned batch, int device = 0);
    ~BatchEngine();
    BatchEngine(const BatchEngine &) = delete;
    BatchEngine &operator=(const BatchEngine &) = delete;
    std::vector<std::vector<Detection>> infer(void *input, unsigned valid, float confidence = 0.25f,
                                              float nms = 0.45f);
    const std::vector<std::string> &labels() const;

  private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
    int device_{};
};
} // namespace amdva
