#include <stdio.h>
#include <limits.h>
#include <string.h>
// 32位补码实验：使用 -fwrapv 编译，有符号右移为算术右移。
#define check(format,f1,f2) \
	printf((format),(f1),(f2)); \
	printf("  自实现函数和标准函数 运算结果相等:%s\n",((f1)==(f2)?"True":"false"));
/* 1. 返回 x 的绝对值 */
int absVal(int x) {
	// 取符号symbol(有符号右移)
    int symbol = x >> 31;
    return (x ^ symbol) + (~symbol + 1);
}
/* 标准函数 */
int absVal_standard(int x) {
    return (x < 0) ? -x : x;
}
/* 2. 实现 -x */
int negate(int x) {
    return ~x + 1;
}
/* 标准函数 */
int negate_standard(int x) {
    return -x;
}
/* 3. 实现 & */
int bitAnd(int x, int y) {
    return ~(~x | ~y);
}
/* 标准函数 */
int bitAnd_standard(int x, int y) {
    return x & y;
}
/* 4. 实现 | */
int bitOr(int x, int y) {
    return ~((~x) & (~y));
}
/* 标准函数 */
int bitOr_standard(int x, int y) {
    return x | y;
}
/* 5. 实现 ^ */
int bitXor(int x, int y) {
    return ~(~(x & ~y) & ~(~x & y));
}
/* 标准函数 */
int bitXor_standard(int x, int y) {
    return x ^ y;
}
/* 6. 判断x是否为最大的正整数（7FFFFFFF） */
int isTmax(int x) {
    return !((x + 1) ^ (~x)) & !!(x + 1);
}
/* 标准函数 */
int isTmax_standard(int x) {
    return x == 0x7FFFFFFF;
}
/* 7. 统计x的二进制表示中 1 的个数 */
int bitCount(int x) {
    int m1 = 0x55555555;
    int m2 = 0x33333333;
    int m4 = 0x0F0F0F0F;
    x = (x & m1) + ((x >> 1) & m1);
    x = (x & m2) + ((x >> 2) & m2);
    x = (x & m4) + ((x >> 4) & m4);
    x = x + (x >> 8);
    x = x + (x >> 16);
    return x & 0x3F;
}
/* 标准函数 */
int bitCount_standard(int x) {
    int count = 0;
    unsigned int ux = (unsigned int)x;
    while (ux) {
        ux &= (ux - 1);
        count++;
    }
    return count;
}
/* 8. 产生从lowbit 到 highbit 全为1，其他位为0的数 */
int bitMask(int highbit, int lowbit) {
    return (~0u << lowbit) & ~(~0u << highbit << 1);
}
/* 标准函数 */
int bitMask_standard(int highbit, int lowbit) {
    unsigned int result = 0;
    for (int i = lowbit; i <= highbit; ++i)
        result |= 1u << i;
    return result;
}
/* 9. 当x+y 会产生溢出时返回1，否则返回 0 */
int addOK(int x, int y) {
    int sum = x + y;
    return (((sum ^ x) & (sum ^ y)) >> 31) & 1;
}
/* 标准函数 */
int addOK_standard(int x, int y) {
    long long sum = (long long)x + y;
    return sum > INT_MAX || sum < INT_MIN;
}
/* 10. 将x的第n个字节与第m个字节交换，返回交换后的结果 */
int byteSwap(int x, int n, int m) {
    unsigned int mask = 0xFF;
    unsigned int byte_n = (x >> (n << 3)) & mask;
    unsigned int byte_m = (x >> (m << 3)) & mask;
    x = (x & ~(mask << (n << 3))) | (byte_m << (n << 3));
    x = (x & ~(mask << (m << 3))) | (byte_n << (m << 3));
    return x;
}
int byteSwap_standard(int x, int n, int m) {
    unsigned char bytes[sizeof(int)];
    memcpy(bytes, &x, sizeof(x));
    unsigned char temp = bytes[n];
    bytes[n] = bytes[m];
    bytes[m] = temp;
    memcpy(&x, bytes, sizeof(x));
    return x;
}
/* 11. 不使用 ! 实现逻辑非 */
int bang(int x) {
    return ((x | (~x + 1)) >> 31) + 1;
}
int bang_standard(int x) {
    return !x;
}
/* 12. 32位中0的个数与1的个数奇偶性相同 */
int bitParity(int x) {
    x = x ^ (x >> 16);
    x = x ^ (x >> 8);
    x = x ^ (x >> 4);
    x = x ^ (x >> 2);
    x = x ^ (x >> 1);
    return x & 1;
}
int bitParity_standard(int x) {
    unsigned int ux = (unsigned int)x;
    int zeros = 0;
    for (int i = 0; i < 32; ++i) {
        if (((ux >> i) & 1u) == 0) ++zeros;
    }
    return zeros % 2;
}
/* 对边界值及所有合法位/字节位置自动比较。 */
int boundary_check() {
    int values[] = {INT_MIN, INT_MIN + 1, -256, -1, 0,
                    1, 2, 7, 255, INT_MAX - 1, INT_MAX};
    int total = 0, failed = 0;
#define VERIFY(a, b) do { ++total; if ((a) != (b)) ++failed; } while (0)
    for (int x : values) {
        // INT_MIN 的数学绝对值不能用 int 表示。
        if (x != INT_MIN) VERIFY(absVal(x), absVal_standard(x));
        VERIFY(negate(x), negate_standard(x));
        VERIFY(isTmax(x), isTmax_standard(x));
        VERIFY(bitCount(x), bitCount_standard(x));
        VERIFY(bang(x), bang_standard(x));
        VERIFY(bitParity(x), bitParity_standard(x));
        for (int y : values) {
            VERIFY(bitAnd(x, y), bitAnd_standard(x, y));
            VERIFY(bitOr(x, y), bitOr_standard(x, y));
            VERIFY(bitXor(x, y), bitXor_standard(x, y));
            VERIFY(addOK(x, y), addOK_standard(x, y));
        }
        for (int n = 0; n < 4; ++n)
            for (int m = 0; m < 4; ++m)
                VERIFY(byteSwap(x, n, m), byteSwap_standard(x, n, m));
    }
    for (int high = 0; high < 32; ++high)
        for (int low = 0; low < 32; ++low)
            VERIFY(bitMask(high, low), bitMask_standard(high, low));
#undef VERIFY
    printf("边界自动比较: %d 项, 失败 %d 项\n", total, failed);
    return failed;
}

int main() {
    check("1. absVal(-1) = %d, \n   absVal_standard(-1) = %d\n", absVal(-1), absVal_standard(-1));
    check("2. negate(2) = %d, \n   negate_standard(2) = %d\n", negate(2), negate_standard(2));
    check("3. bitAnd(12, 6) = %d, \n   bitAnd_standard(12, 6) = %d\n", bitAnd(12, 6), bitAnd_standard(12, 6));
    check("4. bitOr(5, 3) = %d, \n   bitOr_standard(5, 3) = %d\n", bitOr(5, 3), bitOr_standard(5, 3));
    check("5. bitXor(5, 3) = %d, \n  bitXor_standard(5, 3) = %d\n", bitXor(5, 3), bitXor_standard(5, 3));
    check("6. isTmax(0x7FFFFFFF) = %d, \n   isTmax_standard(0x7FFFFFFF) = %d\n", isTmax(0x7FFFFFFF), isTmax_standard(0x7FFFFFFF));
    check("7. bitCount(15) = %d, \n   bitCount_standard(15) = %d\n", bitCount(15), bitCount_standard(15));
    check("8. bitMask(5, 3) = %X, \n   bitMask_standard(5, 3) = %X\n", bitMask(5, 3), bitMask_standard(5, 3));
    check("9. addOK(0x7FFFFFFF, 1) = %d, \n   addOK_standard(0x7FFFFFFF, 1) = %d\n", addOK(0x7FFFFFFF, 1), addOK_standard(0x7FFFFFFF, 1));
    check("10. byteSwap(0x12345678, 1, 3) = %X, standard = %X\n",
          byteSwap(0x12345678, 1, 3), byteSwap_standard(0x12345678, 1, 3));
    check("    byteSwap(0xDEADBEEF, 0, 2) = %X, standard = %X\n",
          byteSwap(0xDEADBEEF, 0, 2), byteSwap_standard(0xDEADBEEF, 0, 2));
    check("11. bang(3) = %d, standard = %d\n", bang(3), bang_standard(3));
    check("    bang(0) = %d, standard = %d\n", bang(0), bang_standard(0));
    check("12. bitParity(5) = %d, standard = %d\n",
          bitParity(5), bitParity_standard(5));
    check("    bitParity(7) = %d, standard = %d\n",
          bitParity(7), bitParity_standard(7));
    return boundary_check() != 0;
}
