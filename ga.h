/*/==========================================
 * orders:
 *	1. initial
 *	2. reproduction from "population" to "pool"
 *	3. needCrossover?
 *		Y: crossover from pool to "new population"
 *		N: skip(copy) to "new population"
 *	4. in new population
 *		mutation?
 *
 *	go back to order 1.
 *
 *	Q&A
 *	1. crossover: all, part, all not?
==========================================*/

#include <iostream>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "parent.h"

using namespace std;

class ga{
	private:
		int num_run;
		int genetic_len;
		int population_cnt; //群體數量
		double mutation_rate;
		double crossover_rate;

		parent* population;
		parent* pool;
		
	public:
		ga(int num_run,
			int population_cnt,
			int genetic_len,
			double mutation_rate,
			double crossover_rate);
		void run();
		void init();
		void reproduction(); //複製
		void crossover();
		void mutation(); //突變
		void cal_fitness(parent*); //cal 適應值
		void result();
		~ga(){
			delete [] population;
			delete [] pool;
		}

};

ga::ga(int num_run, int population_cnt, int genetic_len, double mutation_rate, double cross_rate):
		num_run(num_run), population_cnt(population_cnt), genetic_len(genetic_len), 
		mutation_rate(mutation_rate), crossover_rate(cross_rate){
	srand(time(0));
	population = new parent[population_cnt];
	pool = new parent[population_cnt];
	for(int i=0; i<population_cnt; i++){
		population[i].genes=new int[genetic_len];
		pool[i].genes=new int[genetic_len];
	}
}

void ga::run(){

	// 1. initialize
	init();
	
	for (int i=0; i<num_run; i++){
		//2. reproduction to crossover pool
		reproduction();

		//3. crossover to new population
		crossover();

		//4. some of new population may mutate
		mutation();

		//5. print the population result after going through 2~4
		result();
	}

}

void ga::result(){
	for (int i=0; i<population_cnt; i++){
		for (int j=0; j<genetic_len; j++)
			cout << population[i].genes[j] << " ";
		cout << endl;
	}
	cout << "==========================\n";

}

void ga::init(){
	for (int i=0; i<population_cnt; i++){
		for(int j=0; j<genetic_len; j++){
			population[i].genes[j]=rand()%2;
			cout << population[i].genes[j] << " ";
		}
		cout << endl;
		cal_fitness(&population[i]);
	}
	cout << "==========================\n";
}

void ga::cal_fitness(parent* fit){
	int val=0;
	for(int i=0; i<genetic_len; i++){
		if(fit->genes[i]==1)
			val++;
	}
	fit->fitness=val;
}

//以輪盤概念(機率累加), 選擇適當區間來複製
void ga::reproduction(){
	double total_fitness=0;
	double interval[population_cnt];
	for(int i=0; i<population_cnt; i++){
		total_fitness += population[i].fitness;
	}	
	interval[0] = population[0].fitness/total_fitness;
	for(int i=1; i<population_cnt; i++){
		interval[i] = interval[i-1] + (population[i].fitness/total_fitness);
	}
	
	// 隨機抽population_cnt個複製到交配池
	for (int i=0; i<population_cnt; i++){
		double prob = (double)rand()/RAND_MAX;
		int pos;

		for(pos=0; pos<population_cnt-1; pos++){
			// 找到落在的位置
			if (prob <= interval[pos]){
				break;
			}
		}

		memcpy(&pool[i], &population[pos], sizeof(parent));
	}
	cout << " after reproduction, in popu \n";
	result();
	cout << " after reproduction, in pool \n";
	for (int i=0; i<population_cnt; i++){
		for (int j=0; j<genetic_len; j++){
			cout << pool[i].genes[j] << " ";
		}
		cout << endl;
	}
}

void ga::mutation(){
	int pos; //要突變的位置
	double rate;
	
	cout << "in mutation block" << endl;
	for(int i=0; i<population_cnt; i++){
		rate=(double)rand()/RAND_MAX;
		
		//要突變的情況
		if(rate<=mutation_rate){
			pos=rand()%genetic_len;
			population[i].genes[pos]=!(population[i].genes[pos]);
			cal_fitness(&population[i]);
			cout << "mutation population: " << i << ", pos: " << pos << endl;
		}
	}
}

// 3. 從交配池選擇交配對象 到新一代
void ga::crossover(){
	double rate; 
	int flag[population_cnt];
	for (int i=0; i<population_cnt; i++)
		flag[i]=0;
	//int flag[population_cnt]={0}; // 記錄是否已被選到下一代
	int cnt=0;

	while(cnt<population_cnt){
		rate = (double)rand()/RAND_MAX;
		int p1, p2; 
		int pos; //交配位置

		do{
			p1=rand()%population_cnt;
			p2=rand()%population_cnt;
		}while( (p1==p2) || (flag[p1]!=0) || (flag[p2]!=0) );
		//cout << "in cross, p1: " << p1 << " p2: " << p2 << endl;
		flag[p1]=flag[p2]=1;

		//不交配情況
		if(rate > crossover_rate){
			memcpy(&population[cnt], &pool[p1], sizeof(parent));	
			memcpy(&population[cnt+1], &pool[p2], sizeof(parent));	
			//cout << "no crossover" << endl;
		}
		else{
			//找交配位置，並進行交配
			do{
				pos=rand()%genetic_len;
			}while(pos==genetic_len-1);
			
			/* if you want to check the executing pross, you can take off the coments in this block
			cout << "need crossover, before cross" << endl;
			for (int i=0; i<genetic_len; i++){
				cout << pool[p1].genes[i] << " ";
			}
			cout << endl;
			for (int i=0; i<genetic_len; i++){
				cout << pool[p2].genes[i] << " ";
			}
			cout << endl;
			
			cout << "cross over" << ", pos: " << pos << endl;
			*/

			parent* tmp_pool=new parent[2];
			tmp_pool[0].genes=new int[genetic_len];
			tmp_pool[1].genes=new int[genetic_len];

			for (int i=0; i<=pos; i++){
				tmp_pool[0].genes[i]=pool[p1].genes[i];
				tmp_pool[1].genes[i]=pool[p2].genes[i];
			}
			for (int i=pos+1; i<genetic_len; i++){
				tmp_pool[0].genes[i]=pool[p2].genes[i];
				tmp_pool[1].genes[i]=pool[p1].genes[i];
			}
			memcpy((void*)&population[cnt], (void*)&tmp_pool[0], sizeof(parent));
			memcpy((void*)&population[cnt+1], (void*)&tmp_pool[1], sizeof(parent));

			/* the coment in this block is printing the population result after crossover
			for (int i=0; i<genetic_len; i++){
				cout << population[cnt].genes[i] << " ";
			}
			cout << endl;
			for (int i=0; i<genetic_len; i++){
				cout << population[cnt+1].genes[i] << " ";
			}
			*/
		}
		cnt+=2;
		cout << "cnt " << cnt << endl;
	}
	cout << "end crossover result" << endl;
	result();
}
