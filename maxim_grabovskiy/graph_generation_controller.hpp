#pragma once
#include <atomic>
#include <functional>
#include <list>
#include <mutex>
#include <thread>
#include "graph.hpp"
#include "graph_generator.hpp"
namespace uni_course_cpp {
class GraphGenerationController {
 public:
  using JobCallback = std::function<void()>;
  using GenStartedCallback = std::function<void(int index)>;
  using GenFinishedCallback = std::function<void(int index, Graph graph)>;

  class Worker {
   public:
    using GetJobCallback = std::function<std::optional<JobCallback>()>;

    enum class State { Idle, Working, ShouldTerminate };

    explicit Worker(const GetJobCallback& getJobCallback)
        : getJobCallback_(getJobCallback) {}

    void start();
    void stop();

    ~Worker();

   private:
    std::thread thread_;
    GetJobCallback getJobCallback_;
    std::atomic<State> state_ = State::Idle;
  };

  GraphGenerationController(int threads_count,
                            int graphs_count,
                            GraphGenerator::Params graphGeneratorParams);

  void generate(GenStartedCallback const& gen_started_callback,
                GenFinishedCallback const& gen_finished_callback);

 private:
  std::list<Worker> workers_;
  std::list<JobCallback> jobs_;
  int graphsCount_;
  GraphGenerator graphGenerator_;
  std::mutex startCallbackMutex_;
  std::mutex finishCallbackMutex_;
  std::mutex jobsMutex_;
};
}  // namespace uni_course_cpp
