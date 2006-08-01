
/*
 * This program tests most of the cliquer functions for errors.  It is
 * given a graph file on the command-line.  It finds certain types of
 * cliques using cliquer, and then checks the results using a very simple
 * exhaustive search (which can be very slow).
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <sys/types.h>

#include "cliquer.h"

#define MAX_VERTEX 2000      /* Maximum size of graph. */
#define MAX_CLIQUES 2000000  /* Aborts if more that this many cliques
			      * are found so as not to run out of memory. */


/* Reordering to correct resulting cliques. */
int *rndorder=NULL;

set_t uw_single;
boolean uw_single_found=FALSE;
set_t w_single;
boolean w_single_found=FALSE;
set_t maximal_single;
boolean maximal_single_found=FALSE;


set_t *w_cliques=NULL;
int w_cliques_size=0;
int w_cliques_count=0;
int w_cliques_count2=0;
set_t *uw_cliques=NULL;
int uw_cliques_size=0;
int uw_cliques_count=0;
int uw_cliques_count2=0;
set_t *maximal_cliques=NULL;
int maximal_cliques_size=0;
int maximal_cliques_count=0;
int maximal_cliques_count2=0;


int max_size, search_size;
int max_weight, search_weight;
int maximal_search_min, maximal_search_max;


graph_t *g;

boolean store_clique(set_t s,graph_t *g,clique_options *opts);
void brutecheck(void);
boolean my_print_time(int i,int n,int m,double f);


int main(int argc, char *argv[]) {
	FILE *fp;

	setlinebuf(stdout);

	if (argc!=2) {
		fprintf(stderr,"%s <dimacs-file>\n",argv[0]);
		return 1;
	}

	fp=fopen(argv[1],"rb");
	if (fp==NULL) {
		perror(argv[1]);
		return 1;
	}
	g=graph_read_dimacs(fp);
	fclose(fp);
	
	if (!graph_test(g,stdout)) {
		fprintf(stderr,"bad graph!\n");
		return 2;
	}

	if (g->n >= MAX_VERTEX) {
		fprintf(stderr,"ERROR: g->n >= MAX_VERTEX!\n");
		return 1;
	}

	printf("Running cliquer...\n");

	rndorder=reorder_by_random(g,FALSE);
	reorder_graph(g,rndorder);
	reorder_invert(rndorder,g->n);

	max_size=clique_unweighted_max_size(g,NULL);
	max_weight=clique_max_weight(g,NULL);

	search_size=MAX(max_size-1,1);
	search_weight=MAX(max_weight*3/4,1);
	maximal_search_min=MAX(max_weight-3,1);
	maximal_search_max=MAX(max_weight-1,1);

	printf("search_size=%d search_weight=%d\n",search_size,search_weight);

	uw_single=clique_unweighted_find_single(g,0,0,FALSE,NULL);
	if (uw_single==NULL) {
		fprintf(stderr,"no max graph!\n");
		return 3;
	}
	reorder_set(uw_single,rndorder);

	w_single=clique_find_single(g,0,0,TRUE,NULL);
	if (w_single==NULL) {
		fprintf(stderr,"no max weight graph!\n");
		return 3;
	}
	reorder_set(w_single,rndorder);

	maximal_single=clique_find_single(g,maximal_search_min,0,TRUE,NULL);
	if (maximal_single==NULL) {
		fprintf(stderr,"no heavy enough (%d) maximal graph!\n",
			maximal_search_min);
		return 3;
	}
	reorder_set(maximal_single,rndorder);

	clique_default_options->user_function=store_clique;
	clique_default_options->user_data=(void *)0;
	clique_unweighted_find_all(g,search_size,0,FALSE,NULL);

	clique_default_options->user_data=(void *)1;
	clique_find_all(g,search_weight,0,FALSE,NULL);

	clique_default_options->user_data=(void *)2;
	clique_find_all(g,maximal_search_min,maximal_search_max,TRUE,
			clique_default_options);

	printf("max_size = %d max_weight = %d uw_cliques_count = %d "
	       "w_cliques_count = %d\n"
	       "maximal_min = %d maximal_max = %d "
	       "maximal_cliques_count = %d\n",
	       max_size,max_weight,uw_cliques_count,w_cliques_count,
	       maximal_search_min,maximal_search_max,maximal_cliques_count);
	fprintf(stderr,"max_size = %d max_weight = %d uw_cliques_count = %d "
	       "w_cliques_count = %d maximal_min = %d maximal_max = %d "
	       "maximal_cliques_count = %d\n",
	       max_size,max_weight,uw_cliques_count,w_cliques_count,
	       maximal_search_min,maximal_search_max,maximal_cliques_count);

	reorder_graph(g,rndorder);

	printf("Running brute check...\n");

	brutecheck();

	if (uw_cliques_count != uw_cliques_count2) {
		fprintf(stderr,"cliquer found too many unweighted! (%d!=%d)\n",
			uw_cliques_count,uw_cliques_count2);
		return 14;
	}
	if (w_cliques_count != w_cliques_count2) {
		fprintf(stderr,"cliquer found too many weighted! (%d!=%d)\n",
			w_cliques_count,w_cliques_count2);
		return 14;
	}
	if (maximal_cliques_count != maximal_cliques_count2) {
		fprintf(stderr,"cliquer found too many maximal! (%d!=%d)\n",
			maximal_cliques_count,maximal_cliques_count2);
		return 14;
	}


	printf("Everything OK!\n");
	fprintf(stderr,"Everything OK!\n");

	return 0;
}





boolean store_clique(set_t set,graph_t *g,clique_options *opts) {
	set_t s;

	s=set_duplicate(set);
	reorder_set(s,rndorder);

	if (opts->user_data==(void *)0) {
		/* Unweighted */
		if (uw_cliques_count>=uw_cliques_size) {
			uw_cliques=realloc(uw_cliques,
					  (uw_cliques_size+64)*sizeof(int));
			uw_cliques_size+=64;
		}
		uw_cliques[uw_cliques_count]=s;
		uw_cliques_count++;
		if (uw_cliques_count>MAX_CLIQUES) {
			fprintf(stderr,"Too many cliques, aborting.\n");
			exit(10);
		}
	} else if (opts->user_data==(void *)1) {
		/* Weighted */
		if (w_cliques_count>=w_cliques_size) {
			w_cliques=realloc(w_cliques,
					  (w_cliques_size+64)*sizeof(int));
			w_cliques_size+=64;
		}
		w_cliques[w_cliques_count]=s;
		w_cliques_count++;
		if (w_cliques_count>MAX_CLIQUES) {
			fprintf(stderr,"Too many cliques, aborting.\n");
			exit(10);
		}
	} else {
		/* Maximal */
		if (maximal_cliques_count>=maximal_cliques_size) {
			maximal_cliques=realloc(maximal_cliques,
						(maximal_cliques_size+64) *
						sizeof(int));
			maximal_cliques_size+=64;
		}
		maximal_cliques[maximal_cliques_count]=s;
		maximal_cliques_count++;
		if (maximal_cliques_count>MAX_CLIQUES) {
			fprintf(stderr,"Too many cliques, aborting.\n");
			exit(10);
		}
	}
	return TRUE;
}



int sub(int ct, int *table, int level, int weight);
/*
 * Checks the graph and results with brute force.
 */
void brutecheck(void) {
	int pos[MAX_VERTEX];
	int i;
	struct tms bf;
	int timer1;
	double timer11;

        /* "start clock" */
        times(&bf);
        timer1 = bf.tms_utime;

	for (i=0; i<MAX_VERTEX; i++)
		pos[i]=i;

	/* main routine */
        for (i = 1; i <= g->n; i++) {
                sub(i, pos, 0, 0);
                times(&bf);
                timer11 = (bf.tms_utime - timer1) / 100.0;
                printf("level = %3d(%d) time = %8.2f\n",i,g->n,timer11);
        }

	if (!uw_single_found) {
		fprintf(stderr,"uw_single not found!\n");
		exit(6);
	}
	if (!w_single_found) {
		fprintf(stderr,"w_single not found!\n");
		exit(6);
	}
	if (!maximal_single_found) {
		fprintf(stderr,"maximal_single not found!\n");
		exit(6);
	}
	return;
}


boolean cliques_identical(set_t s,int *set,int n) {
	int found;
	int i;
	
	if (set_size(s)!=n)
		return FALSE;

	found=TRUE;
	for (i=0; i<n; i++) {
		if (!SET_CONTAINS(s,set[i])) {
			found=FALSE;
			break;
		}
	}
	return found;
}


boolean is_maximal(int *s,int size) {
	int i,j;
	boolean addable;

	for (i=0; i < g->n; i++) {
		addable=TRUE;
		for (j=0; j<size; j++) {
			if (!GRAPH_IS_EDGE(g,i,s[j])) {
				addable=FALSE;
				break;
			}
		}
		if (addable)
			return FALSE;
	}
	return TRUE;
}


int set[MAX_VERTEX];

int sub(int ct, int *table, int size, int weight) {
	int i,j,k;
	int newtable[MAX_VERTEX];
	int *p1,*p2;

	if (weight >= search_weight) {
		if (weight > max_weight) {
			fprintf(stderr,"found heavier clique than cliquer "
				"(%d>%d)!\n",weight,max_weight);
			exit(4);
		}
		w_cliques_count2++;

		k=FALSE;
		for (i=0; i<w_cliques_count; i++) {
			if (cliques_identical(w_cliques[i],set,size)) {
				k=TRUE;
				break;
			}
		}
		if (!k) {
			fprintf(stderr,"cliquer didn't find all weighted!\n");
			exit(5);
		}

		if (cliques_identical(w_single,set,size))
			w_single_found=TRUE;
	}
	
	if (size >= search_size) {
		if (size > max_size) {
			fprintf(stderr,"found larger clique than cliquer "
				"(%d>%d)!\n",size,max_size);
			exit(4);
		}
		uw_cliques_count2++;

		k=FALSE;
		for (i=0; i<uw_cliques_count; i++) {
			if (cliques_identical(uw_cliques[i],set,size)) {
				k=TRUE;
				break;
			}
		}
		if (!k) {
			fprintf(stderr,"cliquer didn't find all unweightd!\n");
			exit(5);
		}

		if (cliques_identical(uw_single,set,size))
			uw_single_found=TRUE;
	}

	if ((ct==0) && (weight >= maximal_search_min) &&
	    is_maximal(set,size)) {
		if (weight <= maximal_search_max) {
			maximal_cliques_count2++;
			
			k=FALSE;
			for (i=0; i<maximal_cliques_count; i++) {
				if (cliques_identical(maximal_cliques[i],
						      set,size)) {
					k=TRUE;
					break;
				}
			}
			if (!k) {
				fprintf(stderr,"cliquer didn't find all "
					"maximal!\n");

				exit(5);
			}
		}

		if (cliques_identical(maximal_single,set,size))
			maximal_single_found=TRUE;
	}

	for (i=ct-1; i>=0; i--) {
		if ((size == 0) && (i < ct-1))
			return 0;
		k = table[i];
		set[size] = k;
		p1 = newtable;
		p2 = table;
		while (p2 < table + i) {
			j = *p2++;
			if (GRAPH_IS_EDGE(g, j, k)) {
				*p1++ = j;
			}
		}
		sub(p1 - newtable, newtable, size + 1, weight+g->weights[k]);
	}
	return 0;
}


#ifndef ABS
#define ABS(a) (((a)>=0)?(a):-(a))
#endif
boolean my_print_time(int i,int n,int m,double f) {
	static float prev_f=100;
	static int prev_i=100;
	static int prev_m=100;

	if (ABS(prev_f-f)>0.1 || i==n || i<prev_i || m!=prev_m) {
		if (f<prev_f || i<=prev_i)
			fprintf(stderr,"%3d/%d (max %2d)  %2.2f s\n",i,n,m,f);
		else
			fprintf(stderr,"%3d/%d (max %2d)  %2.2f s  "
				"(%2.2f s/round)\n",
				i,n,m,f,(f-prev_f)/(i-prev_i));
		prev_f=f;
		prev_i=i;
		prev_m=m;
	}
	return TRUE;
}

