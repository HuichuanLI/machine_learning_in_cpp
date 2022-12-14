//
// Created by lhc456 on 2022/10/30.
//

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/*符号常数的定义*/
#define MAXVALUE 100 /*重量和价值的最大值*/
#define N 30 /*行李个数*/
#define WEIGHTLIMIT (N*MAXVALUE/4) /*重量限制*/
#define POOLSIZE 30 /*染色体个数*/
#define LASTG 50    /*终止后代*/
#define MRATE 0.01  /*突变的概率*/
#define SEED 32767    /*随机数的seed*/
#define YES 1   /*yes对应的整数值*/
#define NO 0    /*no对应的整数值*/

/*函数原型声明*/
void initparcel();/*行李的初始化*/
int evalfit(int gene[]); /*计算适应度*/
void mating(int pool[POOLSIZE][N], int ngpool[POOLSIZE * 2][N]); /*交叉*/
void mutation(int ngpool[POOLSIZE * 2][N]); /*突变*/
void printp(int pool[POOLSIZE][N]); /*输出结果*/
void initpool(int pool[POOLSIZE][N]);

/*生成初始染色体集合*/
int rndn(); /*生成n之下的随机数   */
int notval(int v);/*真值的反转*/
int selectp(int roulette[POOLSIZE], int totalfitness);

/*选择父代*/
void crossing(int m[], int p[], int c1[], int c2[]);

/* 两个特定染色体的交叉 */
void selectng(int ngpool[POOLSIZE * 2][N], int pool[POOLSIZE][N]);/*选择下一代*/

/*全局变量（行李数据）*/
int parcel[N][2];/*行李*/

int main() {
    int pool[POOLSIZE][N]; /*染色体集合*/
    int ngpool[POOLSIZE * 2][N]; /*下一代染色体集合*/
    int generation;/* 现在的代数 */

    /*随机数的初始化*/
    srand(SEED);

    /*行李的初始化*/
    initparcel();

    /*生成初始染色体集合*/
    initpool(pool);

    /*循环直至终止子代*/
    for (generation = 0; generation < LASTG; ++generation) {
        printf("%d代数\n", generation);
        mating(pool, ngpool);/*交叉*/
        mutation(ngpool);/*突变*/
        selectng(ngpool, pool);/*选择下一子代*/
        printp(pool);/*输出结果*/
    }

    return 0;
}


/***********************/
/*   initpool()函数    */
/*   生成初始染色体集合*/
/***********************/
void initpool(int pool[POOLSIZE][N]) {
    int i, j;/* 循环控制变量 */

    for (i = 0; i < POOLSIZE; ++i)
        for (j = 0; j < N; ++j)
            pool[i][j] = rndn(2);
}


void initparcel() {
    int i = 0;
    while ((i < N) &&
           (scanf("%d %d", &parcel[i][0], &parcel[i][1])
            != EOF)) {
        ++i;
    }
}

int rndn(int l) {
    int rndno;
    while ((rndno = ((double) rand() / RAND_MAX) * l) == l);
    return rndno;
}


void selectng(int ngpool[POOLSIZE * 2][N], int pool[POOLSIZE][N]) {
    int i, j, c;/* 循环控制参数 */
    int totalfitness = 0;/*适应度的总计值*/
    int roulette[POOLSIZE * 2];/*存放适应度*/
    int ball;/* 球（选择位置的数值）*/
    int acc = 0;/*适应度的累积值*/

    for (i = 0; i < POOLSIZE; ++i) {
        /* 生成轮盘 */
        totalfitness = 0;
        for (c = 0; c < POOLSIZE * 2; ++c) {
            roulette[c] = evalfit(ngpool[c]);
            /*计算适应度的总计值*/
            totalfitness += roulette[c];
        }
        /*选择一个染色体*/
        ball = rndn(totalfitness);
        acc = 0;
        for (c = 0; c < POOLSIZE * 2; ++c) {
            acc += roulette[c];/*累积评价值*/
            if (acc > ball) break;/*对应的基因*/
        }

        /*染色体的复制*/
        for (j = 0; j < N; ++j) {
            pool[i][j] = ngpool[c][j];
        }
    }
}


int selectp(int roulette[POOLSIZE], int totalfitness) {

    int i;/* 循环的控制变量 */
    int ball;/* 球（选择位置的数值）*/
    int acc = 0;/*评价值的累积值*/

    ball = rndn(totalfitness);

    for (int i = 0; i < POOLSIZE; ++i) {
        acc += roulette[i];
        if (acc > ball) break;
    }
    return i;
}


int notval(int v) {
    if (v == YES) return NO;
    else return YES;
}


void mutation(int ngpool[POOLSIZE * 2][N]) {
    int i, j;/* 循环的控制变量 */

    for (i = 0; i < POOLSIZE * 2; ++i)
        for (j = 0; j < N; ++j)
            if ((double) rndn(100) / 100.0 <= MRATE)
                /*反转的突变*/
                ngpool[i][j] = notval(ngpool[i][j]);

}


void mating(int pool[POOLSIZE][N], int ngpool[POOLSIZE * 2][N]) {
    int i;/* 循环的控制变量 */
    int totalfitness = 0;/*评价值的总计值*/
    int roulette[POOLSIZE];/*存放评价值*/
    int mama, papa;/*父代的基因的号码*/

    /* 生成轮盘 */
    for (i = 0; i < POOLSIZE; ++i) {
        roulette[i] = evalfit(pool[i]);
        /* 计算评价值的总计值*/
        totalfitness += roulette[i];
    }

    /*选择和交叉的循环*/
    for (i = 0; i < POOLSIZE; ++i) {
        do {/*父代的选择*/
            mama = selectp(roulette, totalfitness);
            papa = selectp(roulette, totalfitness);
        } while (mama == papa);/*删除重复的*/

        /*特定2染色体的交叉*/
        crossing(pool[mama], pool[papa], ngpool[i * 2], ngpool[i * 2 + 1]);
    }
}

void crossing(int m[], int p[], int c1[], int c2[]) {
    int j;/* 循环控制变量 */
    int cp;/*交叉的点*/

    /*确定交叉点*/
    cp = rndn(N);

    /*复制前半部分*/
    for (j = 0; j < cp; ++j) {
        c1[j] = m[j];
        c2[j] = p[j];
    }
    /*复制后半部分*/
    for (; j < N; ++j) {
        c2[j] = m[j];
        c1[j] = p[j];
    }
}

void printp(int pool[POOLSIZE][N]) {
    int i, j;/* 循环的控制变量 */
    int fitness;/* 评价值 */
    double totalfitness = 0;/* 评价值的总计值 */
    int elite, bestfit = 0;/*精英基因的处理用变量*/

    for (i = 0; i < POOLSIZE; ++i) {
        for (j = 0; j < N; ++j)
            printf("%1d", pool[i][j]);
        fitness = evalfit(pool[i]);
        printf("\t%d\n", fitness);
        if (fitness > bestfit) {/*精英解*/
            bestfit = fitness;
            elite = i;
        }
        totalfitness += fitness;
    }
    /*输出精英解的评价值*/
    printf("%d\t%d \t", elite, bestfit);
    /*输出平均评价值*/
    printf("%lf\n", totalfitness / POOLSIZE);
}


int evalfit(int g[]) {
    int pos;/*指定基因位点*/
    int value = 0;/*评价值*/
    int weight = 0;/*重量*/

    /*调查各个基因位点计算重量和评价值*/
    for (pos = 0; pos < N; ++pos) {
        weight += parcel[pos][0] * g[pos];
        value += parcel[pos][1] * g[pos];
    }
    /*致死基因的处理*/
    if (weight >= WEIGHTLIMIT) value = 0;
    return value;

}