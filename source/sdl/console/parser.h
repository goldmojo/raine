#ifndef PARSER_H
#define PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

extern double sr, pc, a[8], d[8],za,zb,zc,zd,ze,zf,zh,zl,iff;

extern int get_cpu_id();
int parse(char *expr);
void get_regs(int cpu = get_cpu_id());
void set_regs(int cpu = get_cpu_id());
void set_nb_scripts(int n);
void set_script_param(int n,int myp);
void init_script_param(int n);

#ifdef __cplusplus
}
#endif
#endif
