#pragma once
#include <iostream>
#include <queue>
#include <vector>
#include <mutex>
#include <thread>
#include <boost/log/trivial.hpp>
#include <fstream>


struct Job
{
    enum Statue
    {
        pending,
        done,
        error
    } status = pending;
    double a;
    double b;
    double c;
    char op;
};

class compute
{
private:
    std::queue<Job *> *jobs_req;
    std::queue<Job *> *jobs_ans = new std::queue<Job *>;
    int num_req;
    int num_ans = 0;
    int num_dispatched = 0;
    std::vector<std::thread *> worker_threads;
    std::vector<std::queue<Job *> *> worker_jobs_queues;
    std::vector<std::mutex *> worker_jobs_queues_mtx;
    int req_thread_num;
    int cal_thread_num;
    // mutex for jobs_req&num_dispatched  jobs_ans&num_ans
    std::mutex mtx_req, mtx_ans;

public:
    compute(std::queue<Job *> *jobs, int req_thread_num, int cal_thread_num);
    void req_worker(int i);
    void cal_worker(int i);
    int init_req_pool(int req_thread_num);
    int init_cal_pool(int cal_thread_num);
    int init(std::queue<Job *> *jobs, int req_thread_num, int cal_thread_num);
    std::queue<Job *> *run();
    int clear();
    ~compute();
};

compute::compute(std::queue<Job *> *jobs, int req_thread_num, int cal_thread_num)
{
    this->jobs_req = jobs;
    this->num_req = jobs->size();
    this->cal_thread_num = cal_thread_num;
    this->req_thread_num = req_thread_num;
}
void compute::req_worker(int i)
{
    while (num_dispatched < num_req && !jobs_req->empty())
    {
        for (int m = 0; m < cal_thread_num / req_thread_num; m++)
        {
            for (int n = 0; n < req_thread_num; n++)
            {
                mtx_req.lock();
                if (!jobs_req->empty())
                {
                    worker_jobs_queues_mtx[req_thread_num * m + n]->lock();
                    worker_jobs_queues[req_thread_num * m + n]->push(jobs_req->front());
                    worker_jobs_queues_mtx[req_thread_num * m + n]->unlock();
                    num_dispatched += 1;
                    jobs_req->pop();
                }
                mtx_req.unlock();
            }
        }
    }
}
void compute::cal_worker(int i)
{
    std::queue<Job *> *cal_queue = worker_jobs_queues[i];
    while (num_ans < num_req)
    {
        BOOST_LOG_TRIVIAL(debug) << "num_ans: " << num_ans << " id: " << i << " jobs: " << cal_queue->size();
        // cal if not empty
        if (!cal_queue->empty())
        {
            if (cal_queue->front()->op == '+')
            {
                cal_queue->front()->c = cal_queue->front()->a + cal_queue->front()->b;
                cal_queue->front()->status = Job::Statue::done;
            }
            else if (cal_queue->front()->op == '-')
            {
                cal_queue->front()->c = cal_queue->front()->a - cal_queue->front()->b;
                cal_queue->front()->status = Job::Statue::done;
            }
            else if (cal_queue->front()->op == '*')
            {
                cal_queue->front()->c = cal_queue->front()->a * cal_queue->front()->b;
                cal_queue->front()->status = Job::Statue::done;
            }
            else if (cal_queue->front()->op == '/')
            {
                if (cal_queue->front()->b == 0.0)
                {
                    cal_queue->front()->c = 0;
                    cal_queue->front()->status = Job::Statue::error;
                }
                else
                {
                    cal_queue->front()->c = cal_queue->front()->a / cal_queue->front()->b;
                    cal_queue->front()->status = Job::Statue::done;
                }
            }
            // write to ans queue
            mtx_ans.lock();
            worker_jobs_queues_mtx[i]->lock();
            jobs_ans->push(cal_queue->front());
            cal_queue->pop();
            num_ans += 1;
            worker_jobs_queues_mtx[i]->unlock();
            mtx_ans.unlock();
        }
    }
}
int compute::init_cal_pool(int cal_thread_num)
{
    for (int i = 0; i < cal_thread_num; i++)
    {
        std::thread *t = new std::thread(&compute::cal_worker, this, i);
        worker_threads.emplace_back(t);
    }
    return 0;
}
int compute::init_req_pool(int req_thread_num)
{
    for (int i = 0; i < req_thread_num; i++)
    {
        std::thread *t = new std::thread(&compute::req_worker, this, i);
        worker_threads.emplace_back(t);
    }
    return 0;
}
std::queue<Job *> *compute::run()
{
    for (int i = 0; i < cal_thread_num; i++)
    {
        std::queue<Job *> *cal_queue = new std::queue<Job *>;
        worker_jobs_queues.push_back(cal_queue);
        std::mutex *mtx = new std::mutex;
        worker_jobs_queues_mtx.push_back(mtx);
    }
    init_cal_pool(cal_thread_num);
    init_req_pool(req_thread_num);
    for (auto i : worker_threads)
    {
        i->join();
    }
    return jobs_ans;
}
compute::~compute()
{
    // while (!worker_jobs_queues.empty())
    // {
    //     delete worker_jobs_queues.front();
    //     worker_jobs_queues.pop();
    // }
}
