# 实验 1：数据表示和等效运算

在本目录运行 `make run` 编译并运行两个任务，`make clean` 清理可执行文件。

- `t1.cpp`：压缩与解压 5 个学生的信息。姓名用首字母 `wjr`，压缩后共 91 字节，使用默认 UTF-8 编码。
- `t2.cpp`：12 个位运算函数及 1749 项自动比较。按 32 位补码、算术右移环境运行，`-fwrapv` 保证有符号运算回绕；`addOK` 在溢出时返回 1，绝对值比较排除 `INT_MIN`。
- `report/`：实验报告；`report/evidence/`：程序输出、GDB 记录及调试脚本。

从仓库根目录复现调试：

```sh
make -C experiment_1
gdb --batch -q -x experiment_1/report/evidence/inspect.gdb experiment_1/t1
```
