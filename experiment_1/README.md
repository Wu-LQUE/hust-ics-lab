# 实验 1：数据表示和等效运算

在本目录运行 `make run`，编译并运行两个任务。最终报告位于 `report/`，实际运行和 GDB 记录在 `report/evidence/`。

- `t1.cpp`：结构体保持任务书规定的 `name[8]`。源码为 UTF-8，编译使用 `-fexec-charset=GBK`，使“吴基镕”连同结束符共占 7 字节。`make run` 用 `iconv` 将 GBK 输出转换为 UTF-8。不要直接用默认 UTF-8 执行字符集编译，否则完整姓名会超出数组容量。
- `t2.cpp`：按实验的 32 位补码、算术右移环境实现，使用 `-fwrapv` 固定有符号加法的回绕语义。`addOK` 按本任务书定义：溢出返回 1。数学上 `abs(INT_MIN)` 无法用 `int` 表示，不纳入绝对值比较。

从仓库根目录复现调试：

```sh
make -C experiment_1
gdb --batch -q -x experiment_1/report/evidence/inspect.gdb experiment_1/t1
```

两种压缩函数各调用一次；整体复制使用单条 `memcpy` 语句处理每个数值字段，避免压缩缓冲区未对齐时通过 `short*`、`float*` 解引用。位运算程序包含全部 12 个函数及 1749 项边界自动比较。
