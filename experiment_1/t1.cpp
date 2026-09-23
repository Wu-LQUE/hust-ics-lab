#include "stdio.h"
#include "string.h"
struct student {
	char name[8];	  // 8
	short age;		  // 2，后有2字节填充
	float score;	  // 4
	char remark[200]; // 200
};
#define N 5
#define N1 2
#define N2 3
student old_s[N]; // old_s[0].name 为自己的姓名，score为学号后两位;
student new_s[N];
int pack_student_bytebybyte(student *s, int sno, char *buf); // 逐字节压缩
int pack_student_whole(student *s, int sno,
					   char *buf); // 整句读入，要求使用strcpy
int restore_student(char *buf, int len, student *s); // 恢复函数
void print_hex(const char *title, char *buf, int n); // 标题，缓冲区，长度
void init_old_s() {
	for (int i = 0; i < N; ++i) {
		if (i == 0) {
			strcpy(old_s[i].name, "wjr");
			old_s[i].age = 19;
			old_s[i].score = 41;
			strcpy(old_s[0].remark, "good");
		} else {
			char buf[200];
			snprintf(buf, sizeof(buf), "stu%d", i + 1);
			strcpy(old_s[i].name, buf);
			old_s[i].age = 19 + i;
			old_s[i].score = 41 + i;
			snprintf(buf, sizeof(buf), "remark%d", i + 1);
			strcpy(old_s[i].remark, buf);
		}
	}
}
void print_students(student *s, int sno) {
	for (int i = 0; i < sno; ++i) {
		printf("name=%s\nage=%d\nscore=%f\nremark=%s\n%s", s[i].name, s[i].age,
			   s[i].score, s[i].remark, (i < sno - 1 ? "\n" : ""));
	}
}
int main() {
	char message[4096];
	init_old_s();
	printf("sizeof(student) = %zu bytes\n", sizeof(student));
	printf("压缩前 old_s 总长度 = %zu bytes\n", sizeof(old_s));
	printf("压缩前 old_s:\n");
	print_students(old_s, N);

	/* 前 N1 个用 pack_student_bytebybyte 压缩，只调用一次 */
	int len1 = pack_student_bytebybyte(old_s, N1, message);

	/* 后 N2 个用 pack_student_whole 压缩，只调用一次 */
	int len2 = pack_student_whole(old_s + N1, N2, message + len1);

	int total = len1 + len2;
	printf("pack_student_bytebybyte 压缩 %d 人，字节数 = %d\n", N1, len1);
	printf("pack_student_whole      压缩 %d 人，字节数 = %d\n", N2, len2);
	printf("压缩后 message 总长度 = %d 字节\n", total);

	/* 输出 message 前 40 个字节 */
	print_hex("message 前40字节", message, total < 40 ? total : 40);

	 /* 解压 */
    int n = restore_student(message, total, new_s);
    printf("解压人数 = %d\n", n);
	printf("解压后 new_s:\n");
    print_students( new_s, n);

    /* 验证第0个学生 score 的内存表示 */
	/*  41 = 101001 (二进制) = 1.01001 (二进制) * 2^5 
		IEEE 754 单精度：1 位符号 + 8 位阶码 + 23 位尾数
		S = 0
		E = 5 + 127 = 132 = 10000100 (二进制)
		M = 01001000000000000000000
		= 0100 0010 0010 0100 0000 0000 0000 0000
		= 0x42240000 再转小端字节序*/
    unsigned char *p = (unsigned char*)&old_s[0].score;
    printf("old_s[0].score = %.2f, 小端内存字节: %02X %02X %02X %02X\n",
           old_s[0].score, p[0], p[1], p[2], p[3]);

    return 0;
}
int pack_student_bytebybyte(student *s, int sno, char *buf) {
	char *start = buf;
	for (int i = 0; i < sno; i++) {
		/* 写 name，包含结束符 '\0' */
		char *p = s[i].name;
		while ((*buf++ = *p++))
			;
		/* 写 age，逐字节写 */
		unsigned char *q = (unsigned char *)&s[i].age;
		for (size_t j = 0; j < sizeof(short); j++) {
			*buf++ = q[j];
		}
		/* 写 score，逐字节写 */
		q = (unsigned char *)&s[i].score;
		for (size_t j = 0; j < sizeof(float); j++) {
			*buf++ = q[j];
		}
		/* 写 remark，包含结束符 '\0' */
		p = s[i].remark;
		while ((*buf++ = *p++))
			;
	}
	return (int)(buf - start);
}
int pack_student_whole(student *s, int sno, char *buf) {
	char *start = buf;
	for (int i = 0; i < sno; i++) {
		/* 写 name，包含结束符 '\0' */
		strcpy(buf, s[i].name);
		buf += strlen(s[i].name) + 1;
		/* 写 age，一次性写 */
		memcpy(buf, &s[i].age, sizeof(s[i].age));
		buf += sizeof(short);
		/* 写 score */
		memcpy(buf, &s[i].score, sizeof(s[i].score));
		buf += sizeof(float);
		/* 写 remark，包含结束符 '\0' */
		strcpy(buf, s[i].remark);
		buf += strlen(s[i].remark) + 1;
	}
	return (int)(buf - start);
}
void print_hex(const char *title, char *buf, int n) {
	printf("---- %s ----\n", title);
	for (int i = 0; i < n; i++) {
		printf("0x%02X ", (unsigned char)buf[i]);
		if ((i + 1) % 8 == 0)
			printf("\n");
	}
	if (n % 8 != 0)
		printf("\n");
	printf("----------------\n");
}
/*
第 1 个参数 buf → %rdi
第 2 个参数 len → %esi（32 位用 esi）
第 3 个参数 s → %rdx
返回值 int → %eax*/
int restore_student(char *buf, int len, student *s)
{
    int offset = 0;
    int count = 0;

    while (offset < len) {
        /* name */
        char *p = s[count].name;
        while ((*p++ = buf[offset++]));
        /* age：一条语句整体读入 */
        memcpy(&s[count].age, buf + offset, sizeof(s[count].age));
        offset += sizeof(short);
        /* score：一条语句整体读入 */
        memcpy(&s[count].score, buf + offset, sizeof(s[count].score));
        offset += sizeof(float);
        /* remark */
        p = s[count].remark;
        while ((*p++ = buf[offset++]));
        count++;
    }
    return count;
}
