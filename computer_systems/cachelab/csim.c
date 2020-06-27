#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include "cachelab.h"
#include <math.h>

typedef unsigned long long mem_address;

//all of the cache parameters
typedef struct {
    int s;
    int E;
    int b;
    int hits;
    int misses;
    int evicts;
    long S;
    long B;
} cache_var; 

//line sturct
typedef struct {
    int last_used;
    int valid;
    mem_address tag;
} set_line; 

//pointer struct to point last_used in set_line struct
typedef struct {
    set_line *lines;
} cache_set; 

//pointer struct to point first line in each set
typedef struct {
    cache_set *sets;
} cache;

//print details of csim
void print_help(); 

//initialize the cache
cache init_cache(long S, long B, int E); 

//free cache malloc after simulation
void free_malloc(cache used, long S, long B, int E); 

//update cache detecting hits, misses and evicts during simulation
cache_var update_cache(cache using, cache_var var, mem_address address); 

//detect a evicted cache line
int detect_evict_line(cache_set determined_set, cache_var var, int *max_used_return); 

//detect a empty cache line
int detect_empty_line(cache_set determined_set, cache_var var); 

int main(int argc, char **argv)
{
    //initialize parameters for a new cache
    cache_var var;
    cache new_cache;
    var.hits = 0;
    var.misses = 0;
    var.evicts = 0;
    
    //initialize parameters for trace files
    FILE *trace;
    char *file_name;
    char instruction;
    mem_address address;
    int size;

    char opt;
    //detect arguments on command line and put the values in initialized parameters
    while( (opt = getopt(argc,argv,"s:E:b:t:vh")) != -1)
	{
        switch(opt)
		{
        case 's':
            var.s = atoi(optarg);
            var.S = pow(2,var.s); //the number of sets
            break;
        case 'E':
            var.E = atoi(optarg); //the number of lines
            break;
        case 'b':
            var.b = atoi(optarg);
            var.B = pow(2,var.b); //the size of a block(Bytes)
            break;
        case 't':
            file_name = optarg;
            break;
        case 'v':
            break;
        case 'h':
            print_help();
            exit(0);
        default:
            printf("./csim: Missing required command line argument\n");
            print_help();
            exit(1);
        }
    }

    //wrong values detection
    if (var.s == 0 || var.E == 0 || var.b == 0 || file_name == NULL) 
	{
        printf("./csim: Missing required command line argument\n");
        print_help();
        exit(1);
    }

    new_cache = init_cache(var.S, var.B, var.E);
    trace  = fopen(file_name, "r");

    //update the cache and parameters(hits, misses and evicts) opening the trace file
    if(trace != NULL){
		while(fscanf(trace, " %c %llx,%d", &instruction, &address, &size) == 3){
			switch(instruction){
				case 'I': //instruction load
					break;
				case 'L': //data load
					var = update_cache(new_cache, var, address);
					break;
				case 'S': //data store 
					var = update_cache(new_cache, var, address);
					break;
				case 'M': //data modify(load -> modify -> store; double cache use)
					var = update_cache(new_cache, var, address);
					var = update_cache(new_cache, var, address);	
					break;
				default:
					break;
			}
		}
	}

    printSummary(var.hits, var.misses, var.evicts);
    free_malloc(new_cache, var.S, var.B, var.E);
    fclose(trace);
    return 0;
}

void print_help(){
    printf("Usage: ./csim [-hv] -s <num> -E <num> -b <num> -t <file>\n");
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of set index bits.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of block offset bits.\n");
    printf("  -t <file>  Trace file.\n");
    printf("\nExamples:\n");
    printf("  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n");
    printf("  ./csim -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
    exit(0);
}

cache init_cache(long S, long B, int E){
    cache new_cache;
    cache_set set;
    set_line line;

    new_cache.sets = (cache_set *)malloc(sizeof(cache_set) * S);
    //bind sets
    for(int i = 0; i < S; i++){
        set.lines = (set_line *)malloc(sizeof(set_line) * E);
        new_cache.sets[i] = set;
        //set all values to zero and bind lines
        for(int j = 0; j < E; j++){
            line.last_used = 0;
            line.valid = 0;
            line.tag = 0;
            set.lines[j] = line;
        }
    }
    return new_cache;
}

void free_malloc(cache used, long S, long B, int E){
    for(int i = 0; i < S; i++){
        free(used.sets[i].lines);
    }
    free(used.sets);
}

cache_var update_cache(cache using, cache_var var, mem_address address){
    //make tag and set index
    int tag_size = 64 - var.s - var.b;
    mem_address tag_bits = address >> (var.s + var.b);
    mem_address temp = address << tag_size;
    mem_address set_index = temp >> (tag_size + var.b);
    
    int pre_hits = var.hits;
    int cache_full = 1;

    cache_set determined_set = using.sets[set_index];

    //determine hit
    for(int i = 0; i < var.E; i++){
        if(determined_set.lines[i].valid){
            if(determined_set.lines[i].tag == tag_bits){
                determined_set.lines[i].last_used ++;
                var.hits ++;
            }
        }
        else{
            if(cache_full) cache_full = 0;
        }
    }
    //determine miss
    if(pre_hits == var.hits){
        var.misses ++;
    }
    else{
        return var; //cache hit
    }

    /*if cache miss is counted, then start to find a line that will be evicted*/
    
    //To return one more values from detect_evict_line, use a local variable with pointer
    int LRU_tmp;
    int *max_used_return = &LRU_tmp;
    //detect the oldest line and find the maximum value of last_used
    int LRU_index = detect_evict_line(determined_set, var, max_used_return);

    if(cache_full){
        var.evicts++;
        determined_set.lines[LRU_index].tag = tag_bits;
        determined_set.lines[LRU_index].last_used = LRU_tmp + 1;
    }
    else{
        int empty_index = detect_empty_line(determined_set, var);
        determined_set.lines[empty_index].tag = tag_bits;
        determined_set.lines[empty_index].valid = 1;
        determined_set.lines[empty_index].last_used = LRU_tmp + 1;
    }
    return var;
}

int detect_evict_line(cache_set determined_set, cache_var var, int *max_used_return){
    int min_used = determined_set.lines[0].last_used;
    int max_used = determined_set.lines[0].last_used;
    int LRU_index = 0;
    for(int i = 0; i < var.E; i++){
        if(min_used > determined_set.lines[i].last_used){
            LRU_index = i;
            min_used = determined_set.lines[i].last_used;
        }
        if((max_used < determined_set.lines[i].last_used)){
            max_used = determined_set.lines[i].last_used;
        }
    }
    *max_used_return = max_used; //return max_used through the LRU_tmp's pointer(max_used_return)
    return LRU_index;
}

int detect_empty_line(cache_set determined_set, cache_var var){
    for(int i = 0; i < var.E; i++){
        //find empty line which valid is zero
        if(determined_set.lines[i].valid == 0){
            return i;
        }
    }
    return -1;
}