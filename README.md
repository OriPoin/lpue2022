## LPUE 

共三个任务，需要分别提交，在这个仓库中分成三个branch：

### homework1

- Copy 
    - 复制文件
    - Usage:
        - copy ./src_path/filename ./dest_dir/fiename
    - 源码

### homework2

- 文件内容检索 
    - Usage:
        - contentscan "string" dirname
    - 源码

### homework3

- 加减乘除服务线程池
    - 计算队列：每个线程有独立的队列存储计算需求
    - 请求线程：一个队列用于接收结果
    - 加减乘除四种二元运算(int, float)
    - 请求线程与计算线程是多对多关系
    - 源码 + 报告：
        - 程序框架设计
        - 核心数据结构
        - 异常情况设计
        - 流程图
        - 性能分析
