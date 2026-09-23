set pagination off
set debuginfod enabled off
break print_hex
run
up
printf "\n(gdb) x/40bx message\n"
x/40bx message
printf "\n(gdb) x/4bx &old_s[0].score\n"
x/4bx &old_s[0].score
printf "\n(gdb) p sizeof(student)\n"
p sizeof(student)
printf "\n(gdb) member offsets: name, age, score, remark\n"
p (char *)&old_s[0].name - (char *)&old_s[0]
p (char *)&old_s[0].age - (char *)&old_s[0]
p (char *)&old_s[0].score - (char *)&old_s[0]
p (char *)&old_s[0].remark - (char *)&old_s[0]
printf "\n(gdb) array element stride\n"
p (char *)&old_s[1] - (char *)&old_s[0]
