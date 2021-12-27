#include "graph_generation_controller.hpp"
#include <atomic>
#include <cassert>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include "graph.hpp"
#include "graph_generator.hpp"
namespace uni_course_cpp {
void GraphGenerationController::Worker::start() {
  assert(state_ != State::Working);
  state_ = State::Working;

  thread_ =
      std::thread([&state_ = state_, &getJobCallback_ = getJobCallback_]() {
        while (true) {
          if (state_ == State::ShouldTerminate) {
            state_ = State::Idle;
            return;
          }
          auto const jobOptional = getJobCallback_();
          if (jobOptional.has_value()) {
            auto const& job = jobOptional.value();
            job();
          }
        }
      });
}

void GraphGenerationController::Worker::stop() {
  assert(state_ == State::Working);
  state_ = State::ShouldTerminate;
  if (thread_.joinable())
    thread_.join();
}

GraphGenerationController::Worker::~Worker() {
  if (state_ == State::Working)
    stop();
}

GraphGenerationController::GraphGenerationController(
    int threads_count,
    int graphs_count,
    GraphGenerator::Params graphGenerator_params)
    : graphsCount_(graphs_count), graphGenerator_(graphGenerator_params) {
  int const workers_count = std::min(threads_count, graphs_count);
  for (int worker_number = 0; worker_number < workers_count; worker_number++) {
    workers_.emplace_back(
        [&jobs_ = jobs_,
         &jobsMutex_ = jobsMutex_]() -> std::optional<JobCallback> {
          std::lock_guard<std::mutex> const lock(jobsMutex_);
          if ([&jobs_]() { return jobs_.size(); }()) {
            auto const job = jobs_.front();
            jobs_.pop_front();
            return job;
          }
          return std::nullopt;
        });
  }
}

void GraphGenerationController::generate(
    GenStartedCallback const& genStartedCallback,
    GenFinishedCallback const& genFinishedCallback) {
  std::atomic<int> jobsDone = 0;

  for (int graphNumber = 0; graphNumber < graphsCount_; graphNumber++) {
    jobs_.emplace_back([&genStartedCallback = genStartedCallback,
                        &genFinishedCallback = genFinishedCallback,
                        graphNumber,
                        &finishCallbackMutex_ = finishCallbackMutex_,
                        &startCallbackMutex_ = startCallbackMutex_,
                        &graphGenerator_ = graphGenerator_, &jobsDone]() {
      genStartedCallback(graphNumber);

      auto graph = graphGenerator_.generate();

      {
        std::lock_guard const lock(finishCallbackMutex_);
        genFinishedCallback(graphNumber, std::move(graph));
      }

      jobsDone++;
    });
  }

  for (auto& worker : workers_) {
    worker.start();
  }

  while (jobsDone < graphsCount_) {
  }

  for (auto& worker : workers_) {
    worker.stop();
  }
}
}  // namespace uni_course_cpp
