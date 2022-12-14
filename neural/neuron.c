
/* Include头文件 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


/*符号常数的定义*/
#define INPUTNO 2  /*输入数*/
#define MAXINPUTNO 100    /*数据的最大个数*/

/*函数原型的声明*/
double f(double u); /*传递函数*/
void initw(double w[INPUTNO + 1]);

/*权重和阈值的初始化*/
double forward(double w[INPUTNO + 1], double e[INPUTNO]); /*前向计算*/
int getdata(double e[][INPUTNO]);/*读入数据*/

/*******************/
/*    main()函数   */
/*******************/
int main() {
    double w[INPUTNO + 1];/*权重和阈值*/
    double e[MAXINPUTNO][INPUTNO];/*数据集*/
    double o;/*输出*/
    int i, j;/*循环的控制*/
    int n_of_e;/*数据的个数*/

    /*权重的初始化*/
    initw(w);

    /*读入输入数据*/
    n_of_e = getdata(e);
    printf("数据的个数:%d\n", n_of_e);

    /*计算的主体*/
    for (i = 0; i < n_of_e; ++i) {
        printf("%d ", i);
        for (j = 0; j < INPUTNO; ++j)
            printf("%lf ", e[i][j]);
        o = forward(w, e[i]);
        printf("%lf\n", o);
    }

    return 0;
}

/**********************/
/*  getdata()函数     */
/*读入学习数据        */
/**********************/
int getdata(double e[][INPUTNO]) {
    int n_of_e = 0;/*数据集的个数*/
    int j = 0;/*循环的控制用*/

    FILE *fin;
    fin = fopen("majority.txt", "r");


    /*数据的输入*/
    if (fin) {
        int err = ferror(fin);

        while (fscanf(fin, "%lf", &e[n_of_e][j]) != EOF) {
            ++j;
            if (j >= INPUTNO) {/*下一个数据*/
                j = 0;
                ++n_of_e;
            }
        }
    } else {
        perror("1234");
    }

    return n_of_e;
}

/**********************/
/*  forward()函数     */
/*  前向计算          */
/**********************/
double forward(double w[INPUTNO + 1], double e[INPUTNO]) {
    int i;/*循环的控制*/
    double u, o;/*中间的计算值u和输出值o*/

    /*计算的主体*/
    u = 0;
    for (i = 0; i < INPUTNO; ++i)
        u += e[i] * w[i];
    u -= w[i];/*阈值的处理*/
    /*输出值的计算*/
    o = f(u);
    return o;
}

/**********************/
/*    initw()函数     */
/*   权重的初始化     */
/**********************/
void initw(double w[INPUTNO + 1]) {

    /*给权重固定的数*/
    w[0] = 1;
    w[1] = 1;
    w[2] = 1.5;
// w[2]=0.5 ;

}


/*******************/
/* f()函数         */
/* 传递函数        */
/*******************/
double f(double u) {
    /*阶梯函数的计算*/
    if (u >= 0) return 1.0;
    else return 0.0;

    /*sigmoid函数的计算*/
// return 1.0/(1.0+exp(-u)) ;
}



